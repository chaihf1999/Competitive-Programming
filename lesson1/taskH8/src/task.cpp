#include <bits/stdc++.h>
// range_affine_range_sum
#define mid ((l + r) >> 1)
#define L (k << 1 | 0)
#define R (k << 1 | 1)
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
const int N = 1 << 21;
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
mint a[N];
mint b[N];
mint sum[N];
void build(int k, int l, int r) {
    if (l == r - 1) {
        sum[k] = io();
    } else {
        a[k] = 1;
        build(L, l, mid);
        build(R, mid, r);
        sum[k] = sum[L] + sum[R];
    }
}
void update(int k, int l, int r, mint A, mint B) {
    a[k] = A * a[k];
    b[k] = A * b[k] + B;
    sum[k] = A * sum[k] + B * (r - l);
}
void pushDown(int k, int l, int r) {
    update(L, l, mid, a[k], b[k]);
    update(R, mid, r, a[k], b[k]);
    a[k] = 1;
    b[k] = 0;
}
void update(int k, int l, int r, int t, int v, mint A, mint B) {
    if (v <= l || r <= t) return;
    if (t <= l && r <= v) {
        update(k, l, r, A, B);
        return;
    }
    pushDown(k, l, r);
    update(L, l, mid, t, v, A, B);
    update(R, mid, r, t, v, A, B);
    sum[k] = sum[L] + sum[R];
}
mint query(int k, int l, int r, int t, int v) {
    if (v <= l || r <= t) return 0;
    if (t <= l && r <= v) return sum[k];
    pushDown(k, l, r);
    mint x = query(L, l, mid, t, v);
    mint y = query(R, mid, r, t, v);
    return x + y;
}
int main() {
    int n = io();
    int m = io();
    build(1, 0, n);
    for (int i = 0; i < m; ++i) {
        if (io() == 0) {
            int l = io();
            int r = io();
            mint A = io();
            mint B = io();
            update(1, 0, n, l, r, A, B);
        } else {
            int l = io();
            int r = io();
            mint ans = query(1, 0, n, l, r);
            io(ans.x);
        }
    }
}
