#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <iomanip>

template <typename TEM>
using matrix = std::vector<std::vector<TEM>>;

template <typename TEPM, typename TEDM, typename TPF, typename TDF>
auto GeneraliziraniMatricniProizvod(matrix<TEPM> &prva_matrica, matrix<TEDM> &druga_matrica, TPF prva_funkcija, TDF druga_funkcija) {
    TEPM l_value_1 = TEPM(); TEDM l_value_2 = TEDM();
    using TENM = std::remove_reference_t<decltype(druga_funkcija(l_value_1, l_value_2))>;

    if (prva_matrica.empty() || druga_matrica.empty()) return matrix<TENM>(0);
    matrix<TENM> proizvod_matrica(prva_matrica.size(), std::vector<TENM>(druga_matrica.at(0).size()));

    for (int i = 0; i < prva_matrica.size(); i++) {
        if (prva_matrica.at(i).size() != druga_matrica.size()) throw std::domain_error("Matrice nisu saglasne za mnozenje");

        for (int j = 0; j < druga_matrica.at(0).size(); j++) {
            proizvod_matrica.at(i).at(j) = druga_funkcija(prva_matrica.at(i).at(0), druga_matrica.at(0).at(j));

            for (int k = 1; k < druga_matrica.size(); k++) {
                try {
                    proizvod_matrica.at(i).at(j) = prva_funkcija(proizvod_matrica.at(i).at(j), druga_funkcija(prva_matrica.at(i).at(k), druga_matrica.at(k).at(j)));
                } catch (...) {
                    throw std::runtime_error("Neocekivani problemi pri racunanju");
                }
            }
        }

    }

    return proizvod_matrica;
}

int main() {
    int broj_redova_prve;
    std::cout << "Unesite broj redova prve matrice: ";
    std::cin >> broj_redova_prve;

    int broj_kolona_redova;
    std::cout << "Unesite broj kolona prve matrice, ujedno broj redova druge matrice: ";
    std::cin >> broj_kolona_redova;

    int broj_kolona_druge;
    std::cout << "Unesite broj kolona druge matrice: ";
    std::cin >> broj_kolona_druge;

    matrix<std::string> prva_matrica(broj_redova_prve, std::vector<std::string>(broj_kolona_redova));
    std::cout << "\nUnesite elemente prve matrice:\n";
    for (auto &red : prva_matrica)
        for (auto &element : red)
            std::cin >> element;

    matrix<std::string> druga_matrica(broj_kolona_redova, std::vector<std::string>(broj_kolona_druge));
    std::cout << "Unesite elemente druge matrice:\n";
    for (auto &red : druga_matrica)
        for (auto &element : red)
            std::cin >> element;

    try {
        matrix<std::string> proizvod_matrica(GeneraliziraniMatricniProizvod(prva_matrica, druga_matrica,
                                                                             [](const std::string &a, const std::string &b) { return a + "+" + b; },
                                                                            [](const std::string &a, const std::string &b) { return a + "*" + b; }));

        unsigned int maks_sirina = 0;
        for (auto &red : proizvod_matrica)
            for (auto &element : red)
                if (element.size() > maks_sirina)
                    maks_sirina = element.size();

        std::cout << "\nMatricni proizvod:\n" << std::left;
        for (auto &red : proizvod_matrica) {
            for (auto &element : red)
                std::cout << std::setw(int(maks_sirina) + 1) << element;

            std::cout << std::endl;
        }
    } catch (std::exception &izuzetak) {
        std::cout << izuzetak.what();
    }

    return 0;
}