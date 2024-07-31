// These 2 are visible to all.
#include "interface.h"
#include "definition.h"

// These 2 are your implementations.
#include "_scheduler.h"

// This is only for the judge.
#include "runtime.h"

namespace oj::detail::runtime {

static auto grading_policy(priority_t std_ans, priority_t usr_ans) -> double {
    // Mapping [0, infty] -> [0, 1]

    if (std_ans == 0) return 1;

    double ratio = double(usr_ans) / std_ans;

    if (ratio >= 2) return 1;

    return ratio / 2;
}

static auto judge(
    const Description &desc,
    const ServiceInfo &std_info,
    std::vector <Task> tasks) {

    using enum JudgeResult;
    try {
        auto info = schedule_work(desc, tasks);

        if (info.total != std_info.total)
            panic <SystemException> ("Total service priority mismatch!");

        const auto std_ans = std_info.complete;
        const auto usr_ans = info.complete;

        std::cout << grading_policy(std_ans, usr_ans) << std::endl;
    } catch (const OJException &e) {
        handle_exception <ScheduleFailed> (e);
    }
}

} // namespace oj::detail::runtime

signed main() {
    try {
        using oj::detail::runtime::judge;
        using oj::detail::runtime::deserialize;
        using oj::detail::runtime::deserialize_main;
        auto count = deserialize_main(std::cin);
        while (count--) {
            auto [header, tasks] = deserialize(std::cin);
            judge(header.description, header.service_info, std::move(tasks));
        }
    } catch (const std::exception &e) {
        std::cerr << "System Error: Unexpected std::exception(): " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "System Error: An unknown error occurred!" << std::endl;
    }
    return 0;
}
