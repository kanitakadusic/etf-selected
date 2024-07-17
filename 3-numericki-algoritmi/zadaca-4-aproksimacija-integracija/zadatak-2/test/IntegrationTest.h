#ifndef ZADATAK_2_INTEGRATIONTEST_H
#define ZADATAK_2_INTEGRATIONTEST_H

#include <algorithm>
#include <functional>
#include <tuple>

#include "../main/Integration.h"

bool isEqual(double x, double y, double epsilon = -1.) {
    if (epsilon == -1.) epsilon = 10 * std::numeric_limits<double>::epsilon() * (std::fabs(x) + std::fabs(y));
    return std::fabs(x - y) <= epsilon;
}

std::vector<std::tuple<std::function<double(double)>, double, double, double>> generate() {
    return {
            {(double(*)(double))(std::sin), 0, M_PI, 2},
            {(double(*)(double))(std::cos), 0, M_PI / 2, 1},
            {(double(*)(double))(std::exp), 1, 2, std::exp(2) - std::exp(1)},
            {[](double x) { return 1 / (x * x * x); }, 2, 4, 3. / 32},
            {[](double x) { return 4 * std::sin(x / 3); }, -M_PI, M_PI / 2, 6 - 2 * std::pow(3, 3. / 2)},
            {[](double x) { return 5 * x * x - 8 * x + 5; }, 1, 4, 60},
            {[](double x) { return 1 / (x * x - x + 1); }, 1, 2, M_PI / std::pow(3, 3. / 2)},
            {[](double x) { return 1 / (x * x * (x * x + 25)); }, 1, 2, -std::atan(2. / 5) / 125 + std::atan(1. / 5) / 125 + 1. / 50},
            {[](double x) { return std::pow(x, 5) * std::sqrt(2 - x * x * x); }, -2, -1, -8 * std::pow(10, 3. / 2) / 9 - 2 / (5 * std::sqrt(3))}
    };
}

void RombergIntegrationTest() {
    std::vector<std::tuple<std::function<double(double)>, double, double, double>> test(generate());
    std::vector<double> x, y;

    for (std::tuple<std::function<double(double)>, double, double, double>& i : test) {
        x.push_back(RombergIntegration(std::get<0>(i), std::get<1>(i), std::get<2>(i)).first);
        y.push_back(std::get<3>(i));

        x.push_back(RombergIntegration(std::get<0>(i), std::get<2>(i), std::get<1>(i)).first);
        y.push_back(-std::get<3>(i));
    }

    if (!std::equal(x.begin(), x.end(), y.begin(), [](double x, double y) {
        return isEqual(std::round(x * 1e8) / 1e8, std::round(y * 1e8) / 1e8);
    })) throw std::logic_error("RombergIntegrationTest: Integration failed");

    // RombergIntegration([](double x) { return 1 / std::sqrt(x); }, 0, 1).first => nan

    try { RombergIntegration([](double x) { return x; }, 1, 2, -1); }
    catch (...) {
        try { RombergIntegration([](double x) { return x; }, 1, 2, 0.1, -1); }
        catch (...) {
            try { RombergIntegration([](double x) { return x; }, 1, 2, 0.1, 1, -1); }
            catch (...) {
                try { RombergIntegration([](double x) { return x; }, 1, 2, 0.1, 1, 2); }
                catch (...) {
                    return;
                }
            }
        }
    }

    throw std::logic_error("RombergIntegrationTest: Validation failed");
}

void TanhSinhIntegrationTest() {
    std::vector<std::tuple<std::function<double(double)>, double, double, double>> test(generate());
    std::vector<double> x, y;

    for (std::tuple<std::function<double(double)>, double, double, double>& i : test) {
        x.push_back(TanhSinhIntegration(std::get<0>(i), std::get<1>(i), std::get<2>(i)).first);
        y.push_back(std::get<3>(i));

        x.push_back(TanhSinhIntegration(std::get<0>(i), std::get<2>(i), std::get<1>(i)).first);
        y.push_back(-std::get<3>(i));
    }

    if (!std::equal(x.begin(), x.end(), y.begin(), [](double x, double y) {
        return isEqual(std::round(x * 1e8) / 1e8, std::round(y * 1e8) / 1e8);
    })) throw std::logic_error("TanhSinhIntegrationTest: Integration failed");

    // TanhSinhIntegration([](double x) { return 1 / std::sqrt(x); }, 0, 1).first => 2

    try { TanhSinhIntegration([](double x) { return x; }, 1, 2, -1); }
    catch (...) {
        try { TanhSinhIntegration([](double x) { return x; }, 1, 2, 0.1, -1); }
        catch (...) {
            try { TanhSinhIntegration([](double x) { return x; }, 1, 2, 0.1, 1, -1); }
            catch (...) {
                try { TanhSinhIntegration([](double x) { return x; }, 1, 2, 0.1, 1, 2); }
                catch (...) {
                    try { TanhSinhIntegration([](double x) { return x; }, 1, 2, 0.1, 2, 1, -1); }
                    catch (...) {
                        return;
                    }
                }
            }
        }
    }

    throw std::logic_error("TanhSinhIntegrationTest: Validation failed");
}

void AdaptiveIntegrationTest() {
    std::vector<std::tuple<std::function<double(double)>, double, double, double>> test(generate());
    std::vector<double> x, y;

    for (std::tuple<std::function<double(double)>, double, double, double>& i : test) {
        x.push_back(AdaptiveIntegration(std::get<0>(i), std::get<1>(i), std::get<2>(i)).first);
        y.push_back(std::get<3>(i));

        x.push_back(AdaptiveIntegration(std::get<0>(i), std::get<2>(i), std::get<1>(i)).first);
        y.push_back(-std::get<3>(i));
    }

    if (!std::equal(x.begin(), x.end(), y.begin(), [](double x, double y) {
        return isEqual(std::round(x * 1e8) / 1e8, std::round(y * 1e8) / 1e8);
    })) throw std::logic_error("AdaptiveIntegrationTest: Integration failed");

    // AdaptiveIntegration([](double x) { return 1 / std::sqrt(x); }, 0, 1).first => 1.99998

    try { AdaptiveIntegration([](double x) { return x; }, 1, 2, -1); }
    catch (...) {
        try { AdaptiveIntegration([](double x) { return x; }, 1, 2, 0.1, -1); }
        catch (...) {
            try { AdaptiveIntegration([](double x) { return x; }, 1, 2, 0.1, 1, -1); }
            catch (...) {
                return;
            }
        }
    }

    throw std::logic_error("AdaptiveIntegrationTest: Validation failed");
}

#endif //ZADATAK_2_INTEGRATIONTEST_H
