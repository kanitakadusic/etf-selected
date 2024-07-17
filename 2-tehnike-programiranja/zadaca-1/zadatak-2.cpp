#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>

std::vector<std::vector<double>> UsrednjavajuciFilter(const std::vector<std::vector<double>> &matrica, int red_filtriranja) {
    if (red_filtriranja < 0) throw std::domain_error("Neispravan red filtriranja");
    std::vector<std::vector<double>> filtrirana_matrica(matrica.size());

    for (int i = 0; i < matrica.size(); i++) {
        for (int j = 0; j < matrica.at(i).size(); j++) {
            double prosjek = 0;
            int brojac = (2 * red_filtriranja + 1) * (2 * red_filtriranja + 1);

            for (int m = -red_filtriranja; m <= red_filtriranja; m++) {
                for (int n = -red_filtriranja; n <= red_filtriranja; n++) {
                    try {
                        prosjek += matrica.at(i + m).at(j + n);
                    } catch (...) {
                        brojac--;
                    }
                }
            }

            prosjek /= brojac;
            filtrirana_matrica.at(i).push_back(prosjek);
        }
    }

    return filtrirana_matrica;
}

int main() {
    int broj_redova = 0, broj_kolona = 0;
    std::cout << "Unesite broj redova i kolona matrice: ";
    std::cin >> broj_redova >> broj_kolona;

    std::vector<std::vector<double>> matrica(broj_redova, std::vector<double>(broj_kolona));
    std::cout << "Unesite elemente matrice: ";
    for (int i = 0; i < broj_redova; i++) {
        for (int j = 0; j < broj_kolona; j++) {
            std::cin >> matrica.at(i).at(j);
        }
    }

    int red_filtriranja;
    std::cout << "Unesite red filtriranja: ";
    std::cin >> red_filtriranja;

    try {
        matrica = UsrednjavajuciFilter(matrica, red_filtriranja);
        std::cout << "\nMatrica nakon filtriranja:\n" << std::right << std::fixed << std::setprecision(2);
        for (const auto &red : matrica) {
            for (double element : red) {
                std::cout << std::setw(7) << element;
            }

            std::cout << std::endl;
        }
    } catch (std::domain_error &izuzetak) {
        std::cout << "\nGRESKA: " << izuzetak.what() << "!";
    }

    return 0;
}