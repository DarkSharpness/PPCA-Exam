#pragma once
#include "interface.h"
#include "definition.h"

namespace oj {

/**
 * @brief Scheduler side.
 * You need to arrange dynamically generated tasks.
 * Your task is to attempt to improve the SLO rate.
 * That is, you need to optimize the scheduling of tasks.
 * @param time Current time.
 * @param list A list of tasks that need to be scheduled.
 * @param desc Description of the tasks. Same as the one used in generate_tasks.
 */
auto schedule_tasks(time_t time, std::vector <Task> list, const Description &desc) -> std::vector<Policy> {
    return {};
}

} // namespace oj
