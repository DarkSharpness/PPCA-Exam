#pragma once
#include <vector>
#include <cstdint>
#include <variant>

namespace oj {

using time_t        = std::size_t;
using priority_t    = std::size_t;
using task_id_t     = std::size_t;
using cpu_id_t      = std::size_t;

struct Task;
struct Description;
struct TaskList;
struct Launch;
struct Saving;

using Policy = std::variant<Launch, Saving>;

/**
 * @brief Generator side.
 * You need to generate a list of tasks based on the description.
 * Your task is to attempt to lower that SLO rate.
 * That is, you need to paralyze the scheduling of tasks.
 * @param desc Description of the tasks. 
 * @return A list of tasks, which must satisfy the description.
 */
auto generate_tasks(const Description &desc) -> std::vector <Task>;

/**
 * @brief Scheduler side.
 * You need to arrange dynamically generated tasks.
 * Your task is to attempt to improve the SLO rate.
 * That is, you need to optimize the scheduling of tasks.
 * @param time Current time.
 * @param list A list of tasks that need to be scheduled.
 */
auto schedule_tasks(time_t time, TaskList list) -> std::vector<Policy>;

} // namespace oj
