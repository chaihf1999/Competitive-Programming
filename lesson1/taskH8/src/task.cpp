#include <bits/stdc++.h>
// multivariate_convolution
using i64 = int64_t;
using u32 = uint32_t;
using u64 = uint64_t;
const int N = 1 << 19;
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
mint w[N];
void __attribute((constructor)) init() {
    w[N / 2] = 1;
    mint g = mint(3).pow(P / N);
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
    mint inv = P - (P - 1) / n;
    std::reverse(f + 1, f + n);
    for (int i = 0; i < n; ++i) f[i] *= inv;
}
int id[N];
mint a[18][N];
mint b[18][N];
mint c[18][N];
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
    int K = ii();
    if (K == 0) [[unlikely]] {
        std::cout << (mint(ii()) * mint(ii())).x;
        return 0;
    }
    int n[K];
    for (int i = 0; i < K; ++i) n[i] = ii();
    for (int i = 1; i < K; ++i) n[i] = n[i - 1] * n[i];
    int m = n[K - 1];
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < K; ++j)
            id[i] = (id[i] + i / n[j]) % K;
    for (int i = 0; i < m; ++i) a[id[i]][i] = ii();
    for (int i = 0; i < m; ++i) b[id[i]][i] = ii();
    int o = 2 << std::__lg(m + m - 1);
    for (int i = 0; i < K; ++i) fft(a[i], o);
    for (int i = 0; i < K; ++i) fft(b[i], o);
    for (int i = 0; i < K; ++i)
        for (int j = 0; j < K; ++j)
            for (int k = 0; k < o; ++k)
                c[(i + j) % K][k] += a[i][k] * b[j][k];
    for (int i = 0; i < K; ++i) ift(c[i], o);
    for (int i = 0; i < m; ++i) oo(c[id[i]][i].x);
    flush();
}
