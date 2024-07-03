#pragma once
#include "callback.h"
#include <array>

/**
 * Each time this function is called, it will return a different character.
 */
void request(callback &f) {
    static char which = 'a';
    std::array <char, 24> c;
    c.fill(which++);
    // Use null terminator.
    c.back() = '\0';
    f.init_args(c);
}

/**
 * You need to implement this function.
 * We will call request to try requests.
 */
callback get_request() { return callback { ::request }; }
