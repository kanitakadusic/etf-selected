#ifndef ZADATAK_1_PIECEWISEPOLYNOMIALINTERPOLATORTEST_H
#define ZADATAK_1_PIECEWISEPOLYNOMIALINTERPOLATORTEST_H

#include "../main/PiecewisePolynomialInterpolator.h"

class PiecewisePolynomialInterpolatorTest {
public:
    static void ConstructorTest() {
        try { PiecewisePolynomialInterpolator({{0, 1}, {2, 3}}, 0); }
        catch (...) {
            try { PiecewisePolynomialInterpolator({{0, 1}, {2, 3}}, 2); }
            catch (...) { return; }
        }

        throw std::logic_error("PiecewisePolynomialInterpolatorTest: Constructor failed");
    }

    static void InterpolationTest() {
        PolynomialInterpolator a({{-3, 20}, {-1, 6}, {1, 0}, {1.5, -0.25}, {2, 0}, {4, 6}, {6, 20}});

        std::vector<double> x{a(-3), a(6), a(-2), a(0), a(3), a(5), a(-4), a(7)};
        std::vector<double> y{20, 20, 12, 2, 2, 12, 30, 30};

        if (!std::equal(x.begin(), x.end(), y.begin(), [](double x, double y) {
            return Comparator::isEqual(x, y);
        })) throw std::logic_error("PiecewisePolynomialInterpolatorTest: Interpolation failed");
    }
};

#endif //ZADATAK_1_PIECEWISEPOLYNOMIALINTERPOLATORTEST_H
