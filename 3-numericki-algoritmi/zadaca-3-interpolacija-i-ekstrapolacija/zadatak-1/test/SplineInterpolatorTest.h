#ifndef ZADATAK_1_SPLINEINTERPOLATORTEST_H
#define ZADATAK_1_SPLINEINTERPOLATORTEST_H

#include "../main/SplineInterpolator.h"

class SplineInterpolatorTest {
public:
    static void InterpolationTest() {
        std::vector<std::pair<double, double>> points;
        for (double i = 0; i <= 2 * M_PI; i += M_PI / 16)
            points.emplace_back(i, std::sin(i));

        SplineInterpolator a(points);

        std::vector<double> x{a(0), a(M_PI / 16), a(1), a(2.45), a(3), a(4), a(4.95), a(5.75)};
        std::vector<double> y{0, std::sin(M_PI / 16), std::sin(1), std::sin(2.45), std::sin(3), std::sin(4), std::sin(4.95), std::sin(5.75)};

        // extrapolation: a(-0.5), a(-M_PI / 2), a(-M_PI), a(6.5), a(5 * M_PI / 2), a(3 * M_PI)
        // (calculated / correct): (-0.47923 / -0.479426), (-0.926897 / -1), (2.00952 / 0), (0.179425 / 0.21512), (-0.525504 / 1), (-10.7335 / 0)

        if (!std::equal(x.begin(), x.end(), y.begin(), [](double x, double y) {
            return Comparator::isEqual(std::round(x * 10000.) / 10000., std::round(y * 10000.) / 10000.);
        })) throw std::logic_error("SplineInterpolatorTest: Interpolation failed");
    }
};

#endif //ZADATAK_1_SPLINEINTERPOLATORTEST_H
