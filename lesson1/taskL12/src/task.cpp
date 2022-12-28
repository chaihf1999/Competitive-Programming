#include <bits/stdc++.h>
// convolution_mod_2_64_v2
using i64 = int64_t;
using u32 = uint32_t;
using u64 = uint64_t;
const int N = 1 << 20;
const int P0 = 377487361;
const int P1 = 595591169;
const int P2 = 645922817;
const int P3 = 897581057;
const int P4 = 998244353;
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
    int x0, x1, x2, x3, x4;
    constexpr mint() : x0(), x1(), x2(), x3(), x4() {}
    mint(u64 x) : x0(x % P0), x1(x % P1), x2(x % P2), x3(x % P3), x4(x % P4) {}
    mint(int x0, int x1, int x2, int x3, int x4) : x0(x0), x1(x1), x2(x2), x3(x3), x4(x4) {}
    mint operator+(mint o) const {
        return {x0 + o.x0 < P0 ? x0 + o.x0 : x0 + o.x0 - P0,
                x1 + o.x1 < P1 ? x1 + o.x1 : x1 + o.x1 - P1,
                x2 + o.x2 < P2 ? x2 + o.x2 : x2 + o.x2 - P2,
                x3 + o.x3 < P3 ? x3 + o.x3 : x3 + o.x3 - P3,
                x4 + o.x4 < P4 ? x4 + o.x4 : x4 + o.x4 - P4
        };
    }
    mint operator-(mint o) const {
        return {
                x0 - o.x0 < 0 ? x0 - o.x0 + P0 : x0 - o.x0,
                x1 - o.x1 < 0 ? x1 - o.x1 + P1 : x1 - o.x1,
                x2 - o.x2 < 0 ? x2 - o.x2 + P2 : x2 - o.x2,
                x3 - o.x3 < 0 ? x3 - o.x3 + P3 : x3 - o.x3,
                x4 - o.x4 < 0 ? x4 - o.x4 + P4 : x4 - o.x4
        };
    }
    mint operator*(mint o) const {
        return {
                int(u64(x0) * o.x0 % P0),
                int(u64(x1) * o.x1 % P1),
                int(u64(x2) * o.x2 % P2),
                int(u64(x3) * o.x3 % P3),
                int(u64(x4) * o.x4 % P4),
        };
    }
    void operator+=(mint o) { *this = *this + o; }
    void operator-=(mint o) { *this = *this - o; }
    void operator*=(mint o) { *this = *this * o; }
    u64 get() const {
        constexpr u64 P01 = pow<P1>(P0, P1 - 2);
        constexpr u64 P12 = pow<P2>(P1, P2 - 2);
        constexpr u64 P02 = pow<P2>(P0, P2 - 2) * P12 % P2;
        constexpr u64 P23 = pow<P3>(P2, P3 - 2);
        constexpr u64 P13 = pow<P3>(P1, P3 - 2) * P23 % P3;
        constexpr u64 P03 = pow<P3>(P0, P3 - 2) * P13 % P3;
        constexpr u64 P34 = pow<P4>(P3, P4 - 2);
        constexpr u64 P24 = pow<P4>(P2, P4 - 2) * P34 % P4;
        constexpr u64 P14 = pow<P4>(P1, P4 - 2) * P24 % P4;
        constexpr u64 P04 = pow<P4>(P0, P4 - 2) * P14 % P4;
        constexpr u64 M1 = P0;
        constexpr u64 M2 = M1 * P1;
        constexpr u64 M3 = M2 * P2;
        constexpr u64 M4 = M3 * P3;
        u64 a0 = x0;
        u64 a1 = (((u64) P1 << 32) + (x1 - a0) * P01) % P1;
        u64 a2 = (((u64) P2 << 32) + (x2 - a0) * P02 - a1 * P12) % P2;
        u64 a3 = (((u64) P3 << 32) + (x3 - a0) * P03 - a1 * P13 - a2 * P23) % P3;
        u64 a4 = (((u64) P4 << 32) + (x4 - a0) * P04 - a1 * P14 - a2 * P24 - a3 * P34) % P4;
        return a0 + a1 * M1 + a2 * M2 + a3 * M3 + a4 * M4;
    }
};
mint w[N];
void __attribute((constructor)) init() {
    w[N / 2] = 1;
    mint g = {
            pow<P0>(7, P0 / N),
            pow<P1>(3, P1 / N),
            pow<P2>(3, P2 / N),
            pow<P3>(3, P3 / N),
            pow<P4>(3, P4 / N),
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
            P3 - (P3 - 1) / n,
            P4 - (P4 - 1) / n,
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
        u64 x;
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
        u64 x{};
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
