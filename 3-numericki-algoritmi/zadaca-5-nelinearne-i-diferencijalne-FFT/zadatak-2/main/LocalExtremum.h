#ifndef ZADATAK_2_LOCALEXTREMUM_H
#define ZADATAK_2_LOCALEXTREMUM_H

#include <stdexcept>

#include "Comparator.h"

template <typename F>
void BracketMinimum(F f, double x0, double& a, double& b, double& c, double h, double hMax, double lambda) {
    a = x0 - h; c = x0; b = x0 + h;
    double fa = f(a), fc = f(c), fb = f(b);

    if (Comparator::isLess(fb, fc) || (h *= -1, Comparator::isLess(fa, fc))) {
        while (Comparator::isLess(std::fabs(h), hMax)) {
            a = c - h; fa = f(a);
            b = c + h; fb = f(b);

            while (!std::isfinite(fa)) {
                h /= 2 * (1 + lambda);
                a = c - h; fa = f(a);
            }

            while (!std::isfinite(fb)) {
                h /= 2 * (1 + lambda);
                b = c + h; fb = f(b);
            }

            if (Comparator::isLess(fc, fa) && Comparator::isLess(fc, fb)) break;

            h *= lambda;
            c = b; fc = fb;
        }

        if (Comparator::isGreaterEqual(std::fabs(h), hMax)) throw std::logic_error("Minimum has not found");
    }

    if (Comparator::isGreater(a, b)) std::swap(a, b);
}

template <typename F>
double FindMinimum(F f, double x0, double epsilon = 1e-8, double h = 1e-5, double hMax = 1e10, double lambda = 1.4) {
    if (epsilon < 0 || h < 0 || hMax < 0 || lambda < 0) throw std::domain_error("Invalid parameters");

    double a, b, c, d;
    BracketMinimum(f, x0, a, b, c, h, hMax, lambda);

    const double k = (1 + std::sqrt(5)) / 2;
    if (Comparator::isLess(std::fabs(c - a), std::fabs(b - c))) d = b - (b - c) / k;
    else { d = c; c = a + (c - a) / k; }

    double u = f(c), v = f(d);

    while (std::fabs(b - a) > epsilon)
        if (Comparator::isLess(u, v)) {
            b = d;
            d = c;
            c = a + (c - a) / k;
            v = u;
            u = f(c);
        } else {
            a = c;
            c = d;
            d = b - (b - d) / k;
            u = v;
            v = f(d);
        }

    return (a + b) / 2;
}

#endif //ZADATAK_2_LOCALEXTREMUM_H
