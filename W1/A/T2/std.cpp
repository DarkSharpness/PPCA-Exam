#include <bits/stdc++.h>
#define rep(i, l, r) for(int i = (l); i <= (r); ++i)
#define per(i, r, l) for(int i = (r); i >= (l); --i)
#define pb push_back
using namespace std;
struct edge{int from,to,cap,flow;};
const int N = 3e5 + 10, inf = 1e9;
struct Dinic{
	vector<edge> e;
	vector<int> G[N];
	int d[N],cur[N],n,m,s,t;
	bool vis[N];
	void clear(){
		rep(i,0,(int)e.size()-1) e[i].flow=0;
	}
	inline void add_edge(int from,int to,int cap){
		e.pb((edge){from,to,cap,0});
		e.pb((edge){to,from,0,0});
		m=e.size();
		G[from].pb(m-2);G[to].pb(m-1);
	}
	bool bfs(){
		memset(vis,0,sizeof(vis));memset(d,-1,sizeof(d));
		queue<int> q;q.push(s);vis[s]=1;d[s]=0;
		while(!q.empty()){
			int u=q.front();q.pop();
			rep(i,0,(int)G[u].size()-1){
				edge &E=e[G[u][i]];
				if(E.cap>E.flow&&!vis[E.to])
					vis[E.to]=1,d[E.to]=d[u]+1,q.push(E.to);
			}
		}
		return vis[t];
	}
	int dfs(int u,int a){
		if(u==t||a==0) return a;
		int flow=0,f;
		for(int &i=cur[u];i<G[u].size();++i){
			edge &E=e[G[u][i]];
			if(d[E.to]==d[u]+1&&(f=dfs(E.to,min(a,E.cap-E.flow)))){
				E.flow+=f;e[G[u][i]^1].flow-=f;
				a-=f;flow+=f;
				if(a==0) break;
			}
		}
		return flow;
	}
	int max_flow(int s,int t){
		this->s=s;this->t=t;
		int flow=0;
		while(bfs()){
			memset(cur,0,sizeof(cur));
			flow+=dfs(s,inf);
		}
		return flow;
	}
}A, B;
int n, m, a[N], b[N];
int main() {
    freopen("2260/1.in", "r", stdin);
    freopen("2260/1.out", "w", stdout);
    scanf("%d%d", &n, &m); 
    int S = n * 2 + 1, T = S + 1; A.n = B.n = T;
    rep(i, 1, n){
        A.add_edge(S, i, 2);
        A.add_edge(i + n, T, 2);
        B.add_edge(S, i, 1);
        B.add_edge(i + n, T, 1);
    }
    rep(i, 1, m) {
        int u, v;
        scanf("%d%d",&u, &v);
        A.add_edge(u, v + n, 1);
    }
    int x = A.max_flow(S, T);
    if(x < 2 * n) {
        puts("No");
        return 0;
    } else {
        puts("Yes");
    }
    rep(i, 1, n) {
        for(int &x: A.G[i]) {
            edge e = A.e[x];
            if(e.flow == 1 && e.to <= S) {
                B.add_edge(e.from, e.to, 1);
            }
        }
    }
    int y = B.max_flow(S, T);
    rep(i, 1, n) {
        for(int &x: B.G[i]) {
            edge e = B.e[x];
            if(e.flow == 1 && e.to <= S) {
                a[i] = e.to;
            } else if(e.to <= S){
                b[i] = e.to;
            }
        }
    }
    rep(i, 1, n) {
        printf("%d %d\n", i, a[i] - n);
    }
    rep(i, 1, n) {
        printf("%d %d\n", i, b[i] - n);
    }
    return 0;
}