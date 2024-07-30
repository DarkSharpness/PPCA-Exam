#include "interface.h"
#include "definition.h"
#include "runtime.h"
#include "src.hpp"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <filesystem>

static void panic(std::string_view message) {
    std::cerr << message << std::endl;
    std::exit(1);
}

static auto deserialize_tasks(std::string_view filename) -> std::vector<oj::Task> {
    static_assert(std::is_trivial_v <oj::Task>, "Can not serialize the non-trivial type.");

    const auto file_size = std::filesystem::file_size(filename);
    const auto task_size = file_size / sizeof(oj::Task);

    std::ifstream input { std::string(filename), std::ios::binary };

    if (!input.is_open())
        panic("Failed to open file");

    std::vector<oj::Task> tasks(task_size);

    auto *buffer = reinterpret_cast<char *>(tasks.data());
    input.read(buffer, file_size);

    if (input.gcount() != file_size)
        panic("Failed to read file");

    return tasks;
}

static void work(const oj::Description &desc) {
    oj::RuntimeManager manager { deserialize_tasks("tasks.bin") };
    for (std::size_t i = 0; i <= desc.deadline_time.max; ++i) {
        auto new_tasks = manager.synchronize();
        if (i != manager.get_time())
            panic("Time is not synchronized");
        manager.work(oj::schedule_tasks(i, std::move(new_tasks), desc));
    }
}

signed main() {
    work(oj::desciption);
    return 0;
}
