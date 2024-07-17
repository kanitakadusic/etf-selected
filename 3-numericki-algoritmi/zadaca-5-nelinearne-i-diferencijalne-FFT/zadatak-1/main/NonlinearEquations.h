#ifndef ZADATAK_1_NONLINEAREQUATIONS_H
#define ZADATAK_1_NONLINEAREQUATIONS_H

#include <stdexcept>
#include <complex>
#include <vector>
#include <algorithm>
#include <random>

#include "Comparator.h"

template <typename F>
bool OneWayBracketRoot(F f, double x0, double& a, double& b, double h, double hMax, double lambda) {
    a = x0;
    double f1 = f(a);

    while (Comparator::isLess(std::fabs(h), hMax)) {
        b = a + h;
        double f2 = f(b);

        while (!std::isfinite(f2)) {
            h /= 2 * (1 + lambda);

            if (Comparator::isLessEqual(std::fabs(h), std::fabs(a) * std::numeric_limits<double>::epsilon()))
                return false;

            b = a + h;
            f2 = f(b);
        }

        if (Comparator::isLessEqual(f1 * f2, 0))
            return true;

        h *= lambda;
        a = b;
        f1 = f2;
    }

    return false;
}

template <typename F>
bool BracketRoot(F f, double x0, double& a, double& b, double h = 1e-5, double hMax = 1e10, double lambda = 1.4) {
    if (h < 0 || hMax < 0 || lambda < 0) throw std::domain_error("Invalid parameters");

    if (!OneWayBracketRoot(f, x0, a, b, h, hMax, lambda)) {
        if (!OneWayBracketRoot(f, x0, a, b, -h, hMax, lambda)) return false;
        std::swap(a, b);
    }

    return true;
}

enum RegulaFalsiMode { Unmodified, Illinois, Slavic, IllinoisSlavic };

template <typename F>
double UnmodifiedRegulaFalsi(F f, double a, double b, double epsilon, int maxIterations) {
    double f1 = f(a), f2 = f(b), c = a, cOld = b;

    while (--maxIterations && std::fabs(c - cOld) > epsilon) {
        cOld = c;
        c = (a * f2 - b * f1) / (f2 - f1);

        double f3 = f(c);
        if (Comparator::isEqual(f3, 0)) return c;

        if (Comparator::isLess(f1 * f3, 0)) { b = c; f2 = f3; }
        else { a = c; f1 = f3; }
    }

    if (maxIterations == 0) throw std::logic_error("Given accuracy has not achieved");
    return c;
}

template <typename F>
double IllinoisRegulaFalsi(F f, double a, double b, double epsilon, int maxIterations) {
    double f1 = f(a), f2 = f(b), c = a, cOld = b;

    while (--maxIterations && std::fabs(c - cOld) > epsilon) {
        cOld = c;
        c = (a * f2 - b * f1) / (f2 - f1);

        double f3 = f(c);
        if (Comparator::isEqual(f3, 0)) return c;

        if (Comparator::isLess(f1 * f3, 0)) { b = a; f2 = f1; }
        else f2 /= 2;

        a = c;
        f1 = f3;
    }

    if (maxIterations == 0) throw std::logic_error("Given accuracy has not achieved");
    return c;
}

template <typename F>
double SlavicRegulaFalsi(F f, double a, double b, double epsilon, int maxIterations) {
    return UnmodifiedRegulaFalsi([f](double x) {
        double y = f(x);
        return y / (1 + std::fabs(y));
    }, a, b, epsilon, maxIterations);
}

template <typename F>
double IllinoisSlavicRegulaFalsi(F f, double a, double b, double epsilon, int maxIterations) {
    return IllinoisRegulaFalsi([f](double x) {
        double y = f(x);
        return y / (1 + std::fabs(y));
    }, a, b, epsilon, maxIterations);
}

template <typename F>
double RegulaFalsiSolve(F f, double a, double b, RegulaFalsiMode mode = Slavic, double epsilon = 1e-10, int maxIterations = 100) {
    if (f(a) * f(b) > 0) throw std::range_error("Root must be bracketed");
    if (epsilon < 0 || maxIterations < 0) throw std::domain_error("Invalid parameters");

    if (Comparator::isGreater(a, b)) std::swap(a, b);
    switch (mode) {
        case Slavic: return SlavicRegulaFalsi(f, a, b, epsilon, maxIterations);
        case Unmodified: return UnmodifiedRegulaFalsi(f, a, b, epsilon, maxIterations);
        case Illinois: return IllinoisRegulaFalsi(f, a, b, epsilon, maxIterations);
        case IllinoisSlavic: return IllinoisSlavicRegulaFalsi(f, a, b, epsilon, maxIterations);
    }

    throw std::logic_error("Error occurred");
}

int Sgn(double x) {
    return x < 0 ? -1 : (x > 0 ? 1 : 0);
}

template <typename F>
double RiddersSolve(F f, double a, double b, double epsilon = 1e-10, int maxIterations = 100) {
    if (f(a) * f(b) > 0) throw std::range_error("Root must be bracketed");
    if (epsilon < 0 || maxIterations < 0) throw std::domain_error("Invalid parameters");

    if (Comparator::isGreater(a, b)) std::swap(a, b);
    double f1 = f(a), f2 = f(b);

    while (--maxIterations && std::fabs(b - a) > epsilon) {
        double c = (a + b) / 2, f3 = f(c);
        if (Comparator::isEqual(f3, 0)) return c;

        double d = c + f3 * (c - a) * Sgn((f1 - f2)) / std::sqrt(f3 * f3 - f1 * f2), f4 = f(d);
        if (Comparator::isEqual(f4, 0)) return d;

        if (Comparator::isLessEqual(f3 * f4, 0)) { a = c; b = d; f1 = f3; f2 = f4; }
        else if (Comparator::isLessEqual(f1 * f4, 0)) { b = d; f2 = f4; }
        else { a = d; f1 = f4; }
    }

    if (maxIterations == 0) throw std::logic_error("Given accuracy has not achieved");
    return (a + b) / 2;
}

template <typename F1, typename F2>
double NewtonRaphsonSolve(F1 f, F2 fPrim, double x0, double epsilon = 1e-10, double damping = 0, int maxIterations = 100) {
    if (epsilon < 0 || damping < 0 || Comparator::isGreaterEqual(damping, 1) || maxIterations < 0) throw std::domain_error("Invalid parameters");

    double deltaX = std::numeric_limits<double>::infinity();
    double v = f(x0), d = fPrim(x0);
    bool noDamping = Comparator::isEqual(damping, 0);

    while (--maxIterations && std::fabs(deltaX) > epsilon) {
        if (std::fabs(v) <= epsilon) return x0;

        deltaX = v / d;
        double w = v;
        v = f(x0 - deltaX);
        d = fPrim(x0 - deltaX);

        if (noDamping) {
            if (!std::isfinite(v) || Comparator::isEqual(d, 0))
                throw std::logic_error("Convergence has not achieved");
        } else {
            while (Comparator::isGreater(std::fabs(v), std::fabs(w)) || !std::isfinite(v) || Comparator::isEqual(d, 0)) {
                deltaX *= damping;
                v = f(x0 - deltaX);
                d = fPrim(x0 - deltaX);
            }
        }

        x0 -= deltaX;
    }

    if (maxIterations == 0) throw std::logic_error("Convergence has not achieved");
    return x0;
}

std::complex<double> operator *(double s, std::complex<double> c) {
    return {s * c.real(), s * c.imag()};
}

template <typename T>
std::pair<std::complex<double>, bool> LaguerreRoot(const std::vector<T>& coeffs, int order, std::complex<double> x0, double epsilon, int maxIterations) {
    std::complex<double> deltaX = std::numeric_limits<double>::infinity();

    while (std::fabs(deltaX) > epsilon && --maxIterations) {
        std::complex<double> f = coeffs[order], d = 0, s = 0;

        for (int i = order - 1; i > -1; i--) {
            s = s * x0 + 2 * d;
            d = d * x0 + f;
            f = f * x0 + coeffs[i];
        }

        if (std::fabs(f) <= epsilon) return {x0, true};

        std::complex<double> r = std::sqrt((order - 1) * ((order - 1) * d * d - order * f * s));
        if (Comparator::isGreater(std::fabs(d + r), std::fabs(d - r))) deltaX = order * f / (d + r);
        else deltaX = order * f / (d - r);

        x0 -= deltaX;
    }

    if (maxIterations != 0 && std::fabs(deltaX) <= epsilon) return {x0, true};
    return {x0, false};
}

double Random() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(-10.0, 10.0);

    return dist(mt);
}

std::vector<std::complex<double>> PolyRoots(const std::vector<std::complex<double>>& coeffs, double epsilon = 1e-10, int maxIterations = 100, int maxTrials = 10) {
    if (epsilon < 0 || maxIterations < 0 || maxTrials < 0) throw std::domain_error("Invalid parameters");

    std::vector<std::complex<double>> auxCoeffs(coeffs), roots;

    for (int i = auxCoeffs.size() - 1; i > 0; i--) {
        std::pair<std::complex<double>, bool> rootCorrect(0, false);

        for (int t = 1; !rootCorrect.second && t < maxTrials; t++)
            rootCorrect = LaguerreRoot(auxCoeffs, i, {Random(), Random()}, epsilon, maxIterations);

        if (!rootCorrect.second) throw std::logic_error("Convergence has not achieved");

        if (std::fabs(rootCorrect.first.imag()) <= epsilon) rootCorrect.first = rootCorrect.first.real();

        // root polishing
        rootCorrect = LaguerreRoot(coeffs, coeffs.size() - 1, rootCorrect.first, epsilon, maxIterations);
        roots.push_back(rootCorrect.first);

        std::complex<double> v(auxCoeffs[i]);

        for (int j = i - 1; j > -1; j--) {
            std::complex<double> w(auxCoeffs[j]);
            auxCoeffs[j] = v;
            v = w + rootCorrect.first * v;
        }
    }

    std::sort(roots.begin(), roots.end(), [](const std::complex<double>& z1, const std::complex<double>& z2) {
        return Comparator::isLess(z1.real(), z2.real()) || (Comparator::isEqual(z1.real(), z2.real()) && Comparator::isLess(z1.imag(), z2.imag()));
    });

    return roots;
}

std::vector<std::complex<double>> PolyRoots(const std::vector<double>& coeffs, double epsilon = 1e-10, int maxIterations = 100, int maxTrials = 10) {
    if (epsilon < 0 || maxIterations < 0 || maxTrials < 0) throw std::domain_error("Invalid parameters");

    std::vector<double> auxCoeffs(coeffs);
    std::vector<std::complex<double>> roots;

    for (int i = auxCoeffs.size() - 1; i > 0; i--) {
        std::pair<std::complex<double>, bool> rootCorrect(0, false);

        for (int t = 1; !rootCorrect.second && t < maxTrials; t++)
            rootCorrect = LaguerreRoot(auxCoeffs, i, {Random(), Random()}, epsilon, maxIterations);

        if (!rootCorrect.second) throw std::logic_error("Convergence has not achieved");

        if (std::fabs(rootCorrect.first.imag()) <= epsilon) {
            rootCorrect.first = rootCorrect.first.real();

            // root polishing
            rootCorrect = LaguerreRoot(coeffs, coeffs.size() - 1, rootCorrect.first, epsilon, maxIterations);
            roots.push_back(rootCorrect.first);

            double v = auxCoeffs[i];

            for (int j = i - 1; j > -1; j--) {
                double w = auxCoeffs[j];
                auxCoeffs[j] = v;
                v = w + rootCorrect.first.real() * v;
            }
        } else {
            // root polishing
            rootCorrect = LaguerreRoot(coeffs, coeffs.size() - 1, rootCorrect.first, epsilon, maxIterations);
            roots.push_back(rootCorrect.first);
            roots.push_back(std::conj(rootCorrect.first));

            double a = 2 * rootCorrect.first.real(), b = std::abs(rootCorrect.first) * std::abs(rootCorrect.first);
            double u = auxCoeffs[i], v = auxCoeffs[i - 1] + a * u;

            for (int j = i - 2; j > -1; j--) {
                double w = auxCoeffs[j];
                auxCoeffs[j] = u;
                u = v;
                v = w + a * v - b * auxCoeffs[j];
            }

            i--;
        }
    }

    std::sort(roots.begin(), roots.end(), [](const std::complex<double>& z1, const std::complex<double>& z2) {
        return Comparator::isLess(z1.real(), z2.real()) || (Comparator::isEqual(z1.real(), z2.real()) && Comparator::isLess(z1.imag(), z2.imag()));
    });

    return roots;
}

#endif //ZADATAK_1_NONLINEAREQUATIONS_H
