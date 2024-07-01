#include <bits/stdc++.h>
#define rep(i, l, r) for(int i = (l); i <= (r); ++i)
#define per(i, r, l) for(int i = (r); i >= (l); --i)
using namespace std;
const int N = 2e5+10;
mt19937 rng(time(0));
struct seg{
    int l, r;
}a[N], b[N];
seg gen_seg(int l, int r){
    uniform_int_distribution<int> gen(l, r);
    int x = gen(rng), y = gen(rng);
    if(x > y) swap(x, y);
    if(x == y && y < r) ++y;
    else if(x == y) --x;
    // printf("%d %d %d %d\n", l, r, x, y);
    return (seg){x, y};
}
int main() {
    int n = 90000;
    
    uniform_int_distribution<int> gen(1, 15000);
    rep(i, 1, n){
        int xx = i * 2000, l = xx + gen(rng), r = xx + gen(rng);
        if(l > r) 
            std::swap(l, r);
        if(l + 2 >= r) 
            r += 4;
        assert(l < r);
        a[i] = (seg){l, r};
        b[i] = gen_seg(l, r);
        assert(b[i].l < b[i].r);
        assert(b[i].l>=a[i].l&&b[i].r<=a[i].r);
    }
    int pp = 5e8, qq = 9e8;
    a[++n] = (seg){pp + 1, pp + 10}; b[n] = (seg){pp + 2, pp + 3};
    a[++n] = (seg){pp + 6, pp + 9};  b[n] = (seg){pp + 7, pp + 8};
    a[++n] = (seg){qq + 1, qq + 10}; b[n] = (seg){qq + 3, qq + 4};
    a[++n] = (seg){qq + 2, qq + 5};  b[n] = (seg){qq + 6, qq + 9};
    shuffle(a + 1, a + n + 1, rng);
    shuffle(b + 1, b + n + 1, rng);
    printf("%d %d\n", n, n);
    rep(i, 1, n){
        printf("%d %d%c",a[i].l, a[i].r, " \n"[i == n]);
    }
    rep(i, 1, n){
        printf("%d %d%c",b[i].l, b[i].r, " \n"[i == n]);
    }
    return 0;
}
