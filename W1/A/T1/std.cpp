#include <queue>
#include <iostream>
#include <algorithm>

const int N = 1e5 + 3;
struct interval { int l, r; };

bool assert(bool condition, const char* message = "Assertion failed") {
    if (!condition) {
        std::cerr << message << '\n';
        exit(1);
    }
    return true;
}

interval x[N];
interval y[N];
interval z[N]; // Answer

struct cmp_right {
    bool operator() (int a, int b) const {
        return x[a].r > x[b].r;
    }
};

int sort_y(int m) {
    auto cmp = [](interval a, interval b) { return a.l < b.l; };
    std::sort(y, y + m, cmp);
    interval current = y[0];
    int cnt = 0;
    for (int i = 1; i < m; ++i) {
        if (current.r < y[i].l) {
            y[cnt++] = current;
            current = y[i];
        } else {
            current.r = std::max(current.r, y[i].r);
        }
    }
    y[cnt++] = current;
    return cnt;
}

bool solve(int n, int m) {
    for (int i = 0 ; i < n ; ++i)
        z[i].l = z[i].r = -1; // Do not cover any interval

    // Rank right from smallest to largest
    std::priority_queue<int, std::vector<int>, cmp_right> q;
    int i = 0;  // x[i], l from smallest to largest
    int j = 0;  // y[j], l from smallest to largest
    while (j < m) {
        while (i < n && x[i].l <= y[j].l)
            q.push(i++);

        // can not cover y[j]
        while (!q.empty() && x[q.top()].r < y[j].l)
            q.pop();

        // No solution
        if (q.empty()) return false;

        int k = q.top(); q.pop();
        assert(x[k].l <= y[j].l && x[k].r >= y[j].l);

        if (x[k].r >= y[j].r) {
            // y[j] is covered by x[k]
            z[k].l = y[j].l;
            z[k].r = y[j].r;
            ++j;
        } else {
            // y[j] is partially covered by x[k]
            z[k].l = y[j].l;
            z[k].r = x[k].r;
            y[j].l = x[k].r;
        }
    }
    return true;
}

int mapping[N];

void sort_x(int n) {
    struct pack {
        int pos;
        interval i;
    };
    static pack p[N];

    auto cmp = [](pack a, pack b) { return a.i.l < b.i.l; };
    for (int i = 0; i < n; ++i) {
        p[i].pos = i;
        p[i].i = x[i];
    }

    std::sort(p, p + n, cmp);

    for (int i = 0; i < n; ++i) {
        mapping[i] = p[i].pos;
        x[i] = p[i].i;
    }
}


signed main() {
    std::ios_base::sync_with_stdio(false);

    int n, m;
    std::cin >> n >> m;

    for (int i = 0; i < n; ++i)
        std::cin >> x[i].l >> x[i].r;

    for (int i = 0; i < m; ++i)
        std::cin >> y[i].l >> y[i].r;
 
    m = sort_y(m); // update m to new ranges

    sort_x(n);

    if (solve(n, m)) {
        std::cout << "Yes, your majesty\n";
        for (int i = 0; i < n; ++i) {
            int k = mapping[i];           
            std::cout << z[k].l << ' ' << z[k].r << '\n';
        }
    } else {
        std::cout << "No, darksharpness out\n";
    }

    for (int i = 0 ; i < n ; ++i)
        assert(z[i].l == -1 || (z[i].l >= x[i].l && z[i].r <= x[i].r));
    return 0;
}