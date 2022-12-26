#include <bits/stdc++.h>
// convolution_mod_2_64
using i64 = int64_t;
using u32 = uint32_t;
using u64 = uint64_t;
const int N = 1 << 20;
using cplx = std::complex<double>;
cplx w[N];
void __attribute((constructor)) init() {
    using cplx = std::complex<long double>;
    const long double PI = std::acos(-1.l);
    for (int i = 0; i < N / 2; ++i) w[N / 2 + i] = std::exp(cplx(0, 2 * PI / N * i));
    for (int i = N / 2 - 1; i; --i) w[i] = w[i * 2];
}
void fft_pre(i64 a[], i64 b[], i64 c[], i64 d[], int n) {
    int sumA = 0;
    int sumB = 0;
    int sumC = 0;
    int sumD = 0;
    for (int i = 0; i < n; ++i) {
        const u64 x = a[i];
        int A = x >> 00 & 65535;
        int B = x >> 16 & 65535;
        int C = x >> 32 & 65535;
        int D = x >> 48 & 65535;
        sumA += A;
        sumB += B;
        sumC += C;
        sumD += D;
        if (sumA > 65535) sumA -= 65536, A -= 65536, B++;
        if (sumB > 65535) sumB -= 65536, B -= 65536, C++;
        if (sumC > 65535) sumC -= 65536, C -= 65536, D++;
        if (sumD > 65535) sumD -= 65536, D -= 65536;
        a[i] = A;
        b[i] = B;
        c[i] = C;
        d[i] = D;
    }
}
void fft(i64 a[], i64 b[], cplx A[], cplx B[], int n) {
    static cplx f[N];
    for (int i = 0; i < n; ++i) f[i] = cplx(a[i], b[i]);
    for (int k = n / 2; k; k /= 2)
        for (int i = 0; i < n; i += k + k)
            for (int j = 0; j < k; ++j) {
                cplx x = f[i + j];
                cplx y = f[i + j + k];
                f[i + j] = x + y;
                f[i + j + k] = (x - y) * w[j + k];
            }
    A[0] = f[0].real(), B[0] = f[0].imag();
    A[1] = f[1].real(), B[1] = f[1].imag();
    for (int k = 2; k < n; k *= 2) {
        for (int i = k, j = k + k - 1; i < j; ++i, --j) {
            double a0 = f[i].real() / 2, a1 = f[i].imag() / 2;
            double b0 = f[j].real() / 2, b1 = f[j].imag() / 2;
            A[i] = {a0 + b0, a1 - b1};
            A[j] = {a0 + b0, b1 - a1};
            B[i] = {a1 + b1, b0 - a0};
            B[j] = {a1 + b1, a0 - b0};
        }
    }
}
void ift(i64 a[], i64 b[], cplx A[], cplx B[], int n) {
    static cplx f[N];
    for (int i = 0; i < n; ++i) f[i] = A[i] + B[i] * cplx(0, 1);
    for (int k = 1; k < n; k *= 2)
        for (int i = 0; i < n; i += k + k)
            for (int j = 0; j < k; ++j) {
                cplx x = f[i + j];
                cplx y = f[i + j + k] * conj(w[j + k]);
                f[i + j] = x + y;
                f[i + j + k] = x - y;
            }
    for (int i = 0; i < n; ++i) f[i] /= n;
    for (int i = 0; i < n; ++i) {
        a[i] = std::llround(f[i].real());
        b[i] = std::llround(f[i].imag());
    }
}
i64 a0[N], a1[N], a2[N], a3[N];
i64 b0[N], b1[N], b2[N], b3[N];
cplx A0[N], A1[N], A2[N], A3[N];
cplx B0[N], B1[N], B2[N], B3[N];
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
    for (int i = 0; i < n; ++i) a0[i] = ii();
    for (int i = 0; i < m; ++i) b0[i] = ii();
    fft_pre(a0, a1, a2, a3, n);
    fft_pre(b0, b1, b2, b3, m);
    fft(a0, b0, A0, B0, o);
    fft(a1, b1, A1, B1, o);
    fft(a2, b2, A2, B2, o);
    fft(a3, b3, A3, B3, o);
    for (int i = 0; i < o; ++i) {
        cplx a0 = A0[i], a1 = A1[i], a2 = A2[i], a3 = A3[i];
        cplx b0 = B0[i], b1 = B1[i], b2 = B2[i], b3 = B3[i];
        A0[i] = a0 * b0;
        A1[i] = a0 * b1 + a1 * b0;
        A2[i] = a0 * b2 + a1 * b1 + a2 * b0;
        A3[i] = a0 * b3 + a1 * b2 + a2 * b1 + a3 * b0;
    }
    ift(a0, a1, A0, A1, o);
    ift(a2, a3, A2, A3, o);
    for (int i = 0; i < n + m - 1; ++i) oo(u64(a0[i] + (a1[i] << 16) + (a2[i] << 32) + (a3[i] << 48)));
    flush();
}
