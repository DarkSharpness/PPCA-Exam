#include "callback.h"
#include <iostream>

struct generator {
    std::size_t counter;
    static void fn(callback &f) {
        auto &args = f.get_args<generator>();
        std::cout << args.counter << std::endl;
        if (args.counter == 0) {
            f.destruct_args<generator>();
            f.reset();
        } else {
            args.counter -= 1;
        }
    }
};

auto demo() -> callback {
    callback cb;
    cb.reset(generator::fn);
    cb.init_args<generator>(5ull);
    return cb;
}

signed main() {
    auto cb = demo();
    while (cb) cb.call();
    return 0;
}
