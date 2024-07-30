#include "interface.h"
#include "definition.h"
#include "runtime.h"
#include "src.hpp"
#include <algorithm>
#include <iostream>
#include <span>
#include <ranges>
#include <fstream>

static void panic(std::string_view message) {
    std::cerr << message << std::endl;
    std::exit(1);
}

template <typename _Tp>
bool within(_Tp x, oj::Range <_Tp> range) {
    return range.min <= x && x <= range.max;
}

static void check_task(std::span <const oj::Task> tasks, const oj::Description &desc) {
    if (tasks.size() != desc.cpu_count)
        panic("The number of tasks is not equal to the number of CPUs.");

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

static void serialize_tasks(std::span <const oj::Task> tasks, std::string_view filename) {
    static_assert(std::is_trivial_v <oj::Task>, "Can not serialize the non-trivial type.");
    std::ofstream out { std::string(filename), std::ios::binary };
    if (!out.is_open())
        panic("OJ error: Failed to open the file.");

    auto *buffer = reinterpret_cast<const char *>(tasks.data());
    out.write(buffer, tasks.size() * sizeof(oj::Task));

    if (!out.good())
        panic("OJ error: Failed to write the file.");
}

static void work(const oj::Description &desc) {
    auto tasks = oj::generate_tasks(desc);
    std::ranges::sort(tasks, {}, &oj::Task::launch_time);

    /* Check the tasks. */
    check_task(tasks, desc);

    /* Output the tasks to files. */
    serialize_tasks(tasks, "generated.tmp");
}

signed main() {
    work(oj::desciption);
    return 0;
}
