#include <bits/stdc++.h>
// point_set_range_composite
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
    mint a;
    mint b;
    Node operator+(Node o) const { return {o.a * a, o.a * b + o.b}; }
};
Node tree[N + N];
int main() {
    int n = io();
    int m = io();
    for (int i = 0; i < n; ++i) tree[n + i] = {io(), io()};
    for (int i = n - 1; i; --i) tree[i] = tree[i * 2] + tree[i * 2 + 1];
    for (int i = 0; i < m; ++i) {
        if (io() == 0) {
            int k = n + io();
            tree[k] = {io(), io()};
            for (k /= 2; k > 0; k /= 2) tree[k] = tree[k * 2] + tree[k * 2 + 1];
        } else {
            int l = n + io() - 1;
            int r = n + io();
            Node ansL = {0, io()};
            Node ansR = {1, 0};
            for (; l != r - 1; l /= 2, r /= 2) {
                if (~l & 1) ansL = ansL + tree[l ^ 1];
                if (+r & 1) ansR = tree[r ^ 1] + ansR;
            }
            io((ansL + ansR).b.x);
        }
    }
}
