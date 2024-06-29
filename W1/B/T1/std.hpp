#pragma once

#include <iostream>
#include <unordered_map>

template <typename T>
struct tester;

template <>
struct tester <int> {
    int x;
    tester (int y) : x (y) {}
    bool operator == (const tester <int> &) const = default;
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
struct fraction <int> {
    fraction() { std::exit(0); }
};

template <typename T, int N>
struct array {
  private:
    char token;
    inline static std::unordered_map <const void *, T *> mapping;

    T &get(int x) { return mapping[static_cast <const void *> (this)][x]; }

  public:

    array() {
        mapping[static_cast <const void *> (this)] = new T[N];
    }

    ~array() {
        delete[] mapping[static_cast <const void *> (this)];
        mapping.erase(static_cast <const void *> (this));
    }

    T &operator [] (int x) { return get(x); }
    const T &operator [] (int x) const { return get(x); }
};

char *my_string() { static char s[4] = {}; return s; }

template <typename T>
bool my_hash(T &&x) { return 0; }
