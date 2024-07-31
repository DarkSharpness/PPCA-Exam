#pragma once
#include "interface.h"
#include <cmath>

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

struct PublicInformation {
    static constexpr time_t   kMaxTime  = 1e8;
    static constexpr cpu_id_t kCPUCount = 114;
    static constexpr time_t   kStartUp  = 2;
    static constexpr time_t   kSaving   = 2;
};

/**
 * @brief Suppose a task first launched at x, and start saving at y,
 * then the duration should be y - x.
 * (Note that the saving is completed at y + kSaving)
 * 
 * If duration < kStartUp, the task can't be saved,
 * and we should not count the contribution.
 * 
 * Otherwise, we count the contribution by the multiplication
 * of effective core count and the effective duration.
 * 
 * @return The contribution (time) of the task within duration.
 */
inline auto time_policy(time_t duration, cpu_id_t cpu_cnt) -> double {
    if (duration < PublicInformation::kStartUp) return 0;
    const auto effective_core = std::pow(cpu_cnt, 0.75);
    const auto effective_time = duration - PublicInformation::kStartUp;
    return effective_core * effective_time;
}

} // namespace oj
