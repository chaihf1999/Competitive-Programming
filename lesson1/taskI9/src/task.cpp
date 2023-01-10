#include <bits/stdc++.h>
// vertex_add_path_sum
using i64 = int64_t;
using u32 = uint32_t;
using u64 = uint64_t;
#ifdef LOCAL
class IO {
public:
    inline auto operator()() {
        int x;
        std::cin >> x;
        return x;
    }
    inline void operator()(auto x, char c = ' ') {
        std::cout << x << c << std::flush;
    }
} io;
#else
class IO {
private:
    char bufI[1 << 19], *ptrI = bufI, *endI = bufI + sizeof(bufI);
    char bufO[1 << 19], *ptrO = bufO, *endO = bufO + sizeof(bufO);
public:
    inline void load() {
        memcpy(bufI, ptrI, endI - ptrI);
        fread(endI - ptrI + bufI, 1, ptrI - bufI, stdin);
        ptrI = bufI;
    };
    inline void flush() {
        fwrite(bufO, 1, ptrO - bufO, stdout);
        ptrO = bufO;
    };
    inline auto operator()() {
        if (endI - ptrI < 32) load();
        int x{};
        int n{};
        for (; *ptrI < 48; ++ptrI) n = *ptrI == 45;
        for (; *ptrI > 47; ++ptrI) x = x * 10 + *ptrI - 48;
        return n ? -x : +x;
    };
    inline void operator()(auto x, char c = ' ') {
        if (endO - ptrO < 32) flush();
        if (x < 0) x = -x, *ptrO++ = '-';
        static char buf[21];
        char *end = buf + 21;
        char *ptr = buf + 21;
        *--ptr = c;
        for (; x >= 10; x /= 10)
            *--ptr = char(48 + x % 10);
        *--ptr = char(48 + x);
        memcpy(ptrO, ptr, end - ptr);
        ptrO += end - ptr;
    };
    IO() { fread(bufI, 1, sizeof(bufI), stdin); }
    ~IO() { flush(); }
} io;
#endif
const int N = 500100;
int fa[N];
int val[N];
int idx[N];
int str[N];
int end[N];
u64 sum[N];
int min[20][N];
std::vector<int> ch[N];
void dfs(int u, int f) {
    static int cnt;
    ++cnt;
    fa[u] = f;
    str[u] = cnt;
    idx[cnt] = u;
    min[0][cnt] = str[f];
    sum[cnt] = sum[str[f]] + val[u];
    for (int v: ch[u])
        if (v != f)
            dfs(v, u);
    end[u] = cnt + 1;
}
int lca(int u, int v) {
    if (u == v) return u;
    int l = std::min(str[u], str[v]);
    int r = std::max(str[u], str[v]);
    int k = std::__lg(r - l);
    int m = std::min(min[k][l + (1 << k)], min[k][r]);
    return idx[m];
}
u64 query(int p) {
    u64 ans = 0;
    for (int k = str[p]; k > 0; k -= k & -k)
        ans += sum[k];
    return ans;
}
int main() {
    int n = io();
    int m = io();
    for (int i = 1; i <= n; ++i) val[i] = io();
    for (int i = 2; i <= n; ++i) {
        int u = io() + 1;
        int v = io() + 1;
        ch[u].push_back(v);
        ch[v].push_back(u);
    }
    dfs(1, 0);
    for (int i = n; i >= 1; --i) sum[i] -= sum[i & (i - 1)];
    for (int k = 1; n >> k; ++k)
        for (int i = 1 << k, j = 1 << (k - 1); i <= n; ++i, ++j)
            min[k][i] = std::min(min[k - 1][i], min[k - 1][j]);
    for (int i = 1; i <= m; ++i) {
        if (io() == 0) {
            int p = io() + 1;
            int x = io();
            for (int k = str[p]; k <= n; k += k & -k) sum[k] += x;
            for (int k = end[p]; k <= n; k += k & -k) sum[k] -= x;
        } else {
            int u = io() + 1;
            int v = io() + 1;
            int w = lca(u, v);
            u64 ans = query(u) + query(v) - query(w) - query(fa[w]);
            io(ans);
        }
    }
}
