#ifndef ZADATAK_1_POLYNOMIALINTERPOLATORTEST_H
#define ZADATAK_1_POLYNOMIALINTERPOLATORTEST_H

#include "../main/PolynomialInterpolator.h"

class PolynomialInterpolatorTest {
public:
    static void InterpolationTest() {
        PolynomialInterpolator a({{-3, 20}, {-1, 6}, {1, 0}, {1.5, -0.25}, {2, 0}, {4, 6}, {6, 20}});

        std::vector<double> x{a(-3), a(6), a(-2), a(0), a(3), a(5), a(-4), a(7)};
        std::vector<double> y{20, 20, 12, 2, 2, 12, 30, 30};

        if (!std::equal(x.begin(), x.end(), y.begin(), [](double x, double y) {
            return Comparator::isEqual(x, y);
        })) throw std::logic_error("PolynomialInterpolatorTest: Interpolation failed");
    }

    static void AddPointTest() {
        PolynomialInterpolator a({{-1, 6}, {1, 0}, {1.5, -0.25}, {2, 0}, {4, 6}});

        a.AddPoint({-4, 30});
        a.AddPoint({7, 30});
        a.AddPoint({-2, 12});

        std::vector<double> x{a(-4), a(7), a(-3), a(0), a(3), a(5), a(-7), a(10)};
        std::vector<double> y{30, 30, 20, 2, 2, 12, 72, 72, 2, -3, 1, 0, 0, 0, 0, 0};

        std::vector<double> temp(a.GetCoefficients());
        x.insert(x.end(), temp.begin(), temp.end());

        for (double& test : x)
            if (Comparator::isEqual(test, 0, 1e-12))
                test = 0.;

        if (!std::equal(x.begin(), x.end(), y.begin(), [](double x, double y) {
            return Comparator::isEqual(x, y);
        })) throw std::logic_error("PolynomialInterpolatorTest: AddPoint failed");

        try { a.AddPoint({1.5, 0}); }
        catch (...) { return; }

        throw std::logic_error("PolynomialInterpolatorTest: AddPoint failed");
    }

    static void GetCoefficientsTest() {
        PolynomialInterpolator a({{-3, 20}, {-1, 6}, {1, 0}, {1.5, -0.25}, {2, 0}, {4, 6}, {6, 20}});

        std::vector<double> x(a.GetCoefficients()); x[3] = 0.; // 2.77556e-17
        std::vector<double> y{2, -3, 1, 0, 0, 0, 0};

        if (!std::equal(x.begin(), x.end(), y.begin(), [](double x, double y) {
            return Comparator::isEqual(x, y);
        })) throw std::logic_error("PolynomialInterpolatorTest: GetCoefficients failed");
    }
};

#endif //ZADATAK_1_POLYNOMIALINTERPOLATORTEST_H
