#pragma once
#include "interface.h"
#include "definition.h"
#include <array>
#include <vector>
#include <variant>

namespace oj {

struct RuntimeManager {
    static constexpr time_t   kMaxTime  = 1e8;
    static constexpr cpu_id_t kCPUCount = 514;

    static auto get_instance() -> RuntimeManager& {
        static RuntimeManager instance;
        return instance;
    }

    struct TaskStatus {
        bool launched   = false;    // Launched (only allow once)
        bool terminated = false;    // Cancelled
        time_t time_sum = 0;        // Total time
    };

    struct TaskSaved {
        task_id_t id;       // Task ID
        time_t pass_time;   // Time spent on the task
        time_t rest_time;   // Time before the task is completed
    };

    struct TaskFree {};

    struct TaskWorking {
        task_id_t id;       // Task ID
        time_t finish;      // End time
        time_t start;       // Start time
    };
    struct TaskSaving {
        task_id_t id;       // Task ID
        time_t finish;      // End time
        time_t pass_time;   // Time spent on the task
        time_t rest_time;   // Time before the task is completed
    };
    struct TaskMoving {
        task_id_t id;       // Task ID
        time_t finish;      // End time
        time_t pass_time;   // Time spent on the task
        time_t rest_time;   // Time before the task is completed
        task_id_t target;   // Target CPU
    };

    using WorkLoad = std::variant <TaskFree, TaskWorking, TaskSaving, TaskMoving>;

    struct CPUStatus {
        WorkLoad workload;  // Working tasks
        std::vector <TaskSaved> saved;  // Saved tasks
    };

    time_t   global_clock;   // A global clock to record the current time
    TaskList task_list;      // A list of tasks

    std::vector <TaskStatus>    task_state; // A list of task status
    std::vector <CPUStatus>     cpu_state;  // A list of CPU status
};


} // namespace oj
