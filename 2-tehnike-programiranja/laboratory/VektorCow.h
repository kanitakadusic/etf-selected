#ifndef LABORATORY_VEKTORCOW_H
#define LABORATORY_VEKTORCOW_H

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>

class VektorCow {
    int dimenzija, *brojac;
    double *koordinate;
    static constexpr double granica_za_realokaciju = 0.2;

    void TestIndeksa(int indeks) const {
        if (indeks < 1 || indeks > dimenzija) throw std::range_error("Pogresan indeks!");
    }

public:
    explicit VektorCow(int dimenzija): dimenzija(dimenzija), brojac(new int(1)), koordinate(new double[dimenzija]{}) {}
    VektorCow(std::initializer_list<double> lista);

    VektorCow(const VektorCow &vektor);
    VektorCow &operator =(const VektorCow &vektor);

    ~VektorCow();

    void PromijeniDimenziju(int nova_dimenzija);

    double DajKoordinatu(int indeks) const {
        TestIndeksa(indeks); return koordinate[indeks - 1];
    }

    double &DajKoordinatu(int indeks);

    void Ispisi() const;

    double *begin() const { return koordinate; }
    double *end() const { return koordinate + dimenzija; }

    friend VektorCow ZbirCowVektora(const VektorCow &prvi, const VektorCow &drugi);
};

VektorCow::VektorCow(std::initializer_list<double> lista): dimenzija(lista.size()), brojac(new int(1)), koordinate(new double[lista.size()]) {
    std::copy(lista.begin(), lista.end(), koordinate);
}

VektorCow::VektorCow(const VektorCow &vektor): dimenzija(vektor.dimenzija), brojac(vektor.brojac), koordinate(vektor.koordinate) {
    (*brojac)++;
}

VektorCow::~VektorCow() {
    if (--(*brojac) == 0) {
        std::cout << " oslobodi ";
        delete brojac; delete[] koordinate;
    }
}

VektorCow &VektorCow::operator =(const VektorCow &vektor) {
    (*vektor.brojac)++;
    if (--(*brojac) == 0) {
        delete brojac; delete[] koordinate;
    }

    dimenzija = vektor.dimenzija; brojac = vektor.brojac; koordinate = vektor.koordinate;
    return *this;
}

void VektorCow::PromijeniDimenziju(int nova_dimenzija) {
    if (nova_dimenzija > dimenzija || (*brojac == 1 && nova_dimenzija <= dimenzija * granica_za_realokaciju)) {
        std::cout << " realokacija dimenzija ";
        auto novi_prostor = new double[nova_dimenzija]{};
        std::copy(koordinate, koordinate + (dimenzija < nova_dimenzija ? dimenzija : nova_dimenzija), novi_prostor);

        if (--(*brojac) == 0) { delete brojac; delete[] koordinate; }
        brojac = new int(1); koordinate = novi_prostor;
    }

    dimenzija = nova_dimenzija;
}

double &VektorCow::DajKoordinatu(int indeks) {
    TestIndeksa(indeks);

    if (*brojac != 1) {
        std::cout << " realokacija daj ";
        auto novi_prostor = new double[dimenzija];
        std::copy(koordinate, koordinate + dimenzija, novi_prostor);

        (*brojac)--;
        brojac = new int(1); koordinate = novi_prostor;
    }

    return koordinate[indeks - 1];
}

VektorCow ZbirCowVektora(const VektorCow &prvi, const VektorCow &drugi) {
    if (prvi.dimenzija != drugi.dimenzija) throw std::domain_error("Nesaglasne dimenzije!");

    VektorCow zbir(prvi.dimenzija);
    for (int i = 0; i < prvi.dimenzija; i++)
        zbir.koordinate[i] = prvi.koordinate[i] + drugi.koordinate[i];

    return zbir;
}

void VektorCow::Ispisi() const {
    std::cout << "{";

    for (int i = 0; i < dimenzija; i++)
        std::cout << koordinate[i] << (i != dimenzija - 1 ? "," : "");

    std::cout << "}";
}

#endif //LABORATORY_VEKTORCOW_H

/*
    VektorCow a{1, 2, 3, 4, 5}; a.Ispisi();
    VektorCow b(a); b.PromijeniDimenziju(10); b.DajKoordinatu(9) = 9; a.Ispisi(); b.Ispisi();
    std::cout << std::endl;

    VektorCow c(ZbirCowVektora(a, {1, 1, 1, 1, 1}));
    for (auto &element : c) std::cout << element << " ";
    std::cout << std::endl;

    c = a; c.PromijeniDimenziju(3); a.Ispisi(); c.Ispisi(); std::cout << std::endl;
    c.DajKoordinatu(1) = 11; c.DajKoordinatu(2) = 22; a.Ispisi(); b.Ispisi(); c.Ispisi();

    VektorCow d(b);
*/