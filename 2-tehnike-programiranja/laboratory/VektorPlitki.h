#ifndef LABORATORY_VEKTORPLITKI_H
#define LABORATORY_VEKTORPLITKI_H

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>

class VektorPlitki {
    int *dimenzija, *brojac;
    double **koordinate;
    static constexpr double granica_za_realokaciju = 0.2;

    void TestIndeksa(int indeks) const {
        if (indeks < 1 || indeks > *dimenzija) throw std::range_error("Pogresan indeks!");
    }

public:
    explicit VektorPlitki(int dimenzija);
    VektorPlitki(std::initializer_list<double> lista);

    VektorPlitki(const VektorPlitki &vektor);
    VektorPlitki &operator =(const VektorPlitki &vektor);

    ~VektorPlitki();

    void PromijeniDimenziju(int nova_dimenzija);

    double DajKoordinatu(int indeks) const {
        TestIndeksa(indeks); return (*koordinate)[indeks - 1];
    }

    double &DajKoordinatu(int indeks) {
        TestIndeksa(indeks); return (*koordinate)[indeks - 1];
    }

    void Ispisi() const;

    double *begin() const { return *koordinate; }
    double *end() const { return *koordinate + *dimenzija; }

    friend VektorPlitki ZbirPlitkihVektora(const VektorPlitki &prvi, const VektorPlitki &drugi);
};

VektorPlitki::VektorPlitki(int dimenzija): dimenzija(new int(dimenzija)), brojac(new int(1)), koordinate(new double*) {
    *koordinate = new double[dimenzija]{};
}

VektorPlitki::VektorPlitki(std::initializer_list<double> lista): dimenzija(new int(lista.size())), brojac(new int(1)), koordinate(new double*) {
    *koordinate = new double[lista.size()];
    std::copy(lista.begin(), lista.end(), *koordinate);
}

VektorPlitki::VektorPlitki(const VektorPlitki &vektor): dimenzija(vektor.dimenzija), brojac(vektor.brojac), koordinate(vektor.koordinate) {
    (*brojac)++;
}

VektorPlitki::~VektorPlitki() {
    if (--(*brojac) == 0) {
        delete dimenzija; delete brojac; delete[] *koordinate; delete koordinate;
    }
}

VektorPlitki &VektorPlitki::operator =(const VektorPlitki &vektor) {
    (*vektor.brojac)++;
    if (--(*brojac) == 0) {
        delete dimenzija; delete brojac; delete[] *koordinate; delete koordinate;
    }

    dimenzija = vektor.dimenzija; brojac = vektor.brojac; koordinate = vektor.koordinate;
    return *this;
}

void VektorPlitki::PromijeniDimenziju(int nova_dimenzija) {
    if (nova_dimenzija > *dimenzija || nova_dimenzija <= *dimenzija * granica_za_realokaciju) {
        auto novi_prostor = new double[nova_dimenzija]{};
        std::copy(*koordinate, *koordinate + (*dimenzija < nova_dimenzija ? *dimenzija : nova_dimenzija), novi_prostor);

        delete[] *koordinate; *koordinate = novi_prostor;
    }

    *dimenzija = nova_dimenzija;
}

VektorPlitki ZbirPlitkihVektora(const VektorPlitki &prvi, const VektorPlitki &drugi) {
    if (*prvi.dimenzija != *drugi.dimenzija) throw std::domain_error("Nesaglasne dimenzije!");

    VektorPlitki zbir(*prvi.dimenzija);
    for (int i = 0; i < *prvi.dimenzija; i++)
        (*zbir.koordinate)[i] = (*prvi.koordinate)[i] + (*drugi.koordinate)[i];

    return zbir;
}

void VektorPlitki::Ispisi() const {
    std::cout << "{";

    for (int i = 0; i < *dimenzija; i++)
        std::cout << (*koordinate)[i] << (i != *dimenzija - 1 ? "," : "");

    std::cout << "}";
}

#endif //LABORATORY_VEKTORPLITKI_H

/*
    VektorPlitki a{1, 2, 3, 4, 5}; a.Ispisi();
    VektorPlitki b(a); b.PromijeniDimenziju(10); b.DajKoordinatu(9) = 9; a.Ispisi(); b.Ispisi();
    std::cout << std::endl;

    VektorPlitki c(ZbirPlitkihVektora(a, b));
    for (auto &element : c) std::cout << element << " ";
    std::cout << std::endl;

    c = a; c.DajKoordinatu(7) = 7; a.Ispisi(); b.Ispisi(); c.Ispisi();
*/