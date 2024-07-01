# 整数回绕

输出一个整数, 使得下面一个程序能输出 Welcome.

```C++
#include <iostream>
#include <numeric>

signed main() {
    constexpr std::size_t threshold = std::numeric_limits<int>::max();

    int x;
    std::cin >> x;

    if (!std::cin) {
        std::cout << "wtf dude\n";
        return 0;
    }

    if (int abs = x < 0 ? -x : x; abs > threshold) {
        std::cout << "Welcome!\n";
    } else {
        std::cout << "You are not allowed to enter!\n";
    }
}
```
