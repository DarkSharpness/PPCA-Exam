#pragma once
#include "interface.h"

namespace oj {

struct Task {
    time_t      launch_time;
    time_t      deadline;
    time_t      execution_time;
    priority_t  priority;
};

template <typename _Tp>
struct Range {
    _Tp min;
    _Tp max;
};

struct Description {
    // Count of CPUs
    cpu_id_t    cpu_count;
    // Number of tasks that you should generate
    task_id_t   task_count;
    // Range of deadline time
    Range <time_t> deadline_time;
    // Range of a single execution time
    Range <time_t> execution_time_single;
    // Range of the sum of execution time
    Range <time_t> execution_time_sum;
    // Range of priority of one
    Range <priority_t> priority_single;
    // Range of priority of sum
    Range <priority_t> priority_sum;
};

struct Launch {
    cpu_id_t    cpu_cnt;
    task_id_t   task_id;
};

struct Saving {
    task_id_t   task_id;
};

} // namespace oj
