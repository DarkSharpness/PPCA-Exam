#include <string>
#include <string_view>
#include <unordered_map>
#include <thread>

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
    _Map_t mapping;
    mapping["DarkSharpness"] = old_hash("Emperor");
    mapping["jpp"] = old_hash("中文总能过吧");
    mapping["Wankupi"] = old_hash("🤣");
    mapping["Lily_White"] = old_hash("さよなら");
    return mapping;
}

/**
 * @brief Verify the user name and the password.
 * It will call init_old() to get the old mapping.
 * @param name The user name.
 * @param code The password.
 * @return Whether the user name and the password are correct.
 */
bool verify(std::string_view name, std::string_view code) {
    // To make this function more secure, we
    // will make this process more complex and slower.
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(10ms);

    // To anti hack, we init every time.
    auto mapping = init_old();
    return mapping[std::string(name)] == old_hash(code);
}

} // namespace database
