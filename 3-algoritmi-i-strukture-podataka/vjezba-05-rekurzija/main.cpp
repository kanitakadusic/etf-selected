#include <iostream>
using std::cout;

int fib2_0(int n, int pp = 0, int p = 1) {
    if (n == 0) return pp;
    if (n == 1) return p;
    return fib2_0(n - 1, p, pp + p);
}

int nzd(int x, int y) {
    if (y <= 0) return x;
    return nzd(y, x % y);
}

int main() {

    return 0;
}