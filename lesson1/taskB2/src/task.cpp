#include <bits/stdc++.h>
// predecessor_problem
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
    inline auto operator!() {
        char x;
        std::cin >> x;
        return x == '1';
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
        ++ptrI;
        return n ? -x : +x;
    };
    inline auto operator!() {
        if (endI - ptrI < 32) load();
        return *ptrI++ == '1';
    }
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
const int N = 1 << 24;
std::bitset<N * 2> vec;
void insert(int k) {
    k += N;
    for (; k > 0 && !vec[k]; k /= 2)
        vec.set(k);
}
void remove(int k) {
    k += N;
    if (!vec[k]) return;
    vec.reset(k);
    for (; k > 1 && !vec[k ^ 1]; k /= 2)
        vec.reset(k / 2);
}
int ge(int k) {
    k += N;
    if (vec[k]) return k - N;
    for (; k > 0; k /= 2) {
        if (!(k & 1) && vec[k ^ 1]) {
            k ^= 1;
            while (k < N)
                k = vec[k * 2] ? k * 2 : k * 2 + 1;
            return k - N;
        }
    }
    return -1;
}
int le(int k) {
    k += N;
    if (vec[k]) return k - N;
    for (; k > 0; k /= 2) {
        if (+(k & 1) && vec[k ^ 1]) {
            k ^= 1;
            while (k < N)
                k = vec[k * 2 + 1] ? k * 2 + 1 : k * 2;
            return k - N;
        }
    }
    return -1;
}
int main() {
    int n = io();
    int m = io();
    for (int i = 0; i < n; ++i) vec[N + i] = !io;
    for (int i = N + n; i > 1; i -= 2) vec[i / 2] = vec[i] || vec[i ^ 1];
    for (int i = 0; i < m; ++i) {
        switch (io()) {
            case 0: { // insert
                insert(io());
                break;
            }
            case 1: { // remove
                remove(io());
                break;
            }
            case 2: { // contain
                io(int(vec[N + io()]));
                break;
            }
            case 3: { // ge
                io(ge(io()));
                break;
            }
            case 4: { // le
                io(le(io()));
                break;
            }
        }
    }
}
