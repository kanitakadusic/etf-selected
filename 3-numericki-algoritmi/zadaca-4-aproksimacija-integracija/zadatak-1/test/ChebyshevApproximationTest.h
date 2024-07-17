#ifndef ZADATAK_1_CHEBYSHEVAPPROXIMATIONTEST_H
#define ZADATAK_1_CHEBYSHEVAPPROXIMATIONTEST_H

#include "../main/ChebyshevApproximation.h"

class ChebyshevApproximationTest {
public:
    static void constructorTest() {
        try { ChebyshevApproximation((double(*)(double))(std::sin), M_PI, 0, 5); }
        catch (...) {
            try { ChebyshevApproximation((double(*)(double))(std::sin), 0, M_PI, 0); }
            catch (...) { return; }
        }

        throw std::logic_error("ChebyshevApproximationTest: Constructor failed");
    }

    static void setMTest() {
        ChebyshevApproximation a((double(*)(double))(std::sin), 0, M_PI, 5);

        try { a.set_m(0); }
        catch (...) {
            try { a.set_m(6); }
            catch (...) { return; }
        }

        throw std::logic_error("ChebyshevApproximationTest: SetM failed");
    }

    static void truncTest() {
        ChebyshevApproximation a((double(*)(double))(std::sin), 0, M_PI, 5);

        try { a.trunc(-1); }
        catch (...) {
            try { a.trunc(100); }
            catch (...) { return; }
        }

        throw std::logic_error("ChebyshevApproximationTest: Trunc failed");
    }

    static void approximationTest() {
        ChebyshevApproximation a((double(*)(double))(std::sin), 0, M_PI, 10);

        std::vector<double> x{a(0), a(M_PI / 4), a(M_PI / 2), a(3 * M_PI / 4), a(M_PI), a(1), a(2), a(3)};
        std::vector<double> y{0, std::sin(M_PI / 4), 1, std::sin(3 * M_PI / 4), 0, std::sin(1), std::sin(2), std::sin(3)};

        if (!std::equal(x.begin(), x.end(), y.begin(), [](double x, double y) {
            return Comparator::isEqual(std::round(x * 1e9) / 1e9, std::round(y * 1e9) / 1e9);
        })) throw std::logic_error("ChebyshevApproximationTest: Approximation failed");

        try { a(-M_PI); }
        catch (...) { return; }

        throw std::logic_error("ChebyshevApproximationTest: Approximation failed");
    }

    static void derivativeDoubleTest() {
        ChebyshevApproximation a((double(*)(double))(std::sin), 0, M_PI, 10);

        std::vector<double> x{a.derivative(0), a.derivative(M_PI / 4), a.derivative(M_PI / 2), a.derivative(3 * M_PI / 4), a.derivative(M_PI), a.derivative(1), a.derivative(2), a.derivative(3)};
        std::vector<double> y{1, std::cos(M_PI / 4), 0, std::cos(3 * M_PI / 4), -1, std::cos(1), std::cos(2), std::cos(3)};

        if (!std::equal(x.begin(), x.end(), y.begin(), [](double x, double y) {
            return Comparator::isEqual(std::round(x * 1e7) / 1e7, std::round(y * 1e7) / 1e7);
        })) throw std::logic_error("ChebyshevApproximationTest: DerivativeDouble failed");

        try { a.derivative(-M_PI); }
        catch (...) { return; }

        throw std::logic_error("ChebyshevApproximationTest: DerivativeDouble failed");
    }

    static void derivativeTest() {
        ChebyshevApproximation a = ChebyshevApproximation((double(*)(double))(std::sin), 0, M_PI, 10).derivative().derivative();

        {
            std::vector<double> x{a(0), a(M_PI / 4), a(M_PI / 2), a(3 * M_PI / 4), a(M_PI), a(1), a(2), a(3)};
            std::vector<double> y{0, -std::sin(M_PI / 4), -1, -std::sin(3 * M_PI / 4), 0, -std::sin(1), -std::sin(2), -std::sin(3)};

            if (!std::equal(x.begin(), x.end(), y.begin(), [](double x, double y) {
                return Comparator::isEqual(std::round(x * 1e5) / 1e5, std::round(y * 1e5) / 1e5);
            })) throw std::logic_error("ChebyshevApproximationTest: Derivative failed");
        }

        a = a.derivative();

        {
            std::vector<double> x{a(0), a(M_PI / 4), a(M_PI / 2), a(3 * M_PI / 4), a(M_PI), a(1), a(2), a(3)};
            std::vector<double> y{-1, -std::cos(M_PI / 4), 0, -std::cos(3 * M_PI / 4), 1, -std::cos(1), -std::cos(2), -std::cos(3)};

            if (!std::equal(x.begin(), x.end(), y.begin(), [](double x, double y) {
                return Comparator::isEqual(std::round(x * 1e4) / 1e4, std::round(y * 1e4) / 1e4);
            })) throw std::logic_error("ChebyshevApproximationTest: Derivative failed");
        }
    }

    static void antiderivativeTest() {
        ChebyshevApproximation a = ChebyshevApproximation((double(*)(double))(std::sin), 0, M_PI, 10).antiderivative();

        std::vector<double> x{a(0), a(M_PI / 4), a(M_PI / 2), a(3 * M_PI / 4), a(M_PI), a(1), a(2), a(3)};
        std::vector<double> y{-1, -std::cos(M_PI / 4), 0, -std::cos(3 * M_PI / 4), 1, -std::cos(1), -std::cos(2), -std::cos(3)};

        if (!std::equal(x.begin(), x.end(), y.begin(), [](double x, double y) {
            return Comparator::isEqual(std::round(x * 1e9) / 1e9, std::round(y * 1e9) / 1e9);
        })) throw std::logic_error("ChebyshevApproximationTest: Antiderivative failed");
    }

    static void integrateDoubleDoubleTest() {
        ChebyshevApproximation a((double(*)(double))(std::sin), 0, M_PI, 10);

        std::vector<double> x{a.integrate(0, M_PI / 2), a.integrate(M_PI / 2, M_PI / 4), a.integrate(M_PI / 4, 3 * M_PI / 4), a.integrate(M_PI, M_PI / 2), a.integrate(1, 2), a.integrate(3, 2)};
        std::vector<double> y{1, -1 / std::sqrt(2), std::sqrt(2), -1, std::cos(1) - std::cos(2), std::cos(3) - std::cos(2)};

        if (!std::equal(x.begin(), x.end(), y.begin(), [](double x, double y) {
            return Comparator::isEqual(std::round(x * 1e9) / 1e9, std::round(y * 1e9) / 1e9);
        })) throw std::logic_error("ChebyshevApproximationTest: IntegrateDoubleDouble failed");

        try { a.integrate(-M_PI, M_PI / 2); }
        catch (...) {
            try { a.integrate(M_PI / 2, 2 * M_PI); }
            catch (...) { return; }
        }

        throw std::logic_error("ChebyshevApproximationTest: IntegrateDoubleDouble failed");
    }

    static void integrateTest() {
        std::vector<double> x;
        std::vector<double> y{2, 1, 1 / std::sqrt(2), std::sqrt(2), 1, std::cos(1) - std::cos(2), std::cos(2) - std::cos(3)};

        x.push_back(ChebyshevApproximation((double(*)(double))(std::sin), 0, M_PI, 10).integrate());
        x.push_back(ChebyshevApproximation((double(*)(double))(std::sin), 0, M_PI / 2, 10).integrate());
        x.push_back(ChebyshevApproximation((double(*)(double))(std::sin), M_PI / 4, M_PI / 2, 10).integrate());
        x.push_back(ChebyshevApproximation((double(*)(double))(std::sin), M_PI / 4, 3 * M_PI / 4, 10).integrate());
        x.push_back(ChebyshevApproximation((double(*)(double))(std::sin), M_PI / 2, M_PI, 10).integrate());
        x.push_back(ChebyshevApproximation((double(*)(double))(std::sin), 1, 2, 10).integrate());
        x.push_back(ChebyshevApproximation((double(*)(double))(std::sin), 2, 3, 10).integrate());

        if (!std::equal(x.begin(), x.end(), y.begin(), [](double x, double y) {
            return Comparator::isEqual(std::round(x * 1e9) / 1e9, std::round(y * 1e9) / 1e9);
        })) throw std::logic_error("ChebyshevApproximationTest: Integrate failed");
    }
};

#endif //ZADATAK_1_CHEBYSHEVAPPROXIMATIONTEST_H
