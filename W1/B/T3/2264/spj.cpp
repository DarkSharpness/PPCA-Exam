#include "database.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>

double test(std::istream &output, std::ostream &message) {
    std::string name;
    std::size_t correct = 0;
    std::unordered_set <std::string> names;
    std::string code;
    std::size_t length;
    std::vector <std::uint32_t> array;

    while (output >> name >> length) {
        if (length > 100) {
            message << "Too many elements for " << name << ": " << length << '\n';
            continue;
        }
        array.resize(length);
        for (std::size_t i = 0; i < length; ++i)
            output >> array[i];
        code.resize(length);
        for (std::size_t i = 0; i < length; ++i)
            code[i] = array[i];
        if (!names.insert(name).second) {
            message << "Duplicate user name: " << name << '\n';
        } else if (!database::verify(name, code)) {
            message << "Invalid user name or password: " << name << ' ' << code << '\n';
        } else {
            message << "Successfully login: " << name << '\n';
            ++correct;
        }
    }

    return std::min(static_cast <double> (correct) / database::init_map().size(), 1.0);
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
