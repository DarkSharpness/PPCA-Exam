#pragma once

namespace hidden {

int cmp(const char *str) {
    static constexpr char target[48] =
        "destroy everthing 压力测试 😅 🤣　";

    int cnt = 0;

    for (int i = 0 ; i < 48 ; ++i)
        if (str[i] != target[i]) ++cnt;

    return cnt;
}

enum Output {
    Corrupted_handle,
    Request_too_much,
    Request_success,
};

static constexpr unsigned long long checksum = 0x1145141919810;

static constexpr unsigned long long trials = 10000;

} // namespace hidden
