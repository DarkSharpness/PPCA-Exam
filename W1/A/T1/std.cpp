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

bool cmp(interval a, interval b) {
    return a.l < b.l;
}

struct cmp_right {
    bool operator() (int a, int b) const {
        return x[a].r > x[b].r;
    }
};

int sort_y(int m) {
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
    int i = 0;
    int j = 0;
    while (j < m) {
        while (i < n && x[i].l <= y[j].l)
            q.push(i++);

        while (114514) {
            if (q.empty()) return false; // No solution

            int k = q.top(); q.pop();

            // x[k].l <= y[j].l
            // x[k].r must be the smallest
            assert(x[k].l <= y[j].l);

            if (x[k].r >= y[j].r) {
                z[k] = y[j];
                ++j; break;
            }

            // y[k].l <= x[k].r < y[j].r
            if (x[k].r >= y[k].l) {
                y[j].l = x[k].r;
                z[k] = y[j];
                break;
            }

            // k is useless here
        }
    }
    return true;
}


signed main() {
    int n, m;
    std::ios_base::sync_with_stdio(false);

    std::cin >> n >> m;

    for (int i = 0; i < m; ++i)
        std::cin >> x[i].l >> x[i].r;

    for (int i = 0; i < m; ++i)
        std::cin >> y[i].l >> y[i].r;
 
    m = sort_y(m); // update m to new ranges

    std::sort(x, x + n, cmp);

    if (solve(n, m)) {
        std::cout << "Yes, your majesty\n";
        for (int i = 0; i < n; ++i)
            std::cout << z[i].l << ' ' << z[i].r << '\n';
    } else {
        std::cout << "No, darksharpness out\n";
    }

    return 0;
}