#include <bits/stdc++.h>
// vertex_add_subtree_sum
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
int val[N];
int str[N];
int end[N];
u64 sum[N];
int head[N];
int next[N];
void dfs(int u) {
    static int cnt;
    str[u] = ++cnt;
    sum[cnt] = val[u];
    for (int v = head[u]; v > 0; v = next[v])
        dfs(v);
    end[u] = cnt + 1;
}
int main() {
    int n = io();
    int m = io();
    for (int i = 0; i < n; ++i) val[i] = io();
    for (int i = 1; i < n; ++i) {
        int f = io();
        next[i] = head[f];
        head[f] = i;
    }
    dfs(0);
    for (int i = 1; i <= n; ++i) sum[i] += sum[i - 1];
    for (int i = n; i >= 1; --i) sum[i] -= sum[i & (i - 1)];
    for (int i = 0; i < m; ++i) {
        if (io() == 0) {
            int u = io();
            int x = io();
            for (int k = str[u]; k <= n; k += k & -k) sum[k] += x;
        } else {
            int u = io();
            u64 ans = 0;
            for (int k = str[u] - 1; k > 0; k -= k & -k) ans -= sum[k];
            for (int k = end[u] - 1; k > 0; k -= k & -k) ans += sum[k];
            io(ans);
        }
    }
}
