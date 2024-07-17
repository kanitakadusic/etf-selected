#ifndef NELDER_MEAD_CPP_OPTIMIZATIONTEST_H
#define NELDER_MEAD_CPP_OPTIMIZATIONTEST_H

#include <iostream>
#include <iomanip>
#include <functional>
#include <sstream>
#include <string>

#include "../main/Optimization.h"

std::vector<std::tuple<std::string, std::function<double(const VECTOR&)>, VECTOR, double>> generateTests() {
    // function name, function, point of minimum, value at minimum
    return {
            {"Paraboloid", [](const VECTOR& x) {
                return (x[0] + 5) * (x[0] + 5) + (x[1] + 2) * (x[1] + 2);
            }, {-5, -2}, 0},
            {"Sphere", [](const VECTOR& x) {
                double sum = 0.0;
                for (double e : x) sum += e * e;
                return sum;
            }, {0, 0, 0, 0, 0, 0, 0, 0}, 0},
            {"Rosenbrock", [](const VECTOR& x) {
                double sum = 0.0;
                for (int i = 0; i < x.size() - 1; i++) sum += 100 * std::pow(x[i + 1] - x[i] * x[i], 2) + (1 - x[i]) * (1 - x[i]);
                return sum;
            }, {1, 1, 1, 1, 1}, 0},
            {"Beale", [](const VECTOR& x) {
                return std::pow(1.5 - x[0] + x[0] * x[1], 2) + std::pow(2.25 - x[0] + x[0] * x[1] * x[1], 2) + std::pow(2.625 - x[0] + x[0] * x[1] * x[1] * x[1], 2);
            }, {3, 0.5}, 0},
            {"Booth", [](const VECTOR& x) {
                return std::pow(x[0] + 2 * x[1] - 7, 2) + std::pow(2 * x[0] + x[1] - 5, 2);
            }, {1, 3}, 0},
            {"Matyas", [](const VECTOR& x) {
                return 0.26 * (x[0] * x[0] + x[1] * x[1]) - 0.48 * x[0] * x[1];
            }, {0, 0}, 0},
            {"Easom", [](const VECTOR& x) {
                return -std::cos(x[0]) * std::cos(x[1]) * std::exp(- (x[0] - M_PI) * (x[0] - M_PI) - (x[1] - M_PI) * (x[1] - M_PI));
            }, {M_PI, M_PI}, -1},
            {"McCormick", [](const VECTOR& x) {
                return std::sin(x[0] + x[1]) + (x[0] - x[1]) * (x[0] - x[1]) - 1.5 * x[0] + 2.5 * x[1] + 1;
            }, {-0.54719, -1.54719}, -1.9133}
    };
}

template <typename T>
std::string toString(T x) {
    std::ostringstream output;
    output << x;
    return output.str();
}

std::string pointToString(const VECTOR& x) {
    std::string output = "(";
    for (double e : x) output += toString(e) + ",";

    output.back() = ')';
    return output;
}

void NelderMeadTest() {
    auto tests(generateTests());
    auto w15(std::setw(15)), w40(std::setw(40));

    std::cout << std::left << w15 << "FUNCTION" << w40 << "ACCURATE" << w15 << "TRUE/FALSE" << w40 << "TESTED" << std::endl;
    std::cout << std::boolalpha << std::string(110, '-') << std::endl;

    for (const auto& accurate : tests) {
        std::string name(std::get<0>(accurate));
        auto f(std::get<1>(accurate));
        VECTOR aMinimum(std::get<2>(accurate));
        double aValue = std::get<3>(accurate);

        auto tested(NelderMead(f, getSimplex(aMinimum.size() + 1), 1e-12));

        bool good = tested.first;
        VECTOR tMinimum(tested.second);
        double tValue = f(tMinimum);

        std::cout << w15 << name;
        std::cout << w40 << "f" + pointToString(aMinimum) + " = " + toString(aValue);
        std::cout << w15 << good;
        std::cout << w40 << "f" + pointToString(tMinimum) + " = " + toString(tValue) << std::endl;
    }
}

#endif //NELDER_MEAD_CPP_OPTIMIZATIONTEST_H
