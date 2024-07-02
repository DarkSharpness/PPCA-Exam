#pragma once
#include "callback.h"
#include "hidden.h"
#include <cstring>

struct buffer {
    char storage[48];
    buffer(const buffer &src) { std::strcpy(storage, src.storage); }
};

int verify(callback &request, callback &verifier) {
    request.call();
    verifier.init_args <buffer> (request.get_args<buffer>());
    verifier.call();
    return verifier.get_args<int>();
}

// A possible implementation of the verifier
void verifier(callback &f) {
    auto &args = f.get_args<buffer>();
    // Count of mismatched characters
    int mismatches = hidden::cmp(args.storage);
    f.destruct_args<buffer>();
    f.init_args<int>(mismatches);
}
