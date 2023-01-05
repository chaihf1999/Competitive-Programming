#include <bits/stdc++.h>
// static_range_sum
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
const int N = 1 << 19;
int val[N];
u64 pre[19][N];
u64 suf[19][N];
int main() {
    int n = io();
    int m = io();
    for (int i = 0; i < n; ++i) val[i] = io();
    for (int k = 0; n >> k; ++k) {
        int mask = (1 << k) - 1;
        for (int i = 0; i < n; ++i) pre[k][i] = (i & mask) ? pre[k][i - 1] + val[i] : val[i];
        for (int i = n; i > 0; --i) suf[k][i - 1] = (i & mask) ? suf[k][i] + val[i - 1] : val[i - 1];
    }
    for (int i = 0; i < m; ++i) {
        int l = io();
        int r = io() - 1;
        if (l == r) {
            io(val[l]);
        } else {
            int k = std::__lg(r ^ l);
            io(suf[k][l] + pre[k][r]);
        }
    }
}
