#include <bits/stdc++.h>
// exp_of_formal_power_series
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
    mint inv() const { return pow(P - 2); }
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
mint inv[N];
void __attribute((constructor)) init() {
    w[N / 2] = 1;
    mint g = mint(3).pow(P / N);
    for (int i = N / 2 + 1; i < N; ++i) w[i] = w[i - 1] * g;
    for (int i = N / 2 - 1; i > 0; --i) w[i] = w[i * 2];
    inv[0] = inv[1] = 1;
    for (int i = 2; i < N; ++i) inv[i] = inv[P % i] * (P - P / i);
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
void Exp(const mint a[], mint f[], int n) {
    /*
     * F = Exp(A)
     *dF = Exp(A) * dA
     * F = Int(F * dA) + 1
     */
    static mint A[N + N];
    for (int k = 1; k < n * 2; k *= 2) {
        for (int i = 0; i < k; ++i)
            A[k + i] = a[i] * i;
        fft(A + k, k);
    }
    f[0] = 1;
    for (int i = 1; i < n; ++i) f[i] = 0;
    auto solve = [&](auto solve, int l, int r) -> void {
        if (l == r - 1) {
            f[l] *= inv[l];
        } else {
            int mid = (l + r) >> 1;
            solve(solve, l, mid);
            int n = 2 << std::__lg(r - l - 1);
            mint F[n];
            for (int i = l; i < mid; ++i) F[i - l] = f[i];
            fft(F, n);
            for (int i = 0; i < n; ++i) F[i] *= A[n + i];
            ift(F, n);
            for (int i = mid; i < r; ++i) f[i] += F[i - l];
            solve(solve, mid, r);
        }
    };
    solve(solve, 0, n);
}
mint a[N];
mint f[N];
int main() {
    int n = io();
    for (int i = 0; i < n; ++i) a[i] = io();
    Exp(a, f, n);
    for (int i = 0; i < n; ++i) io(f[i].x);
}
