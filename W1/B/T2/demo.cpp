// 可以使用 database 里面暴露的接口
#include "database.h"
#include <iostream>

// 尝试登录某个用户的时候, 会调用这个函数
// 它会按照格式输出密码, 输出会被后台验证
void try_login(std::string name, std::string_view str) {
    std::cout << name << ' ';
    std::cout << str.length() << ' ';
    for (char c : str)
        std::cout << static_cast <unsigned> (c) << ' ';
    std::cout << '\n';
}

signed main() {
    // 样例代码
    try_login("DarkSharpness", "12345678");
    try_login("DarkSharpness", "87654321");
    // 需要同时登录成功全部的管理员
    try_login("admin", "admin");
    try_login("root", "root");
    return 0;
}
