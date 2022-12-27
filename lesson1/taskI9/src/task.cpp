#include <bits/stdc++.h>
// gcd_convolution
using i64 = int64_t;
using u32 = uint32_t;
using u64 = uint64_t;
const int N = 1 << 20;
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
    mint pow(auto k) const {
        mint a = x;
        mint b = 1;
        for (; k > 0; k /= 2) {
            if (k & 1)
                b *= a;
            a *= a;
        }
        return b;
    }
};
mint mu[N];
void __attribute((constructor)) init() {
    std::bitset<N> vec;
    for (int i = 1; i < N; ++i) mu[i] = 1;
    for (u64 i = 2; i < N; ++i) {
        if (!vec[i]) {
            for (u64 j = i; j < N; j += i) mu[j] = -mu[j], vec.set(j);
            for (u64 j = i * i; j < N; j += i * i) mu[j] = 0;
        }
    }
}
void fft(mint f[], int n) {
    for (int i = 1; i <= n; ++i)
        for (int j = 2; i * j <= n; ++j) f[i] += f[i * j];
}
void ift(mint f[], int n) {
    for (int i = 1; i <= n; ++i)
        for (int j = 2; i * j <= n; ++j) f[i] += f[i * j] * mu[j];
}
mint a[N];
mint b[N];
int main() {
#ifdef LOCAL
    auto flush = [&]() {};
    auto ii = [&]() {
        int x;
        std::cin >> x;
        return x;
    };
    auto oo = [&](auto x, char c = 10) {
        std::cout << x << c << std::flush;
    };
#else
    char bufI[1 << 19], *ptrI = bufI, *endI = bufI + sizeof(bufI);
    char bufO[1 << 19], *ptrO = bufO, *endO = bufO + sizeof(bufO);
    fread(bufI, 1, sizeof(bufI), stdin);
    auto load = [&]() {
        memcpy(bufI, ptrI, endI - ptrI);
        fread(endI - ptrI + bufI, 1, ptrI - bufI, stdin);
        ptrI = bufI;
    };
    auto flush = [&]() {
        fwrite(bufO, 1, ptrO - bufO, stdout);
        ptrO = bufO;
    };
    auto ii = [&]() {
        if (endI - ptrI < 32) load();
        int x{};
        int n{};
        for (; *ptrI < 48; ++ptrI) n = *ptrI == 45;
        for (; *ptrI > 47; ++ptrI) x = x * 10 + *ptrI - 48;
        return n ? -x : +x;
    };
    auto oo = [&](auto x, char c = 10) {
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
#endif
    int n = ii();
    for (int i = 1; i <= n; ++i) a[i] = ii();
    for (int i = 1; i <= n; ++i) b[i] = ii();
    fft(a, n);
    fft(b, n);
    for (int i = 1; i <= n; ++i) a[i] *= b[i];
    ift(a, n);
    for (int i = 1; i <= n; ++i) oo(a[i].x);
    flush();
}
