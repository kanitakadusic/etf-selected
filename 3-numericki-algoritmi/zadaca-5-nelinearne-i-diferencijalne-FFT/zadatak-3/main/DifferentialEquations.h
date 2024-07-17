#ifndef ZADATAK_3_DIFFERENTIALEQUATIONS_H
#define ZADATAK_3_DIFFERENTIALEQUATIONS_H

#include <stdexcept>
#include <vector>

#include "Comparator.h"

template <typename F>
double RK4Step(F f, double x, double y, double h) {
    double K1 = f(x, y);
    double K2 = f(x + h / 2, y + h * K1 / 2);
    double K3 = f(x + h / 2, y + h * K2 / 2);
    double K4 = f(x + h, y + h * K3);

    return y + h * (K1 + 2 * K2 + 2 * K3 + K4) / 6;
}

template <typename F>
std::vector<std::pair<double, double>> RK4NonAdaptive(F f, double x0, double y0, double xMax, double h) {
    double x = x0, y = y0;
    std::vector<std::pair<double, double>> samples;

    while (Comparator::isLessEqual(std::fabs(x), std::fabs(xMax))) {
        samples.emplace_back(x, y);
        y = RK4Step(f, x, y, h);
        x += h;
    }

    return samples;
}

template <typename F>
std::vector<std::pair<double, double>> RK4Adaptive(F f, double x0, double y0, double xMax, double h, double epsilon) {
    double x = x0, y = y0;
    std::vector<std::pair<double, double>> samples;

    while (Comparator::isLessEqual(std::fabs(x + h), std::fabs(xMax))) {
        double u = RK4Step(f, x, y, h / 2);
        double v = RK4Step(f, x + h / 2, u, h / 2);
        double w = RK4Step(f, x, y, h);

        double delta = std::fabs(w - v) / std::fabs(h);

        if (delta <= epsilon) {
            x += h;
            y = v;
            samples.emplace_back(x, y);
        }

        double temp = 0.9 * std::pow(epsilon / delta, 1. / 4);
        h = h * (Comparator::isLess(5, temp) ? 5 : temp);
    }

    if (!Comparator::isEqual(samples.back().first, xMax)) {
        h = xMax - x;

        double u = RK4Step(f, x, y, h / 2);
        y = RK4Step(f, x + h / 2, u, h / 2);

        samples.emplace_back(xMax, y);
    }

    return samples;
}

template <typename F>
std::vector<std::pair<double, double>> RK4Integrator(F f, double x0, double y0, double xMax, double h, double epsilon = 1e-8, bool adaptive = false) {
    if (Comparator::isGreaterEqual(std::fabs(x0), std::fabs(xMax))) return {{x0, y0}};

    if (adaptive) return RK4Adaptive(f, x0, y0, xMax, h, epsilon);
    else return RK4NonAdaptive(f, x0, y0, xMax, h);
}

template <typename F>
std::vector<std::pair<double, std::vector<double>>> RK4SystemIntegrator(F f, double x0, const std::vector<double>& y0, double xMax, double h) {
    if (f(x0, y0).size() != y0.size()) throw std::range_error("Incompatible formats");
    if (Comparator::isGreaterEqual(std::fabs(x0), std::fabs(xMax))) return {{x0, y0}};

    double x = x0;
    std::vector<double> y(y0);
    std::vector<std::pair<double, std::vector<double>>> samples;

    int size = y.size();
    std::vector<double> K1(size), K2(size), K3(size), K4(size), u(size), v(size);

    while (Comparator::isLessEqual(std::fabs(x), std::fabs(xMax))) {
        samples.emplace_back(x, y);

        for (int k = 0; k < size; k++) {
            K1[k] = f(x, y)[k];
            u[k] = y[k] + h * K1[k] / 2;
        }

        x += h / 2;

        for (int k = 0; k < size; k++) {
            K2[k] = f(x, u)[k];
            v[k] = y[k] + h * K2[k] / 2;
        }

        for (int k = 0; k < size; k++) {
            K3[k] = f(x, v)[k];
            u[k] = y[k] + h * K3[k];
        }

        x += h / 2;

        for (int k = 0; k < size; k++)
            K4[k] = f(x, u)[k];

        for (int k = 0; k < size; k++)
            y[k] += h * (K1[k] + 2 * K2[k] + 2 * K3[k] + K4[k]) / 6;
    }

    return samples;
}

#endif //ZADATAK_3_DIFFERENTIALEQUATIONS_H
