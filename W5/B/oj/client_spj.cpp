#include "runtime.h"
#include <iostream>
#include <fstream>

static void check_error(std::istream &output, std::ostream &score, std::ostream &message) {
    auto err = oj::detail::runtime::deserialize_error(output);
    if (err.empty()) {
        score   << 1;
        message << "Accepted";
    } else {
        score   << 0;
        message << err;
    }
}

signed main(int argc, char *argv[]) {
    if (argc != 6) {
        std::cerr << "Usage: " << argv[0] << " <input> <output> <answer> <score> <message>\n";
        return 1;
    } else {
        std::ifstream input(argv[1]);
        std::ifstream output(argv[2]);
        std::ifstream answer(argv[3]);
        std::ofstream score(argv[4]);
        std::ofstream message(argv[5]);
        check_error(output, score, message);
        return 0;
    }
}
