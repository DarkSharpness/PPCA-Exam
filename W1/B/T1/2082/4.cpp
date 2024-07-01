#include "src.hpp"
#include "utility.h"

template <int arrSize>
void test_func() {
    array <int, arrSize> a; // An array ? right ?
    static_assert(arrSize > 0 && sizeof(a) <= 1);
}

template <int arrSize>
void test_all() {
    if constexpr (arrSize > 0) {
        test_func <arrSize> ();
        test_all  <arrSize - 1>();
    }
}

signed main() {
    test_all <10> ();
    array <int, 10> a;
    array <int, 10> b;
    array <int, 3> c;

    for (int i = 0; i < 10; i++) a[i] = dark::rand();
    for (int i = 0; i < 10; i++) b[i] = dark::rand();
    for (int i = 0; i < 3; i++) c[i] = dark::rand();

    for (int i = 0; i < 3; i++) std::cout << c[i] << ' ';
    std::cout << '\n';
    for (int i = 0; i < 10; i++) std::cout << a[i] << ' ';
    std::cout << '\n';
    for (int i = 0; i < 10; i++) std::cout << b[i] << ' ';
    std::cout << '\n';

    dark::check_print(114514);
    return 0;
}
