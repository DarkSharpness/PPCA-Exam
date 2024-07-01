#include <bits/stdc++.h>
#define rep(i, l, r) for(int i = (l); i <= (r); ++i)
#define per(i, r, l) for(int i = (r); i >= (l); --i)
using namespace std;
mt19937 rng(time(0));
int main() {
    int n = 10000, m = 20000;
    uniform_int_distribution<int> len1(1, 10000);
    uniform_int_distribution<int> left_pos(1, 100000000);
    printf("%d %d\n", n, m);
    for(int i = 1; i <= n; ++i) {
        int len = len1(rng), l = left_pos(rng), r = l + len;
        printf("%d %d ", l, r);
    }
    puts("");
    for(int i = 1; i <= m; ++i) {
        int len = len1(rng), l = left_pos(rng), r = l + len;
        printf("%d %d\n", l, r);
    }
    return 0;
}