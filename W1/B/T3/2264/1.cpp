#include "src.hpp"
#include "verify.h"
#include <cstring>
#include <iostream>

signed main() {
    std::size_t counter = hidden::trials;
    callback request { ::request };
    callback verifier { ::verifier };
    int best = sizeof(buffer);
    while (counter --> 0 && request) {
        func(request, verifier);
        if (verifier.get_handle() != ::verifier) {
            std::cout << hidden::Corrupted_handle;
            return 0;
        } else if (int cnt = verifier.get_args<int>(); cnt == 0) {
            std::cout << hidden::Request_success;
            std::cout << '\n';
            std::cout << hidden::checksum;
            return 0;
        } else {
            best = std::min(best, cnt);
        }
    }
    std::cout << hidden::Request_too_much;
    std::cout << '\n';
    std::cout << best;
    return 0;
}
