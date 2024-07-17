#ifndef ZADATAK_3_DIFFERENTIALEQUATIONSTEST_H
#define ZADATAK_3_DIFFERENTIALEQUATIONSTEST_H

#include <functional>
#include <tuple>
#include <algorithm>

#include "../main/DifferentialEquations.h"

std::vector<std::tuple<std::function<double(double, double)>, std::pair<double, double>, std::function<double(double)>>> ode_generator() {
    // ode, {x0, y0}, solution
    return {
        {[](double x, double y) { return 2 * x; }, {0, 0}, [](double x) { return x * x; }},
        {[](double x, double y) { return 6 * x - 3 * y + 11; }, {0, 4}, [](double x) { return std::exp(-3 * x) + 2 * x + 3; }},
        {[](double x, double y) { return 4 * x + 3; }, {1, 7}, [](double x) { return 2 * x * x + 3 * x + 2; }},
        {[](double x, double y) { return 3 * std::exp(x) + x * x - 4; }, {0, 5}, [](double x) { return 3 * std::exp(x) + x * x * x / 3 - 4 * x + 2; }},
        {[](double x, double y) { return x * x - 4 * x + 3 - 6 * std::exp(x); }, {0, 8}, [](double x) { return x * x * x / 3 - 2 * x * x + 3 * x - 6 * std::exp(x) + 14; }}
    };
}

std::vector<std::tuple<std::function<std::vector<double>(double, const std::vector<double>&)>, std::pair<double, std::vector<double>>, std::function<std::vector<double>(double)>>> odeSystem_generator() {
    // ode, {x0, y0}, solution
    return {
            {[](double x, const std::vector<double>& y) -> std::vector<double> { return {y[0], y[0] - y[1]}; },
                {0, {1, 2}},
                [](double x) -> std::vector<double> { return {std::exp(x), std::exp(x) / 2 + 3 * std::exp(-x) / 2}; }}
    };
}

void RK4IntegratorTest() {
    auto test(ode_generator());

    for (auto& i : test) {
        std::function<double(double, double)> ode(std::get<0>(i));
        std::pair<double, double> initial(std::get<1>(i));
        std::function<double(double)> solution(std::get<2>(i));

        {
            std::vector<std::pair<double, double>> S(RK4Integrator(ode, initial.first, initial.second, 1, 0.1, 1e-8, false));

            for (auto& pair : S) {
                double x = pair.second;
                double y = solution(pair.first);

                if (!Comparator::isEqual(std::round(x * 1e3) / 1e3, std::round(y * 1e3) / 1e3))
                    throw std::logic_error("RK4IntegratorTest: Non-adaptive failed");
            }
        }

        {
            std::vector<std::pair<double, double>> S(RK4Integrator(ode, initial.first, initial.second, 1, 0.1, 1e-8, true));

            for (auto& pair : S) {
                double x = pair.second;
                double y = solution(pair.first);

                if (!Comparator::isEqual(std::round(x * 1e3) / 1e3, std::round(y * 1e3) / 1e3))
                    throw std::logic_error("RK4IntegratorTest: Adaptive failed");
            }
        }
    }
}

void RK4SystemIntegratorTest() {
    auto test(odeSystem_generator());

    for (auto& i : test) {
        std::function<std::vector<double>(double, const std::vector<double>&)> ode(std::get<0>(i));
        std::pair<double, std::vector<double>> initial(std::get<1>(i));
        std::function<std::vector<double>(double)> solution(std::get<2>(i));

        std::vector<std::pair<double, std::vector<double>>> S(RK4SystemIntegrator(ode, initial.first, initial.second, 1, 0.1));

        for (auto& pair : S) {
            std::vector<double> X = pair.second;
            std::vector<double> Y = solution(pair.first);

            if (!std::equal(X.begin(), X.end(), Y.begin(), [](double x, double y) {
                return Comparator::isEqual(std::round(x * 1e3) / 1e3, std::round(y * 1e3) / 1e3);
            })) throw std::logic_error("RK4SystemIntegratorTest: Solving failed");
        }
    }

    try { RK4SystemIntegrator([](double x, const std::vector<double>& y) -> std::vector<double> { return {1}; }, 1, {1, 1}, 1, 1); }
    catch (...) { return; }

    throw std::logic_error("RK4SystemIntegratorTest: Validation failed");
}

#endif //ZADATAK_3_DIFFERENTIALEQUATIONSTEST_H
