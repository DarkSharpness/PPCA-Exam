#pragma once

namespace hidden {

bool authenticate(const char *) { return false; }

enum Output {
    Corrupted_handle,
    Request_too_much,
    Request_success,
};

static constexpr unsigned long long checksum = 0x1145141919810;

static constexpr unsigned long long trials = 10000;

} // namespace hidden
