#include <fstream>
#include <iostream>
#include <optional>
#include <vector>
#include <span>
#include <set>
#include <ranges>
#include <algorithm>

// Lines of integers.

using _Vec = std::vector <std::pair <int, int>>;

int extract_int(std::string_view &str) {
    while (str.size() && std::isspace(str[0]))
        str.remove_prefix(1);

    if (str.empty()) return 0;

    bool flag = false;
    if (str.starts_with('-')) {
        flag = true;
        str.remove_prefix(1);
    }

    int result = 0;
    while (str.size()) {
        if (!std::isdigit(str[0])) break;
        result = result * 10 + (str[0] - '0');
    }

    return flag ? -result : result;
}

std::optional <_Vec> read(std::istream& file) {
    _Vec result;
    std::string buffer;
    while (std::getline(file, buffer)) {
        std::string_view str = buffer;
        int a = extract_int(str);
        int b = extract_int(str);
        result.push_back({a, b});
        for (auto &c : str)
            if (!std::isspace(c)) return std::nullopt;
    }
    return result;
}

struct interval {
    int l, r;
    friend auto operator<=>(const interval&, const interval&) = default;
    friend bool operator==(const interval&, const interval&) = default;
};

std::vector <interval> x;
std::vector <interval> y;

auto merge(std::vector <interval> t) {
    std::vector <interval> z;
    interval current = t[0];

    for (std::size_t i = 1; i < t.size(); ++i) {
        if (current.r < t[i].l) {
            z.push_back(current);
            current = t[i];
        } else {
            current.r = std::max(current.r, t[i].r);
        }
    }

    z.push_back(current);
    return z;
}

void init_graph(_Vec vec) {
    if (vec.empty()) return;
    auto slice = std::span(vec);

    auto [n, m] = slice[0];
    slice = slice.subspan(1);

    if (slice.size() != n + m) throw std::runtime_error("Invalid input");

    x.reserve(n);
    for (auto [l, r] : slice.subspan(0, n)) x.push_back({l, r});

    y.reserve(m);
    for (auto [l, r] : slice.subspan(n, m)) y.push_back({l, r});

    y = merge(y);
}

void verify(_Vec vec) {
    std::vector <interval> t;

    if (vec.size() != x.size()) throw std::runtime_error("Invalid output");
    for (auto i : std::views::iota(0llu, vec.size())) {
        auto old = x[i];
        auto tmp = interval{ vec[i].first, vec[i].second };
        if (tmp.l < 0 || tmp.r < 0) continue; // Do not use this interval
        t.push_back(tmp);
    }

    t = merge(t);

    if (t != y) throw std::runtime_error("Invalid output");
}

signed main(int argc, char* argv[]) {
    std::ifstream input (argv[1]);
    std::ifstream output(argv[2]);
    std::ifstream answer(argv[3]);
    std::ofstream score(argv[4]);
    std::ofstream message(argv[5]);
    std::string str, tmp;

    try {
        init_graph(read(input).value());
        bool has_answer {};
        answer >> has_answer;
        if (!has_answer) {
            std::string tmp;
            std::getline(output, tmp);
            if (tmp.starts_with("No")) {
                score << 1;
            } else {
                score << 0;
            }
        } else {
            std::string tmp;
            std::getline(output, tmp);
            if (!tmp.starts_with("Yes")) {
                score << 0;
            } else {
                verify(read(output).value());
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
