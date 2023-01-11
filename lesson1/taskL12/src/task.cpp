#include <bits/stdc++.h>
// range_kth_smallest
#define mid ((l + r) >> 1)
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
const int N = 2e5 + 5;
const int M = 4e6 + 5;
struct Node {
    int L;
    int R;
    int cnt;
} node[M], *ptr = node;
void insert(int *x, int l, int r, int k) {
    while (l != r - 1) {
        auto [L, R, cnt] = node[*x];
        if (k < mid)
            *x = new(++ptr) Node{L, R, ++cnt} - node, x = &ptr->L, r = mid;
        else
            *x = new(++ptr) Node{L, R, cnt++} - node, x = &ptr->R, l = mid;
    }
}
int query(int x, int y, int l, int r, int k) {
    while (l != r - 1) {
        auto [Lx, Rx, _x] = node[x];
        auto [Ly, Ry, _y] = node[y];
        if (int cnt = _y - _x; cnt <= k)
            x = Rx, y = Ry, l = mid, k -= cnt;
        else
            x = Lx, y = Ly, r = mid;
    }
    return l;
}
int val[N];
int vec[N];
int tree[N];
int main() {
    int n = io();
    int m = io();
    for (int i = 0; i < n; ++i) val[i] = vec[i] = io();
    std::sort(vec, vec + n);
    for (int i = 0; i < n; ++i) val[i] = std::lower_bound(vec, vec + n, val[i]) - vec;
    for (int i = 0; i < n; ++i) {
        tree[i + 1] = tree[i];
        insert(tree + i + 1, 0, n, val[i]);
    }
    for (int i = 0; i < m; ++i) {
        int l = io();
        int r = io();
        int x = query(tree[l], tree[r], 0, n, io());
        io(vec[x]);
    }
}
