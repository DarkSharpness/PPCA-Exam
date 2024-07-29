#pragma once
#include "interface.h"
#include "definition.h"
#include <array>
#include <vector>
#include <variant>
#include <optional>
#include <stdexcept>
#include <cmath>

namespace oj {

struct RuntimeManager {
private:
    static constexpr time_t   kMaxTime  = 1e8;
    static constexpr cpu_id_t kCPUCount = 114;
    static constexpr time_t   kStartUp  = 2;
    static constexpr time_t   kSaving   = 3;

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
        const double time_required;
    };

    time_t      global_clock;   // A global clock to record the current time
    task_id_t   global_tasks;   // A global task ID counter.
    cpu_id_t    free_cpu;      // Free CPU count.

    const std::vector <Task> task_list;         // A list of tasks
    std::vector <TaskStatus> task_state;        // A list of task status
    std::vector <TaskStatus *> task_saving;    // A list of working tasks

    void panic(std::string msg) const {
        throw std::runtime_error(std::move(msg));
    }

    // Return time from when the task have done.
    auto time_policy(const TaskLaunch &launch) const -> double {
        const auto [cpu_cnt, start] = launch;
        const auto distance = global_clock - start;

        if (distance <= kStartUp) return 0; // Within start-up time.

        // Effective core count.
        const auto effective_core = cpu_cnt == 1 ? 1 : 1 + double(cpu_cnt - 1) / 2;
        return effective_core * (distance - kStartUp);
    }

    void work_check(const Launch &command) const {
        const auto [cpu_cnt, task_id] = command;
        if (cpu_cnt == 0)
            return panic("CPU count should not be zero.");
        if (cpu_cnt > (kCPUCount - free_cpu))
            return panic("CPU count exceeds the limit.");
        if (task_id >= global_tasks)
            return panic("Task ID out of range.");

        const auto &workload = task_state[task_id].workload;
        if (!holds_alternative <TaskFree> (workload))
            return panic("Task is not free.");
    }

    // From free -> launch.
    void work_commit(const Launch &command) {
        const auto [cpu_cnt, task_id] = command;
        auto &workload = task_state[task_id].workload;

        free_cpu -= cpu_cnt;
        workload = TaskLaunch {
            .cpu_cnt    = cpu_cnt,
            .start      = global_clock,
        };
    }

    void work_check(const Saving &command) const {
        const auto [task_id] = command;
        if (task_id >= global_tasks)
            return panic("Task ID out of range.");

        const auto &workload = task_state[task_id].workload;
        if (!holds_alternative <TaskLaunch> (workload))
            return panic("Task is not launched.");
    }

    // From launch -> saving.
    void work_commit(const Saving &command) {
        const auto [task_id] = command;
        auto &task = task_state[task_id];
        auto &workload = task.workload;
        const auto &launch = get <TaskLaunch> (workload);
        const auto time_sum = this->time_policy(launch);

        task_saving.push_back(&task);
        auto [cpu_cnt, start] = launch;
        workload = TaskSaving {
            .cpu_cnt    = cpu_cnt,
            .finish     = global_clock + kSaving,
            .time_passed = time_sum
        };
    }

public:
    void synchronize(task_id_t new_task_cnt) {
        global_clock += 1;
        global_tasks += new_task_cnt;
        auto begin = task_saving.begin();
        auto finish = task_saving.end();

        while (begin != finish) {
            auto &task = **begin;
            auto &workload = task.workload;
            if (holds_alternative <TaskSaving> (workload)) {
                auto &saving = get <TaskSaving> (workload);
                if (saving.finish == global_clock) {
                    free_cpu += saving.cpu_cnt;
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

    void work(const Launch &command) {
        this->work_check(command);
        this->work_commit(command);
    }

    void work(const Saving &command) {
        this->work_check(command);
        this->work_commit(command);
    }

    void work(std::vector <Policy> p) {
        for (const auto &policy : p) {
            std::visit([this](const auto &command) { this->work(command); }, policy);
        }
    }
};

} // namespace oj
