#include <bits/stdc++.h>
// convolution_mod_1000000007_v2
using i64 = int64_t;
using u32 = uint32_t;
using u64 = uint64_t;
const int N = 1 << 20;
const int P = 1e9 + 7;
const int P0 = 595591169;
const int P1 = 645922817;
const int P2 = 897581057;
template<int Mod>
constexpr int pow(u64 a, int k) {
    u64 b = 1;
    for (; k > 0; k /= 2) {
        if (k & 1)
            b = a * b % Mod;
        a = a * a % Mod;
    }
    return int(b);
}
struct mint {
    int x0, x1, x2;
    constexpr mint() : x0(), x1(), x2() {}
    mint(int x) : x0(x % P0), x1(x % P1), x2(x % P2) {}
    mint(int x0, int x1, int x2) : x0(x0), x1(x1), x2(x2) {}
    mint operator+(mint o) const {
        return {x0 + o.x0 < P0 ? x0 + o.x0 : x0 + o.x0 - P0,
                x1 + o.x1 < P1 ? x1 + o.x1 : x1 + o.x1 - P1,
                x2 + o.x2 < P2 ? x2 + o.x2 : x2 + o.x2 - P2
        };
    }
    mint operator-(mint o) const {
        return {
                x0 - o.x0 < 0 ? x0 - o.x0 + P0 : x0 - o.x0,
                x1 - o.x1 < 0 ? x1 - o.x1 + P1 : x1 - o.x1,
                x2 - o.x2 < 0 ? x2 - o.x2 + P2 : x2 - o.x2
        };
    }
    mint operator*(mint o) const {
        return {
                int(u64(x0) * o.x0 % P0),
                int(u64(x1) * o.x1 % P1),
                int(u64(x2) * o.x2 % P2),
        };
    }
    void operator+=(mint o) { *this = *this + o; }
    void operator-=(mint o) { *this = *this - o; }
    void operator*=(mint o) { *this = *this * o; }
    int get() const {
        constexpr u64 P01 = pow<P1>(P0, P1 - 2);
        constexpr u64 P12 = pow<P2>(P1, P2 - 2);
        constexpr u64 P02 = pow<P2>(P0, P2 - 2) * P12 % P2;
        constexpr u64 M1 = P0 % P;
        constexpr u64 M2 = M1 * P1 % P;
        u64 a0 = x0;
        u64 a1 = ((P1 - a0 + x1) * P01) % P1;
        u64 a2 = ((P2 - a0 + x2) * P02 + (P2 - a1) * P12) % P2;
        return int((a0 + a1 * M1 + a2 * M2) % P);
    }
};
mint w[N];
void __attribute((constructor)) init() {
    w[N / 2] = 1;
    mint g = {
            pow<P0>(3, P0 / N),
            pow<P1>(3, P1 / N),
            pow<P2>(3, P2 / N),
    };
    for (int i = N / 2 + 1; i < N; ++i) w[i] = w[i - 1] * g;
    for (int i = N / 2 - 1; i > 0; --i) w[i] = w[i * 2];
}
void fft(mint f[], int n) {
    for (int k = n / 2; k; k /= 2)
        for (int i = 0; i < n; i += k + k)
            for (int j = 0; j < k; ++j) {
                mint x = f[i + j];
                mint y = f[i + j + k];
                f[i + j] = x + y;
                f[i + j + k] = (x - y) * w[j + k];
            }
}
void ift(mint f[], int n) {
    for (int k = 1; k < n; k *= 2)
        for (int i = 0; i < n; i += k + k)
            for (int j = 0; j < k; ++j) {
                mint x = f[i + j];
                mint y = f[i + j + k] * w[j + k];
                f[i + j] = x + y;
                f[i + j + k] = x - y;
            }
    mint inv = {
            P0 - (P0 - 1) / n,
            P1 - (P1 - 1) / n,
            P2 - (P2 - 1) / n,
    };
    std::reverse(f + 1, f + n);
    for (int i = 0; i < n; ++i) f[i] *= inv;
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
    int m = ii();
    int o = 2 << std::__lg(n + m - 1);
    for (int i = 0; i < n; ++i) a[i] = ii();
    for (int i = 0; i < m; ++i) b[i] = ii();
    fft(a, o);
    fft(b, o);
    for (int i = 0; i < o; ++i) a[i] *= b[i];
    ift(a, o);
    for (int i = 0; i < n + m - 1; ++i) oo(a[i].get());
    flush();
}
