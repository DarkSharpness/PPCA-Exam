// These 2 are visible to all.
#include "interface.h"
#include "definition.h"

// These 2 are your implementations.
#include "_scheduler.h"

// This is only for the judge.
#include "runtime.h"

static auto judge(
    const oj::Description &desc,
    const oj::ServiceInfo &std_info,
    std::vector <oj::Task> tasks) {

    using enum JudgeResult;
    try {
        auto info = schedule_work(desc, tasks);

    } catch (const oj::OJException &e) {
        handle_exception <ScheduleFailed> (e);
    }
}

signed main() {
    try {
        using namespace oj;
        auto count = oj::deserialize_main(std::cin);
        while (count--) {
            auto [header, tasks] = oj::deserialize(std::cin);
            judge(header.description, header.service_info, std::move(tasks));
        }
    } catch (const std::exception &e) {
        std::cerr << "System Error: Unexpected std::exception(): " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "System Error: An unknown error occurred!" << std::endl;
    }
    return 0;
}
