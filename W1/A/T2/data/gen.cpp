#include <iostream>
#include <fstream>
#include <ranges>
#include <random>
#include <vector>
#include <set>

struct worker {
    std::set <std::pair<int, int>> edges;
    std::mt19937 device;
    std::uniform_int_distribution<std::size_t> gen;
    const int n;

    worker(int n) : edges(), device(114514), gen(), n(n) {}

    bool add_edge(int u, int v) {
        return edges.insert({u, v}).second;
    }

    bool select() {
        auto [s, t] = [n = this->n]() {
            auto s = std::vector<int>();
            s.reserve(n);

            for (int i : std::views::iota(1, n + 1))
                s.push_back(i);
        
            auto t = s;
            return std::make_pair(std::move(s), std::move(t));   
        }();

        bool result = true;
        for (int i = 0; i < n; i++) {
            const std::size_t size = n - i;
            int l = gen(device) % size;
            int r = gen(device) % size;
            int x = s[l];
            int y = t[r];
            s[l] = s.back();
            t[r] = t.back();
            s.pop_back();
            t.pop_back();
            result &= add_edge(x, y);
        }

        return result;
    }

    void complete() {
        std::vector <int> s (n + 1, 0), t (n + 1, 0);

        for (auto [k, v] : this->edges) {
            s.at(k) += 1;
            t.at(v) += 1;
        }

        std::set <int> s1, t1;
        for (int i = 1; i <= n; i++) {
            if (s.at(i) < 1 || t.at(i) < 1)
                throw std::runtime_error("Invalid graph");
            if (s[i] == 1) s1.insert(i);
            if (t[i] == 1) t1.insert(i);
        }

        for (auto s : s1)
            add_edge(s, gen(device) % n + 1);

        for (auto t : t1)
            add_edge(gen(device) % n + 1, t);
    }
};


signed main() {
    const int num[10] = {
        5, 10, 100, 1000, 5000,
        6000, 7000, 8000, 9000, 10000
    };

    for (int i = 1; i <= 10; i++) {
        std::ofstream input(std::to_string(i) += ".in");
        std::ofstream output(std::to_string(i) += ".out");

        const int n = num[i - 1];
        worker w {n};

        w.select();
        w.select();
        w.complete();

        const int m = w.edges.size();

        input << n << " " << m << '\n';

        for (auto [fr, to] : w.edges)
            input << fr << " " << to << '\n';
    }
    return 0;
}