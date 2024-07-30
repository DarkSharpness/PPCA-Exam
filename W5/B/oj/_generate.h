#pragma once
#include "interface.h"
#include "definition.h"
#include <random>

namespace oj {

/**
 * @brief Generator side.
 * You need to generate a list of tasks based on the description.
 * Your task is to attempt to lower that SLO rate.
 * That is, you need to paralyze the scheduling of tasks.
 * @param desc Description of the tasks. 
 * @return A list of tasks, which must satisfy the description.
 */
auto generate_tasks(const Description &desc) -> std::vector <Task> {
    const auto min_duration = 1 + desc.execution_time_sum.min / desc.task_count;
    const auto min_priority = 1 + desc.priority_sum.min / desc.task_count;

    const Task template_task = {
        .launch_time    = 1,
        .deadline       = 2 + 2 * min_duration / desc.cpu_count,
        .execution_time = 1 + min_duration,
        .priority       = 1 + min_priority
    };

    auto tasks = std::vector <Task>(desc.task_count, template_task);

    std::mt19937 gen(1919810);
    constexpr std::size_t kMagic = 8;

    for (auto &task : tasks) {
        task.deadline += gen() % kMagic;
        task.deadline = std::max(task.deadline, task.launch_time + 1);
        auto rest = desc.deadline_time.max - task.deadline;
        if (rest > 0) {
            const auto offset = gen() % rest;
            task.launch_time    += offset;
            task.deadline       += offset;
        }
        task.execution_time += gen() % kMagic;
    }

    return tasks;
}

} // namespace oj
