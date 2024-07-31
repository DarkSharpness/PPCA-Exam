// These 3 are visible to all.
#include "interface.h"
#include "definition.h"

// These 2 are your implementations.
#include "_generator.h"
#include "_scheduler.h"

// This is only for the judge.
#include "runtime.h"

// Sorted in length of the name.
#include <span>
#include <ranges>
#include <iomanip>
#include <iostream>
#include <algorithm>

using oj::panic;

template <typename _Tp>
static bool within(_Tp x, oj::Range <_Tp> range) {
    return range.min <= x && x <= range.max;
}

static void check_tasks(std::span <const oj::Task> tasks, const oj::Description &desc) {
    if (tasks.size() != desc.task_count)
        panic("The number of tasks is not equal to the number of tasks.");

    oj::time_t execution_time_sum   = 0;
    oj::priority_t priority_sum     = 0;
    for (const auto &task : tasks) {
        if (task.launch_time >= task.deadline)
            panic("The launch time is no earlier to the deadline.");

        if (!within(task.deadline, desc.deadline_time))
            panic("The deadline time is out of range.");

        if (!within(task.execution_time, desc.execution_time_single))
            panic("The execution time is out of range.");

        if (!within(task.priority, desc.priority_single))
            panic("The priority is out of range.");

        execution_time_sum += task.execution_time;
        priority_sum += task.priority;
    }

    if (!within(execution_time_sum, desc.execution_time_sum))
        panic("The total execution time is out of range.");

    if (!within(priority_sum, desc.priority_sum))
        panic("The total priority is out of range.");
}

static auto generate_work(const oj::Description &desc) -> std::vector <oj::Task> {
    auto tasks = oj::generate_tasks(desc);

    std::ranges::sort(tasks, {}, &oj::Task::launch_time);

    /* Check the tasks. */
    check_tasks(tasks, desc);

    return tasks;
}

static auto schedule_work(const oj::Description &desc, std::vector <oj::Task> tasks)
-> oj::ServiceInfo {
    oj::RuntimeManager manager { std::move(tasks) };
    oj::scheudle_reset(desc);
    for (std::size_t i = 0; i <= desc.deadline_time.max; ++i) {
        auto new_tasks = manager.synchronize();
        if (i != manager.get_time())
            panic <oj::SystemException> ("Time is not synchronized");
        manager.work(oj::schedule_tasks(i, std::move(new_tasks)));
    }

    manager.synchronize();

    return manager.get_service_info();
}

enum class JudgeResult {
    GenerateFailed,
    ScheduleFailed,
};

template <JudgeResult _Default_Result>
[[noreturn]]
static void handle_exception(const oj::OJException &e) {
    if (dynamic_cast <const oj::UserException *> (&e)) {
        if constexpr (_Default_Result == JudgeResult::GenerateFailed)
            std::cerr << "Generate failed: " << e.what() << std::endl;
        if constexpr (_Default_Result == JudgeResult::ScheduleFailed)
            std::cerr << "Schedule failed: " << e.what() << std::endl;
    } else { // Unknown system error.
        std::cerr << "System Error: " << e.what() << std::endl;
    }
    std::exit(EXIT_FAILURE);
}

static auto judge(const oj::Description &desc) {
    using enum JudgeResult;
    std::vector <oj::Task> tasks;

    try {
        tasks = generate_work(desc);
    } catch (const oj::OJException &e) {
        handle_exception <GenerateFailed> (e);
    }

    try {
        auto info = schedule_work(desc, tasks);

        std::cerr << "Complete rate: "
            << std::setprecision(2)
            << std::fixed
            << 100 * double(info.complete) / info.total
            << "% ("
            << info.complete
            << "/"
            << info.total
            << ")"
            << std::endl;

        oj::serialize(std::cout, tasks, desc, info);
    } catch (const oj::OJException &e) {
        handle_exception <ScheduleFailed> (e);
    }
}

signed main() {
    try {
        using namespace oj;
        constexpr Description array[] = {
            small,
            middle,
            senpai,
            huge,
        };
        serialize(std::cout, { .description_count = std::size(array) });
        for (const auto &desc : array) judge(desc);
    } catch (const std::exception &e) {
        std::cerr << "System Error: Unexpected std::exception(): " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "System Error: An unknown error occurred!" << std::endl;
    }
    return 0;
}
