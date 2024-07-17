#include <iostream>
#include <stdexcept>
#include <array>
#include <vector>

enum class TretmanNegativnih {IgnorirajZnak, Odbaci, PrijaviGresku};

long long int MultiplikativniDigitalniKorijen(long long int broj, int baza) {
    if (baza < 2) throw std::domain_error("Neispravna baza");

    while (broj >= baza || broj <= -baza) {
        long long int proizvod_cifara = 1;

        while (broj != 0 && proizvod_cifara != 0) {
            proizvod_cifara *= broj % baza;
            broj /= baza;
        }

        broj = proizvod_cifara;
    }

    return (broj >= 0) ? broj : -broj;
}

std::array<std::vector<long long int>, 10> RazvrstajBrojeve(const std::vector<long long int> &vektor, TretmanNegativnih tretman) {
    std::array<std::vector<long long int>, 10> niz;

    for (long long int element : vektor) {
        if (element < 0) {
            switch (tretman) {
                // s obzirom da se ocekuje da zanemarimo minus, tj. istu vrijednost bez minusa smjestimo u niz
                // slucaj numeric_limits<long long int>::min() uopce ne bi trebao biti testni slucaj
                // tj. ponasanje s njim bi se trebalo dodefinisati, stoga je "element *= -1" u nastavku ok
                case TretmanNegativnih::IgnorirajZnak: element *= -1; break;
                case TretmanNegativnih::Odbaci: continue;
                case TretmanNegativnih::PrijaviGresku: throw std::domain_error("Nije predvidjeno razvrstavanje negativnih brojeva");
            }
        }

        niz.at(MultiplikativniDigitalniKorijen(element, 10)).push_back(element);
    }

    return niz;
}

int main() {
    std::vector<long long int> originalni_vektor;
    std::cout << "Unesite brojeve (bilo koji ne-broj oznacava kraj): ";

    long long int unos = 0;
    while (std::cin >> unos) originalni_vektor.push_back(unos);

    try {
        std::array<std::vector<long long int>, 10> razvrstani_niz(RazvrstajBrojeve(originalni_vektor, TretmanNegativnih::PrijaviGresku));
        std::cout << "\nRezultati razvrstavanja po multiplikativnom digitalnom korijenu:";
        for (int i = 0; i <= 9; i++) {
            if (!razvrstani_niz.at(i).empty()) {
                std::cout << std::endl << i << ": ";

                for (long long int element : razvrstani_niz.at(i)) {
                    std::cout << element << " ";
                }
            }
        }
    } catch (...) {
        std::cout << "\nNije podrzano razvrstavanje negativnih brojeva!";
    }

    return 0;
}