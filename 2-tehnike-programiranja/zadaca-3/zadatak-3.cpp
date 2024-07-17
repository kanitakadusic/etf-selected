#include <iostream>
#include <iomanip>
#include <stdexcept>

template <typename TipElemenata>
struct Matrica {
    char ime_matrice;
    int broj_redova, broj_kolona;
    TipElemenata **elementi = nullptr;
};

template <typename TipElemenata>
void UnistiMatricu(Matrica<TipElemenata> &matrica) {
    if (!matrica.elementi) return;

    for (int i = 0; i < matrica.broj_redova; i++) delete[] matrica.elementi[i];
    delete[] matrica.elementi;

    matrica.elementi = nullptr;
}

template <typename TipElemenata>
Matrica<TipElemenata> StvoriMatricu(int broj_redova, int broj_kolona, char ime = 0) {
    Matrica<TipElemenata> matrica{ime, broj_redova, broj_kolona, new TipElemenata*[broj_redova]{}};

    try {
        for (int i = 0; i < broj_redova; i++)
            matrica.elementi[i] = new TipElemenata[broj_kolona]{};
    } catch (...) {
        UnistiMatricu(matrica); throw;
    }

    return matrica;
}

template <typename TipElemenata>
void UnesiMatricu(Matrica<TipElemenata> &matrica) {
    for (int i = 0; i < matrica.broj_redova; i++) {
        for (int j = 0; j < matrica.broj_kolona; j++) {
            std::cout << matrica.ime_matrice << "(" << i + 1 << "," << j + 1 << ") = ";
            std::cin >> matrica.elementi[i][j];
        }
    }
}

template <typename TipElemenata>
void IspisiMatricu(Matrica<TipElemenata> matrica, int sirina_ispisa, int preciznost = 6, bool treba_brisati = false) {
    for (int i = 0; i < matrica.broj_redova; i++) {
        for (int j = 0; j < matrica.broj_kolona; j++)
            std::cout << std::setw(sirina_ispisa) << std::setprecision(preciznost) << matrica.elementi[i][j];

        std::cout << std::endl;
    }

    if (treba_brisati) UnistiMatricu(matrica);
}

template <typename TipElemenata>
Matrica<TipElemenata> ZbirMatrica(Matrica<TipElemenata> prva, Matrica<TipElemenata> druga, bool treba_brisati_prvu = false, bool treba_brisati_drugu = false) {
    if (prva.broj_redova != druga.broj_redova || prva.broj_kolona != druga.broj_kolona) throw std::domain_error("Matrice nemaju jednake dimenzije");

    auto zbir(StvoriMatricu<TipElemenata>(prva.broj_redova, druga.broj_kolona));
    for (int i = 0; i < prva.broj_redova; i++)
        for (int j = 0; j < druga.broj_kolona; j++)
            zbir.elementi[i][j] = prva.elementi[i][j] + druga.elementi[i][j];

    if (treba_brisati_prvu) UnistiMatricu(prva);
    if (treba_brisati_drugu) UnistiMatricu(druga);
    return zbir;
}

template <typename TipElemenata>
Matrica<TipElemenata> ProduktMatrica(Matrica<TipElemenata> prva, Matrica<TipElemenata> druga, bool treba_brisati_prvu = false, bool treba_brisati_drugu = false) {
    if (prva.broj_kolona != druga.broj_redova) throw std::domain_error("Matrice nisu saglasne za mnozenje");

    auto produkt(StvoriMatricu<TipElemenata>(prva.broj_redova, druga.broj_kolona));
    for (int i = 0; i < prva.broj_redova; i++)
        for (int j = 0; j < druga.broj_kolona; j++)
            for (int k = 0; k < prva.broj_kolona; k++)
                produkt.elementi[i][j] += prva.elementi[i][k] * druga.elementi[k][j];

    if (treba_brisati_prvu) UnistiMatricu(prva);
    if (treba_brisati_drugu) UnistiMatricu(druga);
    return produkt;
}

template <typename TipElemenata>
Matrica<TipElemenata> KolicnikSkalar(Matrica<TipElemenata> matrica, double skalar, bool treba_brisati = false) {
    auto kolicnik(StvoriMatricu<TipElemenata>(matrica.broj_redova, matrica.broj_kolona));
    for (int i = 0; i < matrica.broj_redova; i++)
        for (int j = 0; j < matrica.broj_kolona; j++)
            kolicnik.elementi[i][j] = matrica.elementi[i][j] / skalar;

    if (treba_brisati) UnistiMatricu(matrica);
    return kolicnik;
}

template <typename TipElemenata>
Matrica<TipElemenata> MatricniHarmonijskiPolinom(const Matrica<TipElemenata> &matrica, int red_polinoma) {
    if (matrica.broj_redova != matrica.broj_kolona) throw std::domain_error("Matrica nije kvadratna");
    if (red_polinoma < 0) throw std::domain_error("Pogresan parametar n");

    Matrica<TipElemenata> polinomna = StvoriMatricu<TipElemenata>(matrica.broj_redova, matrica.broj_kolona), stepena, podijeljena;
    try { stepena = StvoriMatricu<TipElemenata>(matrica.broj_redova, matrica.broj_kolona); }
    catch (...) { UnistiMatricu(polinomna); throw; }

    for (int i = 0; i < matrica.broj_redova; i++)
        for (int j = 0; j < matrica.broj_kolona; j++)
            polinomna.elementi[i][j] = stepena.elementi[i][j] = matrica.elementi[i][j];

    for (int red = 2; red <= red_polinoma; red++) {
        try {
            stepena = ProduktMatrica(stepena, matrica, true);
            podijeljena = KolicnikSkalar(stepena, red);

            try {
                polinomna = ZbirMatrica(polinomna, podijeljena, true, true);
            } catch (...) {
                UnistiMatricu(podijeljena); throw;
            }
        } catch (...) {
            UnistiMatricu(stepena); UnistiMatricu(polinomna); throw;
        }
    }

    UnistiMatricu(stepena);
    return polinomna;
}

int main() {
    int dimenzija;
    std::cout << "Unesite dimenziju kvadratne matrice: ";
    std::cin >> dimenzija;

    Matrica<double> matrica, polinom;

    try {
        matrica = StvoriMatricu<double>(dimenzija, dimenzija, 'A');
        std::cout << "Unesite elemente matrice A:\n";
        UnesiMatricu(matrica);

        int red_polinoma;
        std::cout << "Unesite red polinoma: ";
        std::cin >> red_polinoma;

        polinom = MatricniHarmonijskiPolinom(matrica, red_polinoma);
        std::cout << "Matricni harmonijski polinom:\n";
        IspisiMatricu(polinom, 10, 6);
    } catch (std::exception &izuzetak) {
        std::cout << izuzetak.what();
    }

    UnistiMatricu(matrica);
    UnistiMatricu(polinom);

    return 0;
}