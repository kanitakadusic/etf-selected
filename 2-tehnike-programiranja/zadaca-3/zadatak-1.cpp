#include <iostream>
#include <functional>
#include <vector>
#include <cmath>
#include <utility>
#include <stdexcept>

constexpr double EPSILON = 10e-3;

std::function<double(double)> LagrangeovaInterpolacija(const std::vector<std::pair<double, double>> &cvorovi) {
    for (int i = 0; i < cvorovi.size(); i++)
        for (int j = i + 1; j < cvorovi.size(); j++)
            if (std::fabs(cvorovi.at(i).first - cvorovi.at(j).first) < EPSILON) throw std::domain_error("Neispravni cvorovi");

    return [cvorovi](double x) {
        double suma = 0;

        for (int i = 0; i < cvorovi.size(); i++) {
            double proizvod = 1;

            for (int j = 0; j < cvorovi.size(); j++)
                if (i != j) proizvod *= (x - cvorovi.at(j).first) / (cvorovi.at(i).first - cvorovi.at(j).first);

            suma += cvorovi.at(i).second * proizvod;
        }

        return suma;
    };
}

template <typename TipFunkcije>
std::function<double(double)> LagrangeovaInterpolacija(TipFunkcije funkcija, double min, double max, double delta) {
    if (min - max > EPSILON || delta < 0 || std::fabs(delta) < EPSILON) throw std::domain_error("Nekorektni parametri");

    std::vector<std::pair<double, double>> cvorovi;
    for (double tacka = min; tacka < max || std::fabs(tacka - max) < EPSILON; tacka += delta)
        cvorovi.emplace_back(tacka, funkcija(tacka));

    return LagrangeovaInterpolacija(cvorovi);
}

int main() {
    void OpcijaCvorovi();
    void OpcijaAproksimacija();

    int opcija;
    std::cout << "Odaberite opciju (1 - unos cvorova, 2 - aproksimacija): ";
    std::cin >> opcija;

    try { opcija == 1 ? OpcijaCvorovi() : OpcijaAproksimacija(); }
    catch (std::exception &izuzetak) { std::cout << izuzetak.what(); }

    return 0;
}

void OpcijaCvorovi() {
    int broj_cvorova;
    std::cout << "Unesite broj cvorova: ";
    std::cin >> broj_cvorova;

    std::vector<std::pair<double, double>> cvorovi;
    std::cout << "Unesite cvorove kao parove x y: ";

    double x, y, min, max;
    std::cin >> x >> y;
    min = max = x;
    cvorovi.emplace_back(x, y);

    while (--broj_cvorova) {
        std::cin >> x >> y;
        cvorovi.emplace_back(x, y);

        if (min - x > EPSILON) min = x;
        if (x - max > EPSILON) max = x;
    }

    auto interpolacija(LagrangeovaInterpolacija(cvorovi));

    while (std::cin) {
        double unos;
        std::cout << "Unesite argument (ili \"kraj\" za kraj): ";
        std::cin >> unos;

        if (std::cin) {
            std::cout << "f(" << unos << ") = " << interpolacija(unos) << " ";
            std::cout << (min - unos > EPSILON || unos - max > EPSILON ? "[ekstrapolacija]\n" : "\n");
        }
    }
}

void OpcijaAproksimacija() {
    double min, max, delta;
    std::cout << "Unesite krajeve intervala i korak: ";
    std::cin >> min >> max >> delta;

    auto funkcija([](double x) { return x * x + std::sin(x) + std::log(x + 1); });
    auto interpolacija(LagrangeovaInterpolacija(funkcija, min, max, delta));

    while (std::cin) {
        double unos;
        std::cout << "Unesite argument (ili \"kraj\" za kraj): ";
        std::cin >> unos;

        if (std::cin) {
            std::cout << "f(" << unos << ") = " << funkcija(unos) << " ";
            std::cout << "P(" << unos << ") = " << interpolacija(unos) << " ";
            std::cout << (min - unos > EPSILON || unos - max > EPSILON ? "[ekstrapolacija]\n" : "\n");
        }
    }
}