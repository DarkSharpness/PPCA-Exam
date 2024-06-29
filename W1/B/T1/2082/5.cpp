#include "src.hpp"
#include "utility.h"
#include <cstring>

bool my_hash(const char *x) {
    static bool seed = 0;
    seed = !seed;
    return seed;
}

signed main() {
    char *p1 = my_string(); char p2[] = "Dark"; if (std::strcmp(p1,p2) != 0 && my_hash(p1) == my_hash(p2)) {
        std::cout << "Correct!\n";
        dark::check_print(233);
    } else {
        std::cout << "Incorrect!\n";
    }
    return 0;
}
