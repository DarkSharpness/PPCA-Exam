#include "src.hpp"
#include "utility.h"

template <int arrSize>
void test_func() {
    array <int, arrSize> a; // An array which takes no space
    static_assert(arrSize > 0 && sizeof(a) <= 1);
}

template <int arrSize>
void test_0() {
    if constexpr (arrSize > 0) {
        test_func <arrSize> ();
        test_0  <arrSize - 1>();
    }
}

void test_1() {
    array <int, 10> a;
    array <int, 10> b;
    array <int, 3> c;

    for (int i = 0; i < 10; i++)    a[i] = dark::rand();
    for (int i = 0; i < 10; i++)    b[i] = dark::rand();
    for (int i = 0; i < 3; i++)     c[i] = dark::rand();

    for (int i = 0; i < 3; i++) std::cout << c[i] << ' ';
    std::cout << '\n';
    for (int i = 0; i < 10; i++) std::cout << a[i] << ' ';
    std::cout << '\n';
    for (int i = 0; i < 10; i++) std::cout << b[i] << ' ';
    std::cout << '\n';
}

void test_2() {
    array <int, 10> a[1145];
    for (int i = 0; i < 1145; i++) {
        for (int j = 0; j < 10; j++) {
            a[i][j] = dark::rand();
        }
    }
    for (int i = 0; i < 1145; i++) {
        for (int j = 0; j < 10; j++) {
            std::cout << a[i][j] << ' ';
        }
        std::cout << '\n';
    }
}

signed main() {
    test_0 <10> ();
    test_1();
    test_2();
    test_2();
    test_2();
    dark::check_print(114514);
    return 0;
}
