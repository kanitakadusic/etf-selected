#ifndef ZADATAK_2_LOCALEXTREMUMTEST_H
#define ZADATAK_2_LOCALEXTREMUMTEST_H

#include <functional>
#include <vector>
#include <tuple>

#include "../main/LocalExtremum.h"

std::vector<std::tuple<std::function<double(double)>, double, double>> minimum_generate() {
    // function, x0, minimum
    return {
            {[](double x) { return x * x; }, 1, 0},
            {[](double x) { return 2 * x * x * x + 3 * x * x - 12 * x + 5; }, 2, 1},
            {(double(*)(double))(std::sin), 4, M_PI + M_PI / 2},
            {(double(*)(double))(std::cosh), 2, 0},
            {(double(*)(double))(std::fabs), -6, 0}
    };
}

std::vector<std::tuple<std::function<double(double)>, double, double>> noMinimum_generate() {
    // function, x0, minimum
    return {
            {[](double x) { return x; }, 0, -std::numeric_limits<double>::infinity()},
            {[](double x) { return 1 / (x * x); }, -1, 0},
            {(double(*)(double))(std::atan), 2, -M_PI / 2},
            {(double(*)(double))(std::sinh), -2, -std::numeric_limits<double>::infinity()},
            {(double(*)(double))(std::exp), 1, 0}
    };
}

void FindMinimumTest() {
    {
        std::vector<std::tuple<std::function<double(double)>, double, double>> test(minimum_generate());

        for (auto& i : test) {
            double x = FindMinimum(std::get<0>(i), std::get<1>(i));
            double y = std::get<2>(i);

            if (!Comparator::isEqual(std::round(x * 1e6) / 1e6, std::round(y * 1e6) / 1e6))
                throw std::logic_error("FindMinimumTest: Search failed");
        }
    }

    {
        std::vector<std::tuple<std::function<double(double)>, double, double>> test(noMinimum_generate());

        for (auto& i : test) {
            try { FindMinimum(std::get<0>(i), std::get<1>(i), 1e-8, 1e-2, 1e2); }
            catch (...) { continue; }

            throw std::logic_error("FindMinimumTest: Error");
        }
    }

    try { FindMinimum([](double x) { return x; }, 0, -1); }
    catch (...) {
        try { FindMinimum([](double x) { return x; }, 0, 1, -1); }
        catch (...) {
            try { FindMinimum([](double x) { return x; }, 0, 1, 1, -1); }
            catch (...) {
                try { FindMinimum([](double x) { return x; }, 0, 1, 1, 1, -1); }
                catch (...) {
                    return;
                }
            }
        }
    }

    throw std::logic_error("FindMinimumTest: Validation failed");
}

#endif //ZADATAK_2_LOCALEXTREMUMTEST_H
