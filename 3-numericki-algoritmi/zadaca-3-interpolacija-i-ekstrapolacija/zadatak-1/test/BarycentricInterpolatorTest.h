#ifndef ZADATAK_1_BARYCENTRICINTERPOLATORTEST_H
#define ZADATAK_1_BARYCENTRICINTERPOLATORTEST_H

#include "../main/BarycentricInterpolator.h"

class BarycentricInterpolatorTest {
public:
    static void ConstructorTest() {
        try { BarycentricInterpolator({{0, 1}, {2, 3}}, -1); }
        catch (...) {
            try { BarycentricInterpolator({{0, 1}, {2, 3}}, 2); }
            catch (...) { return; }
        }

        throw std::logic_error("BarycentricInterpolatorTest: Constructor failed");
    }

    static void InterpolationTest() {
        std::vector<std::pair<double, double>> points{{0, 0}, {0.5, std::sin(0.5)}, {1, std::sin(1)}, {1.5, std::sin(1.5)}, {M_PI / 2, 1}};
        std::vector<double> x, y{0, 1, 0.124691, 0.827555, 0.0398368, 0.867292, 0, 1, 0.256624, 0.953517, -0.628001, 0.990855, 0, 1, 0.251966, 0.950116, -0.587546, 0.986598, 0, 1, 0.246968, 0.948898, -0.460097, 0.983692, 0, 1, 0.246968, 0.948898, -0.460097, 0.983692};

        // {0, 1, 0.247404, 0.948985, -0.479426, 0.983986} (correct values)
        // y (measured values for different orders)

        for (int i = 0; i < points.size(); i++) {
            BarycentricInterpolator a(points, i);

            std::vector<double> test{a(0), a(M_PI / 2), a(0.25), a(1.25), a(-0.5), a(1.75)};
            x.insert(x.end(), test.begin(), test.end());
        }

        if (!std::equal(x.begin(), x.end(), y.begin(), [](double x, double y) {
            return Comparator::isEqual(std::round(x * 1000000.) / 1000000., std::round(y * 1000000.) / 1000000.);
        })) throw std::logic_error("BarycentricInterpolatorTest: Interpolation failed");
    }

    static void GetWeightsTest() {
        std::vector<std::pair<double, double>> points{{0, 0}, {0.5, std::sin(0.5)}, {1, std::sin(1)}, {1.5, std::sin(1.5)}, {M_PI / 2, 1}};
        std::vector<double> x, y{1, -1, 1, -1, 1, -2, 4, -4, 16.125, -14.125, 2, -6, 9.50388, -30.2501, 24.7462, -1.33333, 5.86777, -11.0078, 29.5834, -23.1101, 0.848826, -3.73554, 7.00775, -18.8334, 14.7123};

        // y (measured values for different orders)

        for (int i = 0; i < points.size(); i++) {
            BarycentricInterpolator a(points, i);

            std::vector<double> test(a.GetWeights());
            x.insert(x.end(), test.begin(), test.end());
        }

        if (!std::equal(x.begin(), x.end(), y.begin(), [](double x, double y) {
            return Comparator::isEqual(std::round(x * 10000.) / 10000., std::round(y * 10000.) / 10000.);
        })) throw std::logic_error("BarycentricInterpolatorTest: GetWeights failed");
    }
};

#endif //ZADATAK_1_BARYCENTRICINTERPOLATORTEST_H
