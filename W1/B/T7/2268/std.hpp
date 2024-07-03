#pragma once

#include <array>
#include <iostream>
#include <unordered_map>

template <typename T>
struct constant;

template <>
struct constant <int> {
    int x;
    constant (int y) : x (y) {}
    bool operator == (const constant <int> &) const = default;
};

template <typename T>
struct fraction;

using ll = long long;

template <>
struct fraction <ll> {
    ll x , y;
    static ll gcd(ll a, ll b) {
        while (b) {
            a %= b;
            std::swap(a, b);
        } return a;
    }

    fraction (ll _x = 0 , ll _y = 1) : x (_x) , y (_y) {
        if (y == 0) throw;
        if (x < 0) x = -x , y = -y;
        ll g = gcd (x , y);
        x /= g , y /= g;
    }

    fraction operator + (const fraction &rhs) const {
        return fraction (x * rhs.y + y * rhs.x , y * rhs.y);
    }
    fraction operator += (const fraction &rhs) {
        return *this = *this + rhs;
    }
    friend std::ostream &operator << (std::ostream &os, fraction <ll> f) {
        return os << f.x << '/' << f.y;
    }
};

template <>
struct fraction <void> {
    fraction() { std::exit(0); }
};

template <typename T, int N>
struct array {
  private:
    using storage_t = std::array <T, N>;
    char token;
    inline static std::unordered_map <const void *, storage_t> mapping {};

    T &get(int x) {
        return mapping[static_cast <const void *> (this)][x];
    }

  public:
    array() {
        mapping[static_cast <const void *> (this)] = {};
    }

    ~array() {
        mapping.erase(static_cast <const void *> (this));
    }

    T &operator [] (int x) { return this->get(x); }
    const T &operator [] (int x) const { return this->get(x); }
};

char *my_string() { static char s[4] = {}; return s; }

template <typename T>
bool my_hash(T &&x) { return 0; }
