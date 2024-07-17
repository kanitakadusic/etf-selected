#ifndef ZADATAK_COMPARATOR_H
#define ZADATAK_COMPARATOR_H

#include <cmath>
#include <limits>

class Comparator {
public:
    static bool isEqual(double x, double y, double epsilon = -1.) {
        if (epsilon == -1.) epsilon = 10 * std::numeric_limits<double>::epsilon() * (std::fabs(x) + std::fabs(y));
        return std::fabs(x - y) <= epsilon;
    }

    static bool isLess(double x, double y) {
        return x < y && !isEqual(x, y);
    }

    static bool isLessEqual(double x, double y) {
        return x <= y || isEqual(x, y);
    }

    static bool isGreater(double x, double y) {
        return x > y && !isEqual(x, y);
    }

    static bool isGreaterEqual(double x, double y) {
        return x >= y || isEqual(x, y);
    }
};

#endif //ZADATAK_COMPARATOR_H
