// These 2 are visible to all.
#include "interface.h"
#include "definition.h"

// These 2 are your implementations.
#include "_generator.h"
#include "_scheduler.h"

// This is only for the judge.
#include "runtime.h"

namespace oj::detail::runtime {

static auto judge(const Description &desc) {
    using enum JudgeResult;
    std::vector <Task> tasks;

    try {
        tasks = generate_work(desc);
    } catch (const OJException &e) {
        handle_exception <GenerateFailed> (e);
    }

    try {
        auto info = schedule_work(desc, tasks);

        std::cerr << "Complete rate: "
            << std::setprecision(2)
            << std::fixed
            << 100 * double(info.complete) / info.total
            << "% ("
            << info.complete
            << "/"
            << info.total
            << ")"
            << std::endl;

        serialize(std::cout, tasks, desc, info);
    } catch (const OJException &e) {
        handle_exception <ScheduleFailed> (e);
    }
}

} // namespace oj::detail::runtime

signed main() {
    try {
        using oj::detail::runtime::serialize;
        using oj::detail::runtime::judge;
        constexpr oj::Description array[] = {
            oj::small,
            oj::middle,
            oj::senpai,
            oj::huge,
        };
        serialize(std::cout, { .description_count = std::size(array) });
        for (const auto &desc : array)
            judge(desc);
    } catch (const std::exception &e) {
        std::cerr << "System Error: Unexpected std::exception(): " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "System Error: An unknown error occurred!" << std::endl;
    }
    return 0;
}
