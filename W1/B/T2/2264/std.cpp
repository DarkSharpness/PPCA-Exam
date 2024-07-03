#include "database.h"
#include <iostream>
#include <ranges>
#include <algorithm>

void split(std::string_view str) {
    std::cout << str.length() << ' ';
    for (char c : str)
        std::cout << static_cast <unsigned> (c) << ' ';
    std::cout << '\n';
}

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
    for (auto [name, code] : database::init_map()) {
        std::cout << name << ' ';
        split(hack(code));
        std::cout << '\n';
    }
    return 0;
}

// signed main() {
//     std::cout << "DarkSharpness";
//     split("Emperor");
//     std::cout << "jpp";
//     split("中文总能过吧");
//     std::cout << "Wankupi";
//     split("🤣");
//     std::cout << "Lily_White";
//     split("さよなら");
//     return 0;
// }