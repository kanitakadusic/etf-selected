#ifndef ZADATAK_1_TRIGONOMETRICINTERPOLATORTEST_H
#define ZADATAK_1_TRIGONOMETRICINTERPOLATORTEST_H

#include "../main/TrigonometricInterpolator.h"

class TrigonometricInterpolatorTest {
public:
    static void ConstructorTest() {
        try { TrigonometricInterpolator({{0, 1}, {M_PI / 2, 0}, {M_PI, -1}}); }
        catch (...) { return; }

        throw std::logic_error("TrigonometricInterpolatorTest: Constructor failed");
    }

    static void InterpolationTest() {
        {
            TrigonometricInterpolator a({{0, 0}, {M_PI / 2, 1}, {3 * M_PI / 2, -1}, {2 * M_PI, 0}});

            std::vector<double> x{a(0), a(3 * M_PI / 2), a(3), a(4), a(-1), a(8)};
            std::vector<double> y{std::sin(0), std::sin(3 * M_PI / 2), std::sin(3), std::sin(4), std::sin(-1), std::sin(8)};

            if (!std::equal(x.begin(), x.end(), y.begin(), [](double x, double y) {
                return Comparator::isEqual(x, y);
            })) throw std::logic_error("TrigonometricInterpolatorTest: Interpolation failed");
        }

        {
            TrigonometricInterpolator a({{0, 1}, {M_PI / 2, 0}, {M_PI, -1}, {3 * M_PI / 2, 0}, {2 * M_PI, 1}});

            std::vector<double> x{a(0), a(2 * M_PI), a(3), a(5), a(-1), a(8)};
            std::vector<double> y{std::cos(0), std::cos(2 * M_PI), std::cos(3), std::cos(5), std::cos(-1), std::cos(8)};

            if (!std::equal(x.begin(), x.end(), y.begin(), [](double x, double y) {
                return Comparator::isEqual(x, y);
            })) throw std::logic_error("TrigonometricInterpolatorTest: Interpolation failed");
        }
    }
};

#endif //ZADATAK_1_TRIGONOMETRICINTERPOLATORTEST_H
