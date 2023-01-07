#include <bits/stdc++.h>
// sqrt_of_formal_power_series
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
    mint sqrt() const {
        if (pow(P / 2).x != 1) return 0;
        mint a = pow(60);
        mint b = pow(119);
        for (int k = 1 << 21; k > 0; k /= 2)
            if (b.pow(k).x != 1) {
                a *= mint(3).pow(P / k / 4);
                b *= mint(3).pow(P / k / 2);
            }
        return std::min(a.x, P - a.x);
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
void SqrtInv(const mint a[], mint f[], mint A[], int n) {
    /* x12
     * 1 = A *(F0 + F1 << n)*(F0 + F1 << n)
     * 1 - A * F0 * F0 = 2A * F0 * F1 << n + O(x^2n)
     *(1 - A * F0 * F0) >> n = 2A * F0 * F1 + O(x^n)
     *(F0 - A * F0 * F0 * F0) >> n = 2F1 + O(x^n)
     */
    if (n == 1) {
        f[0] = a[0].sqrt().inv();
    } else {
        SqrtInv(a, f, A, n / 2);
        mint F[n + n];
        for (int i = 0; i < n; ++i) A[i] = a[i];
        for (int i = 0; i < n / 2; ++i) F[i] = f[i];
        fft(A, n + n);
        fft(F, n + n);
        for (int i = 0; i < n + n; ++i) F[i] *= A[i] * F[i] * F[i] * ((P + 1) / 2);
        ift(F, n + n);
        for (int i = n / 2; i < n; ++i) f[i] = -F[i];
    }
}
void Sqrt(const mint a[], mint g[], int n) {
    /* x11
     * A =(G0 + G1 << n)*(G0 + G1 << n)
     * A - G0 * G0 = 2G0 * G1 << n + O(x^2n)
     *(A - G0 * G0) >> n = 2G0 * G1 + O(x^n)
     *(A - G0 * G0) >> n * F0 = 2G1 + O(x^n)
     * G0 = A0 * F0
     */
    if (n == 1) {
        g[0] = a[0].sqrt();
    } else {
        mint A[n];
        mint F[n];
        mint G[n];
        SqrtInv(a, F, A, n / 2);
        fft(F, n);
        for (int i = 0; i < n; ++i) G[i] = A[i] * F[i];
        ift(G, n);
        for (int i = n / 2; i < n; ++i) G[i] = 0;
        for (int i = 0; i < n / 2; ++i) g[i] = G[i];
        fft(G, n);
        for (int i = 0; i < n; ++i) A[i] = a[i];
        fft(A, n);
        for (int i = 0; i < n; ++i) G[i] = (A[i] - G[i] * G[i]) * F[i] * ((P + 1) / 2);
        ift(G, n);
        for (int i = n / 2; i < n; ++i) g[i] = G[i];
    }
}
mint a[N];
mint f[N];
int main() {
    int n = io();
    for (int i = 0; i < n; ++i) a[i] = io();
    for (int i = 0; i < n; ++i)
        if (a[i].x) {
            if (i % 2 == 1 || a[i].sqrt().x == 0) {
                io(-1);
            } else {
                Sqrt(a + i, f + i / 2, 2 << std::__lg(n));
                for (int j = 0; j < n; ++j) io(f[j].x);
            }
            return 0;
        }
    for (int i = 0; i < n; ++i) io(0);
}
