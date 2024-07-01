#include "database.h"
#include <iostream>
#include <fstream>
#include <unordered_set>

double test(std::istream &output, std::ostream &message) {
    std::string name, code;
    std::size_t correct = 0;
    std::unordered_set <std::string> names;

    while (output >> name >> code) {
        if (!names.insert(name).second) {
            message << "Duplicate user name: " << name << '\n';
        } else if (!database::verify(name, code)) {
            message << "Invalid user name or password: " << name << ' ' << code << '\n';
        } else {
            message << "Successfully login: " << name << '\n';
            ++correct;
        }
    }

    return static_cast <double> (correct) / database::init_old().size();
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
