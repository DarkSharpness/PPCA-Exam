#pragma once
#include <vector>
#include <cstdint>

namespace oj {

using time_t        = std::size_t;
using time_diff_t   = std::size_t;
using priority_t    = std::size_t;
using task_id_t     = std::size_t;
using cpu_id_t      = std::size_t;

struct Task;
struct Description;
struct TaskList;

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
void schedule_tasks(time_t time, TaskList list);

/**
 * @brief Launch a task now.
 * @param id Task ID.
 * @param task A list of pairs of CPU ID and execution time,
 * which describes the execution of the task.
 * @return Whether the task is successfully launched.
 * @note A task will be completed only when all the CPUs have finished executing.
 */
bool launch_task(task_id_t id, const std::vector<std::pair<cpu_id_t, time_t>> &task);

/**
 * @brief Cancel a task.
 * @param id Task ID.
 * @return Whether the task is successfully canceled.
 */
bool cancel_task(task_id_t id);

/**
 * @brief Move a task from one CPU to another.
 * @param id Task ID.
 * @param from The CPU ID where the task is currently running.
 * @param to The CPU ID where the task will be moved to.
 * @return Whether the task is successfully moved.
 */
bool move_task(task_id_t id, cpu_id_t from, cpu_id_t to);

/**
 * @brief Save the context of a task into disk.
 * It can be recovered later by load_task.
 * @param id Task ID.
 * @param cpu The CPU ID where the task is currently running.
 * @return Whether the task is successfully saved.
 */
bool save_task(task_id_t id, cpu_id_t cpu);

/**
 * @brief Recover the context of a task from disk.
 * @param id Task ID.
 * @param cpu The CPU ID where the task will be recovered.
 * @return Whether the task is successfully recovered.
 */
bool load_task(task_id_t id, cpu_id_t cpu);

} // namespace oj
