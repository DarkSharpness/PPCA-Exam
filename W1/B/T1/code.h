#include <iostream>
#include <numeric>

// Core function to test the loyalty!
// Written by DarkSharpness
void test() {
    constexpr std::size_t threshold = std::numeric_limits<int>::max();

    int x;
    std::cin >> x;

    if (!std::cin) {
        std::cout << "wtf dude\n";
        return;
    }

    // Absolute loyalty!
    if (auto y = std::abs(x); y > threshold) {
        std::cout << "Welcome!\n";
    } else {
        std::cout << "You are not allowed to enter!\n";
    }
}
