#include "src.hpp"
#include "hidden.h"
#include "verify.h"
#include <cstring>
#include <iostream>

signed main() {
    std::size_t counter = hidden::trials;
    callback request = get_request();
    callback verifier = get_verifier();
    const auto handle = verifier.get_handle();
    while (counter --> 0 && request) {
        bool result = verify(request, verifier);
        if (verifier.get_handle() != handle) {
            std::cout << hidden::Corrupted_handle;
            return 0;
        } else if (result) {
            std::cout << hidden::Request_success;
            std::cout << '\n';
            std::cout << hidden::checksum;
            return 0;
        }
    }
    std::cout << hidden::Request_too_much;
    return 0;
}
