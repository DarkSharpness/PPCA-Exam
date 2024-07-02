#pragma once
#include <new>
#include <utility>
#include <cstddef>

struct callback {
  private:
    static void dummy(callback &) {}
    using _Fn_t = decltype(dummy);
    std::byte args[48];
    _Fn_t *handle = dummy;

    // Aligned to fill up a cacheline of 64 bytes
    // This may slightly improve performance
    // See https://www.zhihu.com/question/525624303/answer/2431131742
    std::byte _[8]; // Useless padding.

  public:
    explicit callback(_Fn_t *f = dummy) : args(), handle(f), _() {}

    void call() { return this->handle(*this); }

    template <typename _Tp>
    _Tp &get_args() {
        return *std::launder(reinterpret_cast<_Tp *>(this->args));
    }

    template <typename _Tp>
    const _Tp &get_args() const {
        return *std::launder(reinterpret_cast<const _Tp *>(this->args));
    }

    template <typename _Tp, typename... _Args>
    _Tp &init_args(_Args &&...args) {
        auto *ptr = &this->get_args<_Tp>();
        return *new (ptr) _Tp{std::forward<_Args>(args)...};
    }

    template <typename _Tp>
    void destruct_args() {
        this->get_args<_Tp>().~_Tp();
    }

    void reset(_Fn_t *f = dummy) { this->handle = f; }

    _Fn_t *get_handle() const { return this->handle; }

    bool is_dummy() const { return this->handle == dummy; }

    operator bool() const { return !this->is_dummy(); }
};

static_assert(sizeof(callback) == 64, "callback size is not 64 bytes");
