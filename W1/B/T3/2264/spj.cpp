#include "hidden.h"
#include <iostream>
#include <fstream>
#include <cmath>

double test(std::istream &in, std::ostream &out) {
    switch (int x; (in >> x), x) {
        case hidden::Corrupted_handle:
            out << "Callback handle corrupted!\n";
            return 0.5;

        case hidden::Request_too_much:
            out << "Failure after " << hidden::trials << " trials!\n";
            return 0;

        case hidden::Request_success:
            if (std::size_t y; in >> y && y == hidden::checksum) {
                out << "Success!\n";
                return 1;
            } else {
                out << "Checksum mismatch! Corrupted program!\n";
                return 0;
            }

        default:
            out << "Corrupted output!\n";
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
