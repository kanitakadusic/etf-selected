#ifndef ZADATAK_1_CHEBYSHEVAPPROXIMATION_H
#define ZADATAK_1_CHEBYSHEVAPPROXIMATION_H

#include <stdexcept>
#include <vector>

#include "Comparator.h"

class ChebyshevApproximation {
    std::vector<double> coeffs;
    double xMin, xMax;
    int degree;

    ChebyshevApproximation(const std::vector<double>& coeffs, double xMin, double xMax): coeffs(coeffs), xMin(xMin), xMax(xMax), degree(coeffs.size() - 1) {}

public:
    template <typename F>
    ChebyshevApproximation(F, double, double, int);

    void set_m(int);
    void trunc(double);

    double operator ()(double) const;

    double derivative(double) const;
    ChebyshevApproximation derivative() const;
    ChebyshevApproximation antiderivative() const;

    double integrate(double, double) const;
    double integrate() const;
};

template <typename F>
ChebyshevApproximation::ChebyshevApproximation(F function, double xMin, double xMax, int sampleSize): coeffs(sampleSize + 1), xMin(xMin), xMax(xMax), degree(sampleSize) {
    if (Comparator::isGreaterEqual(xMin, xMax) || sampleSize < 1) throw std::domain_error("Bad parameters");

    std::vector<double> w(sampleSize + 2);
    for (int i = 0; i < sampleSize + 2; i++)
        w[i] = std::cos(M_PI * i / (2 * sampleSize + 2));

    std::vector<double> v(sampleSize + 2);
    for (int i = 0; i <= sampleSize / 2; i++)
        v[i] = function((xMin + xMax + (xMax - xMin) * w[2 * i + 1]) / 2.);
    for (int i = sampleSize / 2 + 1; i <= sampleSize; i++)
        v[i] = function((xMin + xMax - (xMax - xMin) * w[2 * sampleSize + 1 - 2 * i]) / 2.);

    for (int k = 0; k <= sampleSize; k++) {
        double s = 0.;

        for (int i = 0; i <= sampleSize; i++) {
            int p = (k * (2 * i + 1)) % (4 * sampleSize + 4);
            if (p > 2 * sampleSize + 2) p = 4 * sampleSize + 4 - p;

            if (p > sampleSize + 1) s -= v[i] * w[2 * sampleSize + 2 - p];
            else s += v[i] * w[p];
        }

        coeffs[k] = 2. * s / (sampleSize + 1);
    }
}

void ChebyshevApproximation::set_m(int polynomialDegree) {
    if (polynomialDegree < 2 || polynomialDegree > coeffs.size() - 1) throw std::domain_error("Bad order");
    degree = polynomialDegree;
}

void ChebyshevApproximation::trunc(double epsilon) {
    if (epsilon < 0.) throw std::domain_error("Bad tolerance");

    int newDegree = 0;
    for (int i = 0; i < coeffs.size(); i++)
        if (coeffs[i] >= epsilon) newDegree = i;

    if (newDegree < 1) throw std::domain_error("Bad tolerance");
    degree = newDegree;
}

double ChebyshevApproximation::operator ()(double x) const {
    if (Comparator::isNotBetween(x, xMin, xMax)) throw std::domain_error("Bad argument");

    double t = (2. * x - xMin - xMax) / (xMax - xMin), p = 0., q = coeffs[degree];
    for (int k = degree - 1; k > 0; k--) {
        double r = 2. * t * q - p + coeffs[k];
        p = q;
        q = r;
    }

    return t * q - p + coeffs[0] / 2.;
}

double ChebyshevApproximation::derivative(double x) const {
    if (Comparator::isNotBetween(x, xMin, xMax)) throw std::domain_error("Bad argument");

    double t = (2. * x - xMin - xMax) / (xMax - xMin), p = 1., q = 4. * t, s = coeffs[1] + q * coeffs[2];
    for (int k = 3; k <= degree; k++) {
        double r = k * (2 * t * q / (k - 1) - p / (k - 2));
        s += coeffs[k] * r;
        p = q;
        q = r;
    }

    return 2. * s / (xMax - xMin);
}

ChebyshevApproximation ChebyshevApproximation::derivative() const {
    std::vector<double> derivCoeffs(degree);
    double u = 4. / (xMax - xMin);

    derivCoeffs[degree - 1] = u * degree * coeffs[degree];
    derivCoeffs[degree - 2] = u * (degree - 1) * coeffs[degree - 1];

    for (int k = degree - 2; k > 0; k--)
        derivCoeffs[k - 1] = derivCoeffs[k + 1] + u * k * coeffs[k];

    return {derivCoeffs, xMin, xMax};
}

ChebyshevApproximation ChebyshevApproximation::antiderivative() const {
    std::vector<double> primCoeffs(coeffs.size() + 1);
    for (int k = 1; k < degree; k++)
        primCoeffs[k] = (xMax - xMin) / (4 * k) * (coeffs[k - 1] - coeffs[k + 1]);

    primCoeffs[0] = 0.;
    primCoeffs[degree] = (xMax - xMin) / (4 * degree) * coeffs[degree - 1];
    primCoeffs[degree + 1] = (xMax - xMin) / (4 * degree + 4) * coeffs[degree];

    return {primCoeffs, xMin, xMax};
}

double ChebyshevApproximation::integrate(double a, double b) const {
    if (Comparator::isNotBetween(a, xMin, xMax) || Comparator::isNotBetween(b, xMin, xMax)) throw std::domain_error("Bad interval");

    ChebyshevApproximation F(antiderivative());
    return F(b) - F(a);
}

double ChebyshevApproximation::integrate() const {
    ChebyshevApproximation F(antiderivative());
    return F(xMax) - F(xMin);
}

#endif //ZADATAK_1_CHEBYSHEVAPPROXIMATION_H
