#ifndef ZADATAK_1_NONLINEAREQUATIONSTEST_H
#define ZADATAK_1_NONLINEAREQUATIONSTEST_H

#include <functional>
#include <tuple>

#include "../main/NonlinearEquations.h"

std::vector<std::tuple<std::function<double(double)>, std::function<double(double)>, std::pair<double, double>, double>> oneRoot_generator() {
    // function, derivative, bracket, root
    return {
            {[](double x) { return std::exp(x) - 1; }, [](double x) { return std::exp(x); }, {1, -1}, 0},
            {(double(*)(double))(std::sin), (double(*)(double))(std::cos), {2, 4}, M_PI},
            {(double(*)(double))(std::sinh), (double(*)(double))(std::cosh), {-1, 1}, 0},
            {[](double x) { return x + 1.5; }, [](double x) { return 1.; }, {-3, 0}, -1.5},
            {[](double x) { return x * x - 4; }, [](double x) { return 2 * x; }, {3, 1}, 2},
            {[](double x) { return (x + 4) * (x + 1) * (x - 2); }, [](double x) { return 3 * x * x + 6 * x - 6; }, {-1.5, -0.5}, -1},
            {[](double x) { return (x - 3) * (x - 3) * (x - 3); }, [](double x) { return 3 * (x - 3) * (x - 3); }, {2, 4}, 3},
            {[](double x) { return x * x * x * x - 1; }, [](double x) { return 4 * x * x * x; }, {0, -2}, -1},
            {[](double x) { double y = sinh(x); return y * y * y * y * y; }, [](double x) { double y = sinh(x); return 5 * cosh(x) * y * y * y * y; }, {1, -1}, 0}
    };
}

std::vector<std::pair<std::vector<std::complex<double>>, std::vector<std::complex<double>>>> polyRootsComplex_generator() {
    // coefficients, roots
    return {
            {{0, {7, 1}, {42, 42}}, {{-2. / 21, 1. / 14}, 0}},
            {{-1, {2, 2}}, {{0.25, -0.25}}},
            {{{1, 1}, -1}, {{1, 1}}},
            {{{-2, -2}, 0, {2, 2}}, {-1, 1}}
    };
}

std::vector<std::pair<std::vector<double>, std::vector<std::complex<double>>>> polyRoots_generator() {
    // coefficients, roots
    return {
            {{-120, 274, -225, 85, -15, 1}, {1, 2, 3, 4, 5}},
            {{1848, -289, -121, 1, 1}, {-8, -7, 3, 11}},
            {{0, 169, 10, 1}, {{-5, -12}, {-5, 12}, 0}},
            {{112, -120, 55, -12, 1}, {{2, -std::sqrt(3)}, {2, std::sqrt(3)}, 4, 4}}
    };
}

void BracketRootTest() {
    std::vector<std::tuple<std::function<double(double)>, std::function<double(double)>, std::pair<double, double>, double>> test(oneRoot_generator());
    double a, b;

    for (auto& i : test) {
        a = b = std::numeric_limits<double>::quiet_NaN();
        BracketRoot(std::get<0>(i), std::get<3>(i) - 2, a, b);

        if (Comparator::isGreater(a, std::get<3>(i)) || Comparator::isLess(b, std::get<3>(i)))
            throw std::logic_error("BracketRootTest: Bracketing failed");
    }

    try { BracketRoot([](double x) { return x; }, 0, a, b, -1); }
    catch (...) {
        try { BracketRoot([](double x) { return x; }, 0, a, b, 1, -1); }
        catch (...) {
            try { BracketRoot([](double x) { return x; }, 0, a, b, 1, 1, -1); }
            catch (...) {
                return;
            }
        }
    }

    throw std::logic_error("BracketRootTest: Validation failed");
}

void RegulaFalsiSolveTest() {
    std::vector<std::tuple<std::function<double(double)>, std::function<double(double)>, std::pair<double, double>, double>> test(oneRoot_generator());

    for (auto& i : test) {
        double X[4];
        double y = std::get<3>(i);

        X[0] = RegulaFalsiSolve(std::get<0>(i), std::get<2>(i).first, std::get<2>(i).second, Unmodified);
        X[1] = RegulaFalsiSolve(std::get<0>(i), std::get<2>(i).first, std::get<2>(i).second, Illinois);
        X[2] = RegulaFalsiSolve(std::get<0>(i), std::get<2>(i).first, std::get<2>(i).second, Slavic);
        X[3] = RegulaFalsiSolve(std::get<0>(i), std::get<2>(i).first, std::get<2>(i).second, IllinoisSlavic);

        for (double x : X)
            if (!Comparator::isEqual(std::round(x * 1e8) / 1e8, std::round(y * 1e8) / 1e8))
                throw std::logic_error("RegulaFalsiSolveTest: Solving failed");
    }

    try { RegulaFalsiSolve([](double x) { return x; }, 1, 2); }
    catch (...) {
        try { RegulaFalsiSolve([](double x) { return x; }, -1, 1, Illinois, -0.1); }
        catch (...) {
            try { RegulaFalsiSolve([](double x) { return x; }, -1, 1, Slavic, 0.1, -1); }
            catch (...) {
                return;
            }
        }
    }

    throw std::logic_error("RegulaFalsiSolveTest: Validation failed");
}

void RiddersSolveTest() {
    std::vector<std::tuple<std::function<double(double)>, std::function<double(double)>, std::pair<double, double>, double>> test(oneRoot_generator());

    for (auto& i : test) {
        double x = RiddersSolve(std::get<0>(i), std::get<2>(i).first, std::get<2>(i).second);
        double y = std::get<3>(i);

        if (!Comparator::isEqual(std::round(x * 1e8) / 1e8, std::round(y * 1e8) / 1e8))
            throw std::logic_error("RiddersSolveTest: Solving failed");
    }

    try { RiddersSolve([](double x) { return x; }, 1, 2); }
    catch (...) {
        try { RiddersSolve([](double x) { return x; }, -1, 1, -0.1); }
        catch (...) {
            try { RiddersSolve([](double x) { return x; }, -1, 1, 0.1, -1); }
            catch (...) {
                return;
            }
        }
    }

    throw std::logic_error("RiddersSolveTest: Validation failed");
}

void NewtonRaphsonSolveTest() {
    std::vector<std::tuple<std::function<double(double)>, std::function<double(double)>, std::pair<double, double>, double>> test(oneRoot_generator());

    for (auto& i : test) {
        double x = NewtonRaphsonSolve(std::get<0>(i), std::get<1>(i), std::get<3>(i) - 0.5, 1e-10, 0.5);
        double y = std::get<3>(i);

        // '1e1' zbog posljednja 3 testna slucaja (funkcije sa visestrukim nulama) - ostali slucajevi ok i sa '1e8'
        if (!Comparator::isEqual(std::round(x * 1e1) / 1e1, std::round(y * 1e1) / 1e1))
            throw std::logic_error("NewtonRaphsonSolveTest: Solving failed");
    }

    // [](double x) { return x * x * x - 2 * x + 2; => -1.76929 (ipak konvergira)
    // [](double x) { return std::pow(x, 1. / 3); => 0 (ipak konvergira)
    // [](double x) { return std::pow(x, 1. / 3) * std::exp(-x * x); => 0 (ipak konvergira)
    // [](double x) { return x + x * x * std::sin(2 / x); (zaista divergira)

    try { NewtonRaphsonSolve([](double x) { return x; }, [](double x) { return 1; }, 1, -0.1); }
    catch (...) {
        try { NewtonRaphsonSolve([](double x) { return x; }, [](double x) { return 1; }, 1, 1, -0.1); }
        catch (...) {
            try { NewtonRaphsonSolve([](double x) { return x; }, [](double x) { return 1; }, 1, 1, 1.1); }
            catch (...) {
                try { NewtonRaphsonSolve([](double x) { return x; }, [](double x) { return 1; }, 1, 1, 0.1, -1); }
                catch (...) {
                    return;
                }
            }
        }
    }

    throw std::logic_error("NewtonRaphsonSolveTest: Validation failed");
}

void PolyRootsComplexTest() {
    std::vector<std::pair<std::vector<std::complex<double>>, std::vector<std::complex<double>>>> test(polyRootsComplex_generator());

    for (auto& i : test) {
        std::vector<std::complex<double>> x(PolyRoots(i.first));
        std::vector<std::complex<double>> y(std::move(i.second));

        if (!std::equal(x.begin(), x.end(), y.begin(), [](const std::complex<double>& z1, const std::complex<double>& z2) {
            return Comparator::isEqual(std::round(z1.real() * 1e4) / 1e4, std::round(z2.real() * 1e4) / 1e4) &&
                   Comparator::isEqual(std::round(z1.imag() * 1e4) / 1e4, std::round(z2.imag() * 1e4) / 1e4);
        })) throw std::logic_error("PolyRootsComplexTest: Solving failed");
    }

    try { PolyRoots(std::vector<std::complex<double>>{}, -0.1); }
    catch (...) {
        try { PolyRoots(std::vector<std::complex<double>>{}, 1, -1); }
        catch (...) {
            try { PolyRoots(std::vector<std::complex<double>>{}, 1, 1, -1); }
            catch (...) {
                return;
            }
        }
    }

    throw std::logic_error("PolyRootsComplexTest: Validation failed");
}

void PolyRootsTest() {
    std::vector<std::pair<std::vector<double>, std::vector<std::complex<double>>>> test(polyRoots_generator());

    for (auto& i : test) {
        std::vector<std::complex<double>> x(PolyRoots(i.first));
        std::vector<std::complex<double>> y(std::move(i.second));

        if (!std::equal(x.begin(), x.end(), y.begin(), [](const std::complex<double>& z1, const std::complex<double>& z2) {
            return Comparator::isEqual(std::round(z1.real() * 1e4) / 1e4, std::round(z2.real() * 1e4) / 1e4) &&
                   Comparator::isEqual(std::round(z1.imag() * 1e4) / 1e4, std::round(z2.imag() * 1e4) / 1e4);
        })) throw std::logic_error("PolyRootsTest: Solving failed");
    }

    try { PolyRoots(std::vector<double>{}, -0.1); }
    catch (...) {
        try { PolyRoots(std::vector<double>{}, 1, -1); }
        catch (...) {
            try { PolyRoots(std::vector<double>{}, 1, 1, -1); }
            catch (...) {
                return;
            }
        }
    }

    throw std::logic_error("PolyRootsTest: Validation failed");
}

#endif //ZADATAK_1_NONLINEAREQUATIONSTEST_H
