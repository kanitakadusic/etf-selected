#ifndef ZADATAK_2_INTEGRATION_H
#define ZADATAK_2_INTEGRATION_H

#include <stdexcept>
#include <vector>
#include <cmath>
#include <limits>

template <typename F>
std::pair<double, bool> RombergIntegration(F f, double a, double b, double eps = 1e-8, int N_max = 1e6, int N_min = 50) {
    if (eps < 0. || N_max < 0 || N_min < 0 || N_max < N_min) throw std::domain_error("Bad parameter");

    constexpr int M_max = 30;
    int N_div = 2;
    double h = (b - a) / N_div, s = (f(a) + f(b)) / 2., I_old = s;
    std::vector<double> I(M_max);

    for (int i = 0; i < M_max && N_div <= N_max; i++) {
        for (int j = 1; j <= N_div / 2; j++)
            s += f(a + (2 * j - 1) * h);

        I[i] = h * s;
        double p = 4.;

        for (int k = i; k > 0; k--) {
            I[k - 1] = (p * I[k] - I[k - 1]) / (p - 1);
            p *= 4;
        }

        if (N_div >= N_min && std::fabs(I[0] - I_old) <= eps)
            return {I[0], true};

        I_old = I[0];
        h /= 2;
        N_div *= 2;
    }

    return {I[0], false};
}

template <typename F>
std::pair<double, bool> TanhSinhIntegration(F f, double a, double b, double eps = 1e-8, int N_max = 1e6, int N_min = 20, double R = 3.5) {
    if (eps < 0. || N_max < 0 || N_min < 0 || N_max < N_min || R < 0.) throw std::domain_error("Bad parameter");

    int N_div = 2;
    double h = 2 * R / N_div, p = (b + a) / 2., q = (b - a) / 2., s = 0., I_old = s;

    while (N_div < N_max) {
        for (int i = 1; i <= N_div / 2; i++) {
            double t = -R + (2 * i - 1) * h, u = M_PI * std::sinh(t) / 2., v = f(p + q * std::tanh(u));

            if (std::isfinite(v))
                s += q * M_PI * std::cosh(t) * v / (2. * std::cosh(u) * std::cosh(u));
        }

        double I = h * s;

        if (N_div >= N_min && std::fabs(I - I_old) <= eps)
            return {I, true};

        I_old = I;
        h /= 2;
        N_div *= 2;
    }

    return {I_old, false};
}

bool NaN(double x) {
    return std::fabs(x) == std::numeric_limits<double>::infinity() || !std::isfinite(x);
}

template <typename F>
double AdaptiveSimpson(F f, double a, double b, double eps, double f1, double f2, double f3, int D_max, bool& correct) {
    double c = (a + b) / 2.;

    double f4 = f((a + c) / 2.); if (NaN(f4)) f4 = 0.;
    double f5 = f((c + b) / 2.); if (NaN(f5)) f5 = 0.;

    double I1 = (b - a) * (f1 + 4. * f3 + f2) / 6.;
    double I2 = (b - a) * (f1 + 4. * f4 + 2. * f3 + 4. * f5 + f2) / 12.;

    if (std::fabs(I1 - I2) <= eps) return I2;
    else if (D_max <= 0) { correct = false; return I2; }

    return AdaptiveSimpson(f, a, c, eps, f1, f3, f4, D_max - 1, correct) + AdaptiveSimpson(f, c, b, eps, f3, f2, f5, D_max - 1, correct);
}

template <typename F>
std::pair<double, bool> AdaptiveIntegration(F f, double a, double b, double eps = 1e-10, int D_max = 30, int N_min = 1) {
    if (eps < 0. || D_max < 0 || N_min < 0) throw std::domain_error("Bad parameter");

    bool correct = true;
    double s = 0., h = (b - a) / N_min;

    for (int i = 0; i < N_min; i++) {
        double f1 = f(a); if (NaN(f1)) f1 = 0.;
        double f2 = f(a + h); if (NaN(f2)) f2 = 0.;
        double f3 = f(a + h / 2); if (NaN(f3)) f3 = 0.;

        s += AdaptiveSimpson(f, a, a + h, eps, f1, f2, f3, D_max, correct);
        a += h;
    }

    return {s, correct};
}

#endif //ZADATAK_2_INTEGRATION_H
