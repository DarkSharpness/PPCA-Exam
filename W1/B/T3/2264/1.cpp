#include "callback.h"
#include "hidden.h"
#include "src.hpp"
#include <cstring>
#include <iostream>

struct buffer {
    char storage[48];
    buffer(const buffer &src) { std::strcpy(storage, src.storage); }
};

void func(callback &read, callback &verifier) {
    read.call();
    verifier.init_args <buffer> (read.get_args<buffer>());
    return verifier.call();
}

void verifier(callback &f) {
    auto &args = f.get_args<buffer>();
    bool result = hidden::authenticate(args.storage);
    f.destruct_args<buffer>();
    f.init_args<bool>(result);
}

signed main() {
    std::size_t counter = hidden::trials;
    callback request { ::request };
    while (counter --> 0 && request) {
        callback verifier { ::verifier };
        func(request, verifier);
        if (verifier.get_handle() != ::verifier) {
            std::cout << hidden::Corrupted_handle;
            return 0;
        } else if (verifier.get_args<bool>()) {
            std::cout << hidden::Request_success;
            std::cout << '\n';
            std::cout << hidden::checksum;
            return 0;
        }
    }
    std::cout << hidden::Request_too_much;
    return 0;
}
