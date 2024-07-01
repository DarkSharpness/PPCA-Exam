#include <string>
#include <string_view>
#include <unordered_map>

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
_Map_t init_old();

/**
 * @brief Verify the user name and the password.
 * It will call init_old() to get the old mapping.
 * @param name The user name.
 * @param code The password.
 * @return Whether the user name and the password are correct.
 */
bool verify(std::string_view, std::string_view);

} // namespace database
