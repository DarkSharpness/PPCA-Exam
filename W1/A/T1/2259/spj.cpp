#include <fstream>
#include <iostream>
#include <optional>
#include <vector>
#include <span>
#include <set>
#include <ranges>
#include <algorithm>

// Lines of integers.

struct interval {
    int l; int r;
    friend bool operator == (const interval &lhs, const interval &rhs) = default;
    friend auto operator <=> (const interval &lhs, const interval &rhs) = default;
};

using _Vec = std::vector <interval>;

_Vec read(std::istream &input) {
    int x, y;
    _Vec res;
    while (input >> x >> y)
        res.push_back({x, y});
    return res;
}

_Vec x;
_Vec y;

void init_graph(_Vec t) {
    auto [n, m] = t.at(0);
    x.clear(); y.clear();
    if (n + m + 1 != t.size())
        throw std::runtime_error("Invalid input");
    for (int i = 1; i <= n; ++i)
        x.push_back(t.at(i));
    for (int i = n + 1; i <= n + m; ++i)
        y.push_back(t.at(i));
}

_Vec merge(_Vec t) {
    std::ranges::sort(t);
    auto tmp = t.front();
    _Vec z;
    for (std::size_t i = 1; i < t.size(); ++i) {
        auto [a, b] = t.at(i);
        if (a <= tmp.r) { // Can merge
            tmp.r = std::max(tmp.r, b);
        } else { // Else, push back and update tmp
            z.push_back(tmp);
            tmp = t.at(i);
        }
    }
    z.push_back(tmp);
    return z;
}

void verify(_Vec t) {
    if (t.size() != x.size())
        throw std::runtime_error("Mismatched output length!");

    _Vec z;
    for (std::size_t i = 0 ; i < t.size(); ++i) {
        auto [a, b] = x.at(i);
        auto [c, d] = t.at(i);
        if (c == -1 && d == -1) continue;
        if (c < a || d > b || c > d) {
            std::cerr << a << ' ' << b << ' ' << c << ' ' << d << '\n';
            throw std::runtime_error("Invalid interval");
        }
        z.push_back({c, d});
    }

    if (merge(z) != merge(y))
        throw std::runtime_error("Mismatched intervals");
}

signed main(int argc, char* argv[]) {
    std::ifstream input (argv[1]);
    std::ifstream output(argv[2]);
    std::ifstream answer(argv[3]);
    std::ofstream score(argv[4]);
    std::ofstream message(argv[5]);
    std::string str, tmp;

    try {
        init_graph(read(input));
        bool has_answer {};
        std::string tmp;
        std::getline(output, tmp);
        answer >> has_answer;
        if (!has_answer) {
            if (tmp.starts_with("No")) {
                score << 1;
            } else {
                score << 0;
            }
        } else {
            if (!tmp.starts_with("Yes")) {
                score << 0;
            } else {
                verify(read(output));
                score << 1;
            }
        }
    } catch(std::exception &e) {
        message << e.what();
        score << 0;
        return 0;
    } catch(...) {
        message << "wtf is this?";
        score << 0;
        return 0;
    }
}
