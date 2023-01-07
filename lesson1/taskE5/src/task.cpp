#include <bits/stdc++.h>
// pow_of_formal_power_series
using i64 = int64_t;
using u32 = uint32_t;
using u64 = uint64_t;
#ifdef LOCAL
class IO {
public:
    inline auto operator()() {
        u64 x;
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
        u64 x{};
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
void Inv(const mint a[], mint f[], int n) {
    /* x10
     * 1 = A *(F0 + F1 << n)
     * 1 - A * F0 = A * F1 << n
     *(1 - A * F0) >> n = A * F1
     *(1 - A * F0) >> n * F0 = F1 + O(x^n)
     */
    if (n == 1) {
        f[0] = a[0].inv();
    } else {
        Inv(a, f, n / 2);
        mint A[n];
        mint F[n];
        for (int i = 0; i < n; ++i) A[i] = a[i];
        for (int i = 0; i < n / 2; ++i) F[i] = f[i];
        fft(A, n);
        fft(F, n);
        for (int i = 0; i < n; ++i) A[i] *= F[i];
        ift(A, n);
        for (int i = 0; i < n / 2; ++i) A[i] = 0;
        fft(A, n);
        for (int i = 0; i < n; ++i) A[i] *= F[i];
        ift(A, n);
        for (int i = n / 2; i < n; ++i) f[i] = -A[i];
    }
}
void Div(const mint b[], const mint a[], mint g[], int n) {
    /* x13
     * B = A *(G0 + G1 << n)
     * B - A * G0 = A * G1 << n
     *(B - A * G0) >> n = A * G1
     *(B - A * G0) >> n * F0 = G1 + O(x^n)
     * G0 = F0 * B0
     */
    if (n == 1) {
        g[0] = b[0] * a[0].inv();
    } else {
        mint A[n];
        mint F[n];
        mint G[n];
        Inv(a, F, n / 2);
        fft(F, n);
        for (int i = 0; i < n / 2; ++i) G[i] = b[i];
        fft(G, n);
        for (int i = 0; i < n; ++i) G[i] *= F[i];
        ift(G, n);
        for (int i = n / 2; i < n; ++i) G[i] = 0;
        for (int i = 0; i < n / 2; ++i) g[i] = G[i];
        fft(G, n);
        for (int i = 0; i < n; ++i) A[i] = a[i];
        fft(A, n);
        for (int i = 0; i < n; ++i) G[i] *= A[i];
        ift(G, n);
        for (int i = 0; i < n / 2; ++i) G[i] = 0;
        for (int i = n / 2; i < n; ++i) G[i] -= b[i];
        fft(G, n);
        for (int i = 0; i < n; ++i) G[i] *= F[i];
        ift(G, n);
        for (int i = n / 2; i < n; ++i) g[i] = -G[i];
    }
}
void Der(const mint a[], mint f[], int n) {
    f[n - 1] = 0;
    for (int i = 1; i < n; ++i) f[i - 1] = a[i] * i;
}
void Int(const mint a[], mint f[], int n) {
    f[0] = 0;
    for (int i = 1; i < n; ++i) f[i] = a[i - 1] * inv[i];
}
void Log(const mint a[], mint f[], int n) {
    mint b[n];
    mint c[n];
    Der(a, b, n);
    Div(b, a, c, n);
    Int(c, f, n);
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
void Pow(const mint a[], mint f[], int n, u64 k) {
    static mint A[N];
    static mint B[N];
    mint inv = a[0].inv();
    mint mul = a[0].pow(k);
    for (int i = 0; i < n; ++i) A[i] = a[i] * inv;
    Log(A, B, 2 << std::__lg(n));
    for (int i = 0; i < n; ++i) B[i] *= int(k % P);
    Exp(B, A, n);
    for (int i = 0; i < n; ++i) f[i] = A[i] * mul;
}
mint a[N];
mint f[N];
int main() {
    int n = io();
    u64 k = io();
    if (k == 0) {
        io(1);
        for (int i = 1; i < n; ++i) io(0);
    } else {
        for (int i = 0; i < n; ++i) a[i] = io();
        for (int i = 0; i * k < n; ++i)
            if (a[i].x) {
                Pow(a + i, f + i * k, n - i * k, k);
                for (int j = 0; j < n; ++j) io(f[j].x);
                return 0;
            }
        for (int i = 0; i < n; ++i) io(0);
    }
}
