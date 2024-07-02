#include "src.hpp"
#include "utility.h"
#include <iostream>

template <typename _Tp>
requires requires(_Tp x) {  x != int(1); }
bool check_constant() {
    const int N = 1e9;
    for (int i = 0 ; i < N ; ++i) {
        _Tp x = i;
        if (x != i) return 0;
    }
    return 1;
}

signed main() {
    constant <int> x = 1;

    if (!(x == 1) || !check_constant <constant <int>> ()) {
        std::cout << "Wrong answer!\n";
    } else {
        std::cout << "Accepted!\n";
    }
    return 0;
}
