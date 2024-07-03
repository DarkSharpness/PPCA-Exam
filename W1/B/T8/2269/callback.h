#pragma once
#include <new>
#include <utility>
#include <cstddef>

struct callback {
  private:
    // This function will do nothing.
    static void dummy(callback &) {}

    using _Fn_t = decltype(dummy);
    std::byte args[48];
    _Fn_t *handle = dummy;

    std::byte _[8]; // Useless padding.

  public:
    explicit callback(_Fn_t *f = dummy) : args(), handle(f), _() {}

    void call() { return this->handle(*this); }

    /**
     * @return Returns a reference to the arguments of the callback.
     */
    template <typename _Tp>
    _Tp &get_args() {
        return *reinterpret_cast<_Tp *>(this->args);
    }

    /**
     * @return Returns a reference to the arguments of the callback.
     */
    template <typename _Tp>
    const _Tp &get_args() const {
        return *reinterpret_cast<const _Tp *>(this->args);
    }

    /**
     * @brief Construct a new object of type _Tp in the arguments of the callback.
     * @return Returns a reference to the newly constructed object.
     */
    template <typename _Tp>
    _Tp &init_args(const _Tp &args) {
        auto *ptr = &this->get_args<_Tp>();
        return *new (ptr) _Tp{ args };
    }

    /**
     * @brief Destroy the object of type _Tp in the arguments of the callback.
     * @tparam _Tp The type of the object to destroy.
     */
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
