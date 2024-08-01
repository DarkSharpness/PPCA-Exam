#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>

std::string read_file(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    const auto file_size = std::filesystem::file_size(filename);
    std::string content = std::string(file_size, '\0');
    file.read(content.data(), file_size);
    return content;
}

void write_file(const std::string &filename, std::string_view content) {
    std::ofstream file(filename, std::ios::binary);
    file.write(content.data(), content.size());
}

signed main() {
    // cat interface.h with definition.h with runtime.h

    // replace #include "runtime.h" with this file
    // remove all #include "interface.h" and #include "definition"

    auto interface  = read_file("interface.h");
    auto definition = read_file("definition.h");
    auto runtime    = read_file("runtime.h");

    std::string result;
    result.reserve(interface.size() + definition.size() + runtime.size());

    result += interface;
    result += definition;
    result += runtime;

    // Remove #include "interface.h" and #include "definition.h"
    // From runtime.h
    [&result] {
        std::stringstream tmp_file;
        std::string line;
        std::stringstream ss(std::move(result));
        while (std::getline(ss, line)) {
            if (line.starts_with("#include \"interface.h\"")
            || line.starts_with("#include \"definition.h\"")
            || line.starts_with("#pragma once")) {
                continue;
            } else {
                tmp_file << line << '\n';
            }
        }
        result = std::move(tmp_file).str();
    }();

    std::filesystem::create_directories("merged");

    write_file("merged/runtime.h", result);

    auto replace_method = [&result](std::string name) {
        auto path = "merged/" + name;
        std::ifstream     old_file(name, std::ios::binary);
        std::ofstream     new_file(path, std::ios::binary);
        std::stringstream tmp_file;

        std::string line;
        while (std::getline(old_file, line)) {
            if (line.starts_with("#include \"runtime.h\"")) {
                tmp_file.write(result.data(), result.size());
                tmp_file << '\n';
            } else {
                tmp_file << line << '\n';
            }
        }

        while (std::getline(tmp_file, line)) {
            if (line.starts_with("#include \"interface.h\"")
            || line.starts_with("#include \"definition.h\"")
            || line.starts_with("#pragma once")) {
                continue;
            } else {
                new_file << line << '\n';
            }
        }
    };

    replace_method("src.hpp");
    replace_method("client.cpp");
    replace_method("server.cpp");

    std::filesystem::copy_file("client_spj.cpp", "merged/client_spj.cpp", std::filesystem::copy_options::overwrite_existing);
    std::filesystem::copy_file("server_spj.cpp", "merged/server_spj.cpp", std::filesystem::copy_options::overwrite_existing);

    return 0;
}