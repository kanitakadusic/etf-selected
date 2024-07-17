#ifndef ZADATAK_1_LINEARINTERPOLATORTEST_H
#define ZADATAK_1_LINEARINTERPOLATORTEST_H

#include "../main/LinearInterpolator.h"

class LinearInterpolatorTest {
public:
    static void InterpolationTest() {
        LinearInterpolator a({{1, 1}, {3, 3}, {5, 2}});

        std::vector<double> x{a(1), a(5), a(2), a(4), a(0), a(6)};
        std::vector<double> y{1, 2, 2, 2.5, 0, 1.5};

        if (!std::equal(x.begin(), x.end(), y.begin(), [](double x, double y) {
            return Comparator::isEqual(x, y);
        })) throw std::logic_error("LinearInterpolatorTest: Interpolation failed");
    }
};

#endif //ZADATAK_1_LINEARINTERPOLATORTEST_H
