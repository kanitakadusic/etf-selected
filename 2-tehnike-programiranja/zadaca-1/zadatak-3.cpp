#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <stdexcept>

std::vector<std::vector<int>> KreirajSpiralnuMatricu(int broj_redova, int broj_kolona, int pocetna_vrijednost, bool desna_spiralna_matrica) {
    if (broj_redova <= 0 || broj_kolona <= 0) return std::vector<std::vector<int>>(0);
    if ((static_cast<long int>(pocetna_vrijednost) + broj_redova * broj_kolona - 1) > std::numeric_limits<int>::max()) throw std::overflow_error("Prekoracenje opsega!");

    std::vector<std::vector<int>> spiralna_matrica(broj_redova, std::vector<int>(broj_kolona));
    int red = 0, kolona = 0; broj_redova--; broj_kolona--;

    if (desna_spiralna_matrica) {
        for (;;) {
            for (int i = kolona; i <= broj_kolona; i++)
                spiralna_matrica.at(red).at(i) = pocetna_vrijednost++;
            red++;

            for (int i = red; i <= broj_redova; i++)
                spiralna_matrica.at(i).at(broj_kolona) = pocetna_vrijednost++;
            broj_kolona--;

            if (red > broj_redova || kolona > broj_kolona) break;

            for (int i = broj_kolona; i >= kolona; i--)
                spiralna_matrica.at(broj_redova).at(i) = pocetna_vrijednost++;
            broj_redova--;

            for (int i = broj_redova; i >= red; i--)
                spiralna_matrica.at(i).at(kolona) = pocetna_vrijednost++;
            kolona++;

            if (red > broj_redova || kolona > broj_kolona) break;
        }
    } else {
        for (;;) {
            for (int i = red; i <= broj_redova; i++)
                spiralna_matrica.at(i).at(kolona) = pocetna_vrijednost++;
            kolona++;

            for (int i = kolona; i <= broj_kolona; i++)
                spiralna_matrica.at(broj_redova).at(i) = pocetna_vrijednost++;
            broj_redova--;

            if (red > broj_redova || kolona > broj_kolona) break;

            for (int i = broj_redova; i >= red; i--)
                spiralna_matrica.at(i).at(broj_kolona) = pocetna_vrijednost++;
            broj_kolona--;

            for (int i = broj_kolona; i >= kolona; i--)
                spiralna_matrica.at(red).at(i) = pocetna_vrijednost++;
            red++;

            if (red > broj_redova || kolona > broj_kolona) break;
        }
    }

    return spiralna_matrica;
}

bool DaLiJeSpiralnaMatrica(const std::vector<std::vector<int>> &matrica) {
    for (auto &red : matrica)
        if (red.size() != matrica.at(0).size()) return false;

    try {
        if (matrica != KreirajSpiralnuMatricu(matrica.size(), matrica.at(0).size(), matrica.at(0).at(0), true)) {
            try {
                return matrica == KreirajSpiralnuMatricu(matrica.size(), matrica.at(0).size(), matrica.at(0).at(0), false);
            } catch (std::overflow_error &) {
                return false;
            }
        }
    } catch (std::overflow_error &) {
        return false;
    }

    return true;
}

int main() {
    int broj_redova, broj_kolona;
    std::cout << "Unesite broj redova i kolona matrice: ";
    std::cin >> broj_redova >> broj_kolona;

    int pocetna_vrijednost;
    std::cout << "Unesite pocetnu vrijednost: ";
    std::cin >> pocetna_vrijednost;

    char orijentacija;
    std::cout << "Unesite L za lijevu, a D za desnu spiralnu matricu: ";
    std::cin >> orijentacija;

    std::vector<std::vector<int>> spiralna_matrica(KreirajSpiralnuMatricu(broj_redova, broj_kolona, pocetna_vrijednost, orijentacija == 'D'));

    int sirina_ispisa = pocetna_vrijednost + broj_redova * broj_kolona - 1;
    if (std::abs(pocetna_vrijednost) > std::abs(sirina_ispisa)) sirina_ispisa = pocetna_vrijednost;

    bool minus = sirina_ispisa < 0;
    sirina_ispisa = (sirina_ispisa == 0) ? 1 : std::log10(std::abs(sirina_ispisa)) + 1;
    sirina_ispisa += (minus) ? 2 : 1;

    std::cout << "\nKreirana spiralna matrica: " << std::endl;
    for (const auto &red : spiralna_matrica) {
        for (int element : red) {
            std::cout.width(sirina_ispisa);
            std::cout << element;
        }

        std::cout << std::endl;
    }

    return 0;
}


/*

// *POKUSAJ* "AUTOMATIZOVANIJIM" PUTEM
// case 5 5 -9 D failed

enum class SmjerKretanja {Desno, Lijevo, Gore, Dolje};

std::vector<std::vector<int>> KreirajSpiralnuMatricu(int broj_redova, int broj_kolona, int pocetna_vrijednost, bool desna_spiralna_matrica) {
    if (broj_redova <= 0 || broj_kolona <= 0) return std::vector<std::vector<int>>(0);

    std::vector<std::vector<int>> spiralna_matrica(broj_redova, std::vector<int>(broj_kolona));
    SmjerKretanja smjer = (desna_spiralna_matrica) ? SmjerKretanja::Desno : SmjerKretanja::Dolje;
    int i = 0, j = 0, krajnja_vrijednost = pocetna_vrijednost + broj_redova * broj_kolona - 1;

    while (pocetna_vrijednost < krajnja_vrijednost) {
        int desno_lijevo = 0, gore_dolje = 0;

        if (desna_spiralna_matrica) {
            switch (smjer) {
                case SmjerKretanja::Desno: desno_lijevo = 1; gore_dolje = 0; smjer = SmjerKretanja::Dolje; break;
                case SmjerKretanja::Dolje: desno_lijevo = 0; gore_dolje = 1; smjer = SmjerKretanja::Lijevo; break;
                case SmjerKretanja::Lijevo: desno_lijevo = -1; gore_dolje = 0; smjer = SmjerKretanja::Gore; break;
                case SmjerKretanja::Gore: desno_lijevo = 0; gore_dolje = -1; smjer = SmjerKretanja::Desno; break;
            }
        } else { // iako radi ok i bez else dijela
            switch (smjer) {
                case SmjerKretanja::Dolje: desno_lijevo = 0; gore_dolje = 1; smjer = SmjerKretanja::Desno; break;
                case SmjerKretanja::Desno: desno_lijevo = 1; gore_dolje = 0; smjer = SmjerKretanja::Gore; break;
                case SmjerKretanja::Gore: desno_lijevo = 0; gore_dolje = -1; smjer = SmjerKretanja::Lijevo; break;
                case SmjerKretanja::Lijevo: desno_lijevo = -1; gore_dolje = 0; smjer = SmjerKretanja:: Dolje; break;
            }
        }

        while (spiralna_matrica.at(i).at(j) == 0) {
            try {
                if (spiralna_matrica.at(i + gore_dolje).at(j + desno_lijevo) == 0) {
                    spiralna_matrica.at(i).at(j) = pocetna_vrijednost++;
                } else {
                    break;
                }
            } catch (...) {
                break;
            }

            i += gore_dolje;
            j += desno_lijevo;
        }
    }

    spiralna_matrica.at(i).at(j) = pocetna_vrijednost;
    return spiralna_matrica;
}

*/