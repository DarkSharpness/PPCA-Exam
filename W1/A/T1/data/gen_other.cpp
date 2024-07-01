#include <bits/stdc++.h>
#define rep(i, l, r) for(int i = (l); i <= (r); ++i)
#define per(i, r, l) for(int i = (r); i >= (l); --i)
using namespace std;
const int N = 1e6+10;
// int ttt = 1719749159;
mt19937 rng(time(0));
struct seg{
    int l, r;
}s[N], t[N];
int id[N];
set<pair<int, int> > cur_seg;
int main() {
    // fprintf(stderr, "now seed: %d\n",ttt);
    vector<seg> A;
    int n = 99000, m = 90000, dd = 1500, kk = rng() % 10, tm = m - dd;
    uniform_int_distribution<int> len1(1, 20000), len2(1, 15000);
    int x = 1;
    rep(i, 1, tm) {
        int l = x, r = x + len1(rng);
        s[i] = t[i] = (seg){l, r};
        x = r + len2(rng);
    }
    rep(i, 1, dd) {
        uniform_int_distribution<int> pos(1, tm + i - 1);
        int p = pos(rng);
        uniform_int_distribution<int> gen_ss(t[p].l, t[p].r);
        int l = gen_ss(rng), r = gen_ss(rng);
        if(l > r) swap(l, r);
        t[i + m - dd] = (seg){l, t[p].r};
        t[p].r = r; 
    }
    shuffle(t + 1, t + m + 1, rng);
    rep(i, 1, n) id[i] = i;
    shuffle(id + 1, id + n + 1, rng);
    sort(id + 1, id + tm + 1);
    int j = 1, xx = 1, yy = 1, zz = 1;
    rep(i, 1, id[tm]) {
        int key = rng();
        // fprintf(stderr,"now: %d, %d %d %d s[i].r: %d\n",i,xx,yy,zz,s[i].r);
        uniform_int_distribution<int> gen_l(xx, yy);
        cur_seg.insert(make_pair(key, gen_l(rng)));
        int r;
        if(i == id[j]) {
            r = s[j].r;
            if(rng() % 3 == 0) {
                xx = zz;
            }
            zz = s[j + 1].l;
            yy = s[j + 1].l;
            ++j;
        } else {
            uniform_int_distribution<int> gen_r(zz, s[j].r);
            r = gen_r(rng);
            if(rng() % 3 == 0) {
                xx = zz;
            }
            zz = r;
            yy = r;
        }
        int l = cur_seg.begin() -> second;
        cur_seg.erase(cur_seg.begin());
        A.push_back((seg){l, r});
    }
    rep(i, 1, kk + n - id[tm]) {
        uniform_int_distribution<int> left_pos(1, 900000000);
        int l = left_pos(rng), r = l + len1(rng);
        A.push_back((seg){l, r});
    }
    shuffle(A.begin(), A.end(), rng);
    printf("%d %d\n",(int)A.size() + 4, m + 4);
    for(auto &[l, r]: A) {
        if(l == r) ++r;
        printf("%d %d ", l, r);
        assert(l < r);
    }
    // puts("");
    int t1 = 950000000, t2 = 970000000;
    printf("%d %d %d %d %d %d %d %d\n", t1 + 1, t1 + 10, t1 + 6, t1 + 9, t2 + 1, t2 + 10, t2 + 2, t2 + 5);
    rep(i, 1, m) {
        printf("%d %d ", t[i].l, t[i].r);
        assert(t[i].l < t[i].r);
    }
    printf("%d %d %d %d %d %d %d %d\n", t1 + 2, t1 + 3, t1 + 7, t1 + 8, t2 + 3, t2 + 4, t2 + 6, t2 + 9);
}