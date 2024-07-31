#pragma once
#include "interface.h"
#include "definition.h"
#include <array>
#include <vector>
#include <variant>
#include <optional>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <ranges>
#include <bit>
#include <span>
#include <fstream>
#include <iostream>     // For local debug only
#include <filesystem>   // For local debug only

namespace oj {

struct OJException : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct UserException : public OJException {
    using OJException::OJException;
};

struct SystemException : public OJException {
    using OJException::OJException;
};

template <typename _Error = UserException>
[[noreturn]]
static void panic(const std::string &message) {
    throw UserException { message };
}

struct ServiceInfo {
    priority_t complete;
    priority_t total;
};

struct RuntimeManager : public PublicInformation {
private:
    struct TaskFree {
        /* Nothing. */
    };
    struct TaskLaunch {
        cpu_id_t cpu_cnt;
        time_t   start;
    };
    struct TaskSaving {
        cpu_id_t cpu_cnt;
        time_t   finish;
        double   time_passed;
    };

    struct TaskStatus {
        using WorkLoad = std::variant <TaskFree, TaskLaunch, TaskSaving>;
        WorkLoad workload;
        double time_passed; // Total time passed.
        const time_t deadline;
    };

    // Return time from when the task have done.
    auto time_policy(const TaskLaunch &launch) const -> double {
        const auto distance = get_time() - launch.start;
        return oj::time_policy(distance, launch.cpu_cnt);
    }

    void launch_check(const Launch &command) const {
        const auto [cpu_cnt, task_id] = command;
        if (cpu_cnt == 0)
            panic("CPU count should not be zero.");
        if (cpu_cnt > kCPUCount)
            panic("CPU count exceeds the kMaxCPU limit.");
        if (task_id >= global_tasks)
            panic("Task ID out of range.");
        const auto &workload = task_state[task_id].workload;
        if (!holds_alternative <TaskFree> (workload))
            panic("Task is not free.");
    }

    // From free -> launch.
    void launch_commit(const Launch &command) {
        const auto [cpu_cnt, task_id] = command;
        auto &workload = task_state[task_id].workload;

        this->cpu_usage += cpu_cnt;

        workload = TaskLaunch {
            .cpu_cnt    = cpu_cnt,
            .start      = get_time(),
        };
    }

    void saving_check(const Saving &command) const {
        const auto [task_id] = command;
        if (task_id >= global_tasks)
            panic("Task ID out of range.");

        const auto &workload = task_state[task_id].workload;
        if (!holds_alternative <TaskLaunch> (workload))
            panic("Task is not launched.");
    }

    // From launch -> saving.
    void saving_commit(const Saving &command) {
        const auto [task_id] = command;

        auto &task = task_state[task_id];
        auto &workload = task.workload;
        const auto &launch = get <TaskLaunch> (workload);
        const auto time_sum = this->time_policy(launch);

        task_saving.push_back(&task);
        auto [cpu_cnt, start] = launch;
        workload = TaskSaving {
            .cpu_cnt    = cpu_cnt,
            .finish     = get_time() + kSaving,
            .time_passed = time_sum,
        };
    }

    /* Counting all the tasks in this cycle. */
    auto get_new_tasks() -> std::vector <Task> {
        auto &which = this->global_tasks;
        const auto start = which;

        while (which < task_list.size()
        && task_list[which].launch_time == get_time())
            which += 1;

        auto begin = task_list.begin();
        return std::vector <Task> (begin + start, begin + which);
    }

    void work(const Launch &command) {
        // std::cerr << "Commit launch " << command.task_id << ' ' << command.cpu_cnt << std::endl;
        this->launch_check(command);
        this->launch_commit(command);
    }

    void work(const Saving &command) {
        // std::cerr << "Commit saving " << command.task_id << std::endl;
        this->saving_check(command);
        this->saving_commit(command);
    }

    /* Remove those outdated saving file within.  */
    void complete_this_cycle() {
        auto begin = task_saving.begin();
        auto finish = task_saving.end();

        while (begin != finish) {
            auto &task = **begin;
            auto &workload = task.workload;
            if (holds_alternative <TaskSaving> (workload)) {
                auto &saving = get <TaskSaving> (workload);
                if (saving.finish == this->get_time()) {
                    cpu_usage -= saving.cpu_cnt;
                    if (this->get_time() <= task.deadline)
                        task.time_passed += saving.time_passed;
                    workload = TaskFree {};
                    std::swap(*begin, *--finish);
                } else {
                    ++begin;
                }
            } else {
                ++begin;
            }
        }

        task_saving.resize(finish - task_saving.begin());
    }

public:
    explicit RuntimeManager(std::vector <Task> task_list)
        : global_clock(-1), global_tasks(0), cpu_usage(0), task_list(std::move(task_list)) {
        if (!std::ranges::is_sorted(this->task_list, {}, &Task::launch_time))
            panic <SystemException> ("Task list is not sorted.");
        task_state.reserve(this->task_list.size());
        for (auto &task : this->task_list)
            task_state.push_back(TaskStatus {
                .workload       = TaskFree {},
                .time_passed    = 0,
                .deadline       = task.deadline,
            });
    }

    auto synchronize() -> std::vector <Task> {
        this->complete_this_cycle();

        if (this->cpu_usage > kCPUCount)
            panic("CPU usage exceeds the limit.");

        global_clock += 1;
        return this->get_new_tasks();
    }

    void work(std::vector <Policy> p) {
        // if (!p.empty()) std::cerr << "Work at " << this->get_time() << std::endl;
        for (const auto &policy : p) {
            std::visit([this](const auto &command) { this->work(command); }, policy);
        }
    }

    auto get_time() const -> time_t {
        return global_clock;
    }

    auto get_service_info() const -> ServiceInfo {
        ServiceInfo result { .complete = 0, .total = 0 };
        for (task_id_t id = 0; id < global_tasks; ++id) {
            auto &state   = task_state[id];
            auto &task    = task_list[id];
            if (time_t(state.time_passed) >= task.execution_time) {
                result.complete += task.priority;
            }
            result.total += task.priority;
        }
        return result;
    }

private:
    time_t      global_clock;   // A global clock to record the current time
    task_id_t   global_tasks;   // A global task ID counter.
    cpu_id_t    cpu_usage;      // The current CPU usage

    const std::vector <Task> task_list;         // A list of tasks
    std::vector <TaskStatus> task_state;        // A list of task status
    std::vector <TaskStatus *> task_saving;     // A list of working tasks
};

static_assert(std::is_standard_layout_v <Task>);
static_assert(std::is_trivial_v <Task>);

struct Header {
    std::size_t task_count;
    Description description;
    ServiceInfo service_info;
    const std::size_t magic = kMagic;
    // 'D' 'A' 'R' 'K'
    // 'W' 'H' 'A' 'T'
    static constexpr std::size_t kMagic =
        std::size_t(0x4B524144) << 32 | std::size_t(0x54414857);
};

struct OJTask {
    std::size_t description_count;
    const std::size_t magic = kMagic;
    // 'D' 'A' 'R' 'K'
    // 'W' 'H' 'A' 'T'
    static constexpr std::size_t kMagic =
        std::size_t(0x4B524144) << 32 | std::size_t(0x54414857);
};

inline void serialize(std::ostream &os, const OJTask &task) {
    os.write(std::bit_cast <const char *> (&task), sizeof(OJTask));
    if (!os.good())
        panic <SystemException> ("File write failed.");
}

inline void serialize(
    std::ostream &os,
    std::span <const Task> vec,
    Description description,
    ServiceInfo service_info) {
    const auto header = Header {
        .task_count   = vec.size(),
        .description  = description,
        .service_info = service_info,
    };
    os.write(std::bit_cast <const char *> (&header), sizeof(Header));

    const auto size = vec.size() * sizeof(Task);
    os.write(std::bit_cast <const char *> (vec.data()), size);

    if (!os.good())
        panic <SystemException> ("File write failed.");
}

inline auto deserialize(std::istream &is) -> std::vector <Task> {
    Header header;

    is.read(std::bit_cast <char *> (&header), sizeof(Header));
    if (header.magic != header.kMagic)
        panic <UserException> ("Magic number mismatch.");

    std::vector <Task> vec(header.task_count);
    const auto size = vec.size() * sizeof(Task);
    is.read(std::bit_cast <char *> (vec.data()), size);

    if (!is.good())
        panic <SystemException> ("File is corrupted.");

    return vec;
}

} // namespace oj
