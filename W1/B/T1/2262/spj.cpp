#pragma GCC optimize("O0") // Must disable optimization
#include <iostream>
#include <numeric>
#include <fstream>

int test(std::istream &in, std::ostream &out) {
    constexpr std::size_t threshold = std::numeric_limits<int>::max();

    int x;
    in >> x;

    if (!in) {
        out << "wtf dude\n";
        return 0;
    }

    if (std::abs(x) > threshold) {
        out << "Welcome!\n";
        return 1;
    } else {
        out << "You are not allowed to enter!\n";
        return 0;
    }
}

signed main(int argc, char *argv[]) {
    if (argc == 1) { // Local test
        auto &output    = std::cin;
        auto &message   = std::cerr;
        auto &score     = std::cout;
        score << test(output, message);
    } else {
        // std::ifstream input (argv[1]);
        std::ifstream output(argv[2]);
        // std::ifstream answer(argv[3]);
        std::ofstream score(argv[4]);
        std::ofstream message(argv[5]);
        score << test(output, message);
    }
    return 0;
}
