#include<bits/stdc++.h>
using namespace std;
const int N=1e6+5;
int n,m,a[N],vis[N],cnt[N],flag,ans;char s[N];
vector<int> G[N],now;
void dfs(int u){
    vis[u]=1;now.push_back(u);
    if(u<=m){for(int v:G[u])if(vis[v]!=1)dfs(v);}
    else{
        int s=a[u-m],p=0;
        for(int v:G[u]){if(vis[v]!=1)p=v;else s+=cnt[v];}
        if(p)cnt[p]=(3-s%3)%3,dfs(p);
        else if(s%3!=0)flag=0;
    }
}
int main(){
    // freopen("data/5.in","r",stdin);
    scanf("%d%d",&n,&m);scanf("%s",s+1);
    for(int i=1;i<=n;i++)
        a[i]=(s[i]=='R'?0:(s[i]=='G'?1:2));
    for(int i=1,k;i<=m;i++){
        scanf("%d",&k);
        while(k--){
            int j;scanf("%d",&j);
            G[i].push_back(j+m);
            G[j+m].push_back(i);
        }
        // if(i % 1000 == 0 || i > 308000) {
        //     fprintf(stderr, "%d ", i);
        // }
    }
    for(int i=1;i<=n;i++)
        if(!G[i+m].size()&&a[i]!=0){printf("Sad Dark\n");return 0;}
    for(int i=1;i<=m;i++)if(!vis[i]){
        int mn=1e9;
        for(cnt[i]=0;cnt[i]<3;cnt[i]++){
            flag=1;dfs(i);int s=0;
            if(flag){for(int j:now)if(j<=m)s+=cnt[j];mn=min(mn,s);}
            for(int j:now)vis[j]=2;now.clear();
        }
        if(mn==1e9){printf("Sad Dark\n");return 0;}
        ans+=mn;
    }
    printf("%d\n",ans);
    return 0;
}