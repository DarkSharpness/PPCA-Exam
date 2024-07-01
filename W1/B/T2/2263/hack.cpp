#include "database.h"
#include <iostream>
#include <ranges>
#include <algorithm>

std::string_view hack(std::size_t code) {
    static char c[20];
    int count = 0;

    while (code != 0) {
        c[count++] = code % 31;
        code /= 31;
    }

    std::ranges::reverse(c, c + count);

    return std::string_view(c, count);
}

signed main() {
    for (auto [name, code] : database::init_old()) {
        std::cout << name << ' ' << code << ' ';
        std::cout << database::old_hash(hack(code)) << '\n';
    }
    return 0;
}
