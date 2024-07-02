#pragma once
#include "callback.h"
#include "hidden.h"
#include <cstring>

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
    // Count of mismatched characters
    int mismatches = hidden::cmp(args.storage);
    f.destruct_args<buffer>();
    f.init_args<int>(mismatches);
}
