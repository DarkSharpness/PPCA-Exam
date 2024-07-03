#pragma once
#include "callback.h"
#include <cstring>

struct buffer {
    char storage[48];
    /** Simply copy the buffer  */
    buffer(const buffer &src) { std::strcpy(storage, src.storage); }
};

/** Implementation of verifier. */
void verifier(callback &f) {
    auto args = f.get_args<buffer>();

    // if (std::strcmp(args.storage, "admin") {
    //     f.init_args(true);
    //     return;
    // }

    f.init_args(false);
}

/** Construct a verifer  */
callback get_verifier() {
    return callback { ::verifier };
}

/* Tries to verify a request. Return true on success. */
bool verify(callback &request, callback &verifier) {
    // Call the request to get the arguments.
    request.call();
    // Initialize the verifier with the arguments.
    verifier.init_args(request.get_args<buffer>());
    // Call the verifier to verify the arguments.
    verifier.call();
    // Return the result.
    return verifier.get_args<bool>();
}
