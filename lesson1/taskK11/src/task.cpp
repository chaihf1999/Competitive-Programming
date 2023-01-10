#include <bits/stdc++.h>
// vertex_set_path_composite
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
const int N = 400100;
const int P = 998244353;
struct mint {
    int x;
    constexpr mint(int x = 0) : x(x) {}
    mint operator+(mint o) const { return x + o.x < P ? x + o.x : x + o.x - P; }
    mint operator-(mint o) const { return x - o.x < 0 ? x - o.x + P : x - o.x; }
    mint operator*(mint o) const { return int(u64(x) * o.x % P); }
    mint operator-() const { return x ? P - x : 0; }
    void operator+=(mint o) { *this = *this + o; }
    void operator-=(mint o) { *this = *this - o; }
    void operator*=(mint o) { *this = *this * o; }
};
struct Node {
    mint a, b;
    Node operator+(Node o) const { return {o.a * a, o.a * b + o.b}; }
};
int to[N];
int head[N];
int next[N];
Node val[N];
Node sum1[N];
Node sum2[N];
int fa[N];
int sz[N];
int dep[N];
int son[N];
int idx[N];
int top[N];
void dfs1(int u, int f) {
    fa[u] = f;
    sz[u] = 1;
    dep[u] = dep[f] + 1;
    for (int e = head[u], v = to[e]; e > 0; v = to[e = next[e]])
        if (v != f) {
            dfs1(v, u);
            sz[u] += sz[v];
            if (sz[v] > sz[son[u]])
                son[u] = v;
        }
}
void dfs2(int u, int w) {
    static int cnt;
    idx[u] = cnt++;
    top[u] = w;
    if (son[u])
        dfs2(son[u], w);
    for (int e = head[u], v = to[e]; e > 0; v = to[e = next[e]])
        if (v != fa[u] && v != son[u])
            dfs2(v, v);
}
inline Node query1(int l, int r) {
    Node L{1, 0};
    Node R{1, 0};
    for (--l, ++r; l != r - 1; l /= 2, r /= 2) {
        if (~l & 1) L = L + sum1[l ^ 1];
        if (+r & 1) R = sum1[r ^ 1] + R;
    }
    return L + R;
}
inline Node query2(int l, int r) {
    Node L{1, 0};
    Node R{1, 0};
    for (--l, ++r; l != r - 1; l /= 2, r /= 2) {
        if (~l & 1) R = sum2[l ^ 1] + R;
        if (+r & 1) L = L + sum2[r ^ 1];
    }
    return L + R;
}
int main() {
    int n = io();
    int m = io();
    for (int i = 1; i <= n; ++i) val[i] = {io(), io()};
    for (int i = 2; i <= n; ++i) {
        int u = io() + 1;
        int v = io() + 1;
        to[i + i - 1] = v, next[i + i - 1] = head[u], head[u] = i + i - 1;
        to[i + i - 2] = u, next[i + i - 2] = head[v], head[v] = i + i - 2;
    }
    dfs1(1, 0);
    dfs2(1, 1);
    for (int i = 1; i <= n; ++i) sum1[n + idx[i]] = sum2[n + idx[i]] = val[i];
    for (int i = n - 1; i > 0; --i) sum1[i] = sum1[i * 2] + sum1[i * 2 + 1];
    for (int i = n - 1; i > 0; --i) sum2[i] = sum2[i * 2 + 1] + sum2[i * 2];
    for (int i = 1; i <= m; ++i) {
        if (io() == 0) {
            int k = n + idx[io() + 1];
            sum1[k] = sum2[k] = {io(), io()};
            for (k /= 2; k > 0; k /= 2) {
                sum1[k] = sum1[k * 2] + sum1[k * 2 + 1];
                sum2[k] = sum2[k * 2 + 1] + sum2[k * 2];
            }
        } else {
            int u = io() + 1;
            int v = io() + 1;
            Node ansL = {0, io()};
            Node ansR = {1, 0};
            while (top[u] != top[v])
                if (dep[top[u]] > dep[top[v]]) {
                    ansL = ansL + query2(n + idx[top[u]], n + idx[u]);
                    u = fa[top[u]];
                } else {
                    ansR = query1(n + idx[top[v]], n + idx[v]) + ansR;
                    v = fa[top[v]];
                }
            if (dep[u] > dep[v])
                ansL = ansL + query2(n + idx[v], n + idx[u]) + ansR;
            else
                ansL = ansL + query1(n + idx[u], n + idx[v]) + ansR;
            io(ansL.b.x);
        }
    }
}
