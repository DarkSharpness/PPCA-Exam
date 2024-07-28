#pragma once
#include <interface.h>

namespace oj {

struct Task {
    time_t      launch_time;
    time_t      deadline;
    time_diff_t execution_time;
    priority_t  priority;
};

struct Description {
    // Count of CPUs
    cpu_id_t    cpu_count;
    // Number of tasks that you should generate
    task_id_t   task_count;
    // All deadline should not exceed this value
    // All launch time should not exceed this value
    time_t      max_deadline_time;
    // Sum of the execution time should not exceed this value
    time_t      max_execution_sum;
    // Maximal of the execution time should not exceed this value
    time_t      max_execution_time;
    // Sum of the priority should not exceed this value
    priority_t  max_priority_sum;
    // Maximal of the priority should not exceed this value
    priority_t  max_priority_time;
};

class TaskList {
public:
    constexpr TaskList() noexcept : first(0), finish(0), tasks(nullptr) {}
    constexpr TaskList(task_id_t first, task_id_t finish, const Task *tasks)
        noexcept : first(first), finish(finish), tasks(tasks) {}
    using iterator = const Task *;
    auto begin() const -> iterator {
        return this->tasks + this->first;
    }
    auto end() const -> iterator {
        return this->tasks + this->finish;
    }
    auto size() const -> task_id_t {
        return this->finish - this->first;
    }
    auto get_first_id() const -> task_id_t {
        return this->first;
    }
private:
    task_id_t first;
    task_id_t finish;
    const Task *tasks;
};

} // namespace oj