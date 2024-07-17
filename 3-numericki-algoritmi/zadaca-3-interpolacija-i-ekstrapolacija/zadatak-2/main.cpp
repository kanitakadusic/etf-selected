#include <iostream>
#include <stdexcept>
#include <vector>
#include <iomanip>

#include "Comparator.h"

template <typename F>
std::pair<double, bool> Limit(F function, double x0, double h = 0, double epsilon = 1e-8, int max = 20) {
    if (epsilon < std::numeric_limits<double>::epsilon() || max < 3 || max > 30) throw std::domain_error("Invalid parameters");

    bool infinity = false;
    if (x0 == std::numeric_limits<double>::infinity()) { x0 = 0.; infinity = true; }
    else if (x0 == -std::numeric_limits<double>::infinity()) { x0 = -0.; infinity = true; }

    if (Comparator::isEqual(h, 0)) h = 0.001 * (Comparator::isGreater(1, std::fabs(x0)) ? 1 : std::fabs(x0));

    std::vector<double> y(max);
    double old = std::numeric_limits<double>::infinity();

    for (int i = 0; i < max; i++) {
        y[i] = function((infinity ? 1 / x0 : x0) + h);
        int p = 2;

        for (int k = i - 1; k > -1; k--) {
            y[k] = (p * y[k + 1] - y[k]) / (p - 1);
            p *= 2;
        }

        if (std::fabs(y[0] - old) < epsilon) return {y[0], true};
        old = y[0];
        h /= 2;
    }

    return {y[0], false};
}

int main() {
    std::vector<std::pair<double, bool>> limits;

    limits.push_back(Limit([](double x) { return x / (std::sqrt(1 + x) - 1); }, 0, 0, 1e-10, 25));
    limits.push_back(Limit([](double x) { return std::pow(1 + x, 1 / x); }, 0));
    limits.push_back(Limit([](double x) { return std::sin(x) / x; }, 0, 0.0005, 1e-9, 15));
    limits.push_back(Limit([](double x) { return (std::pow(std::exp(1), x) - 1) / x; }, 0));
    limits.push_back(Limit([](double x) { return std::pow(x, 1 / (1 - x)); }, 1, 0.0002, 1e-15, 28));
    limits.push_back(Limit([](double x) { return std::pow(x, 1 / x); }, std::numeric_limits<double>::infinity()));
    limits.push_back(Limit([](double x) { return std::pow(2, x) / std::pow(x, 2); }, std::numeric_limits<double>::infinity()));

    for (auto& limit : limits)
        std::cout << std::setw(10) << limit.first << std::setw(10) << std::boolalpha << limit.second << std::endl;

    return 0;
}
