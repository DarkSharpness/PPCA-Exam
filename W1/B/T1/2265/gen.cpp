#include <thread>
#include <vector>
#include <format>
#include <iostream>
#include <filesystem>

void compile(int i) {
    system (std::format("g++ -std=c++20 -O2 {}.cpp -D _LOCAL -o {}", i, i).c_str());
}

void run(int i) {
    system (std::format("{}.exe > {}.out", i, i).c_str());
}

void clean(int i) {
    std::filesystem::remove(std::format("{}.exe", i));
}

signed main() {
    std::vector <std::thread> pool;

    for (int i = 1 ; i <= 5 ; ++i) pool.emplace_back(compile, i);
    for (auto &&th : pool) th.join();

    pool.clear();

    for (int i = 1 ; i <= 5 ; ++i) pool.emplace_back(run, i);
    for (auto &&th : pool) th.join();

    for (int i = 1 ; i <= 5 ; ++i) clean(i);

    std::cerr << "Done\n";
    return 0;
}