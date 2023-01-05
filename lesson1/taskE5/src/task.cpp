#include <bits/stdc++.h>
// static_rmq
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
const int K = 5;
const int N = 1 << 19;
const int M = 1 << 14;
const int mask = (1 << K) - 1;
int val[N];
int pre[N];
int suf[N];
int min[14][M];
int main() {
    int n = io();
    int m = io();
    int o = n >> K;
    for (int i = 0; i < n; ++i) val[i] = io();
    for (int i = 0; i < n; ++i) pre[i] = (i & mask) ? std::min(pre[i - 1], val[i]) : val[i];
    for (int i = n; i > 0; --i) suf[i - 1] = (i & mask) ? std::min(suf[i], val[i - 1]) : val[i - 1];
    for (int i = 0; i < o; ++i) min[0][i] = suf[i << K];
    for (int k = 1; o >> k; ++k)
        for (int i = (1 << k) - 1, j = (1 << (k - 1)) - 1; i < o; ++i, ++j)
            min[k][i] = std::min(min[k - 1][i], min[k - 1][j]);
    for (int i = 0; i < m; ++i) {
        int l = io();
        int r = io() - 1;
        if (r - l < 64) {
            io(*std::min_element(val + l, val + r + 1));
        } else {
            int L = (l >> K) + 1;
            int R = (r >> K) - 1;
            int k = std::__lg(R - L + 1);
            int ans = std::min(std::min(suf[l], pre[r]), std::min(min[k][L + (1 << k) - 1], min[k][R]));
            io(ans);
        }
    }
}
