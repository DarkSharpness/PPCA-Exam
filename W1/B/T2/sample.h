#include <iostream>
#include <unordered_map>
#include <any>

namespace database {

using _Map_t = std::unordered_map <std::string, unsigned>;

constexpr auto old_hash(std::string_view str) {
    unsigned hash = 0;
    for (char c : str) hash = hash * 31 + c;
    return hash;
}

/**
 * @brief Initialize the old mapping from 
 * a user name to the hashed password.
 * It will extract the mapping from some
 * encrypted inner format.
 */
_Map_t init_old() {
    _Map_t old;

    old["root"]     = old_hash("admin");
    old["Alice"]    = old_hash("dark");
    old["Bob"]      = old_hash("sharpness");

    return old;
}

/**
 * @brief Verify the user name and the password.
 * It will call init_old() to get the old mapping.
 * @param name The user name.
 * @param code The password.
 * @return Whether the user name and the password are correct.
 */
bool verify(std::string_view name, std::string_view code) {
    const static _Map_t old = ::database::init_old();

    auto it = old.find(std::string(name));
    if (it == old.end()) return false;

    return it->second == old_hash(code);
}

} // namespace database
