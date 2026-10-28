#ifndef LABORATORY_VEKTOR_H
#define LABORATORY_VEKTOR_H

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>

class Vektor {
    int dimenzija;
    double *koordinate;

    void TestIndeksa(int indeks) const {
        if (indeks < 1 || indeks > dimenzija) throw std::range_error("Pogresan indeks!");
    }

    static constexpr double granica_za_realokaciju = 0.2;

public:
    explicit Vektor(int dimenzija): dimenzija(dimenzija), koordinate(new double[dimenzija]{}) {}
    Vektor(std::initializer_list<double> lista);

    Vektor(const Vektor &vektor);
    Vektor(Vektor &&vektor): dimenzija(vektor.dimenzija), koordinate(vektor.koordinate) { vektor.koordinate = nullptr; }

    Vektor &operator =(const Vektor &vektor);
    Vektor &operator =(Vektor &&vektor);

    ~Vektor() { delete[] koordinate; }

    void PromijeniDimenziju(int nova_dimenzija);

/*
    void PostaviKoordinatu(int indeks, double vrijednost) {
        TestIndeksa(indeks); koordinate[indeks - 1] = vrijednost;
    }
*/

    double DajKoordinatu(int indeks) const {
        TestIndeksa(indeks); return koordinate[indeks - 1];
    }

    double &DajKoordinatu(int indeks) {
        TestIndeksa(indeks); return koordinate[indeks - 1];
    }

    void Ispisi() const;

    double *begin() const { return koordinate; }
    double *end() const { return koordinate + dimenzija; }

    friend Vektor ZbirVektora(const Vektor &prvi, const Vektor &drugi);
};

Vektor::Vektor(std::initializer_list<double> lista): dimenzija(lista.size()), koordinate(new double[lista.size()]) {
    std::copy(lista.begin(), lista.end(), koordinate);
}

Vektor::Vektor(const Vektor &vektor): dimenzija(vektor.dimenzija), koordinate(new double[vektor.dimenzija]) {
    std::copy(vektor.koordinate, vektor.koordinate + vektor.dimenzija, koordinate);
}

Vektor &Vektor::operator =(const Vektor &vektor) {
    if (vektor.dimenzija > dimenzija || vektor.dimenzija <= dimenzija * granica_za_realokaciju) {
        double *novi_prostor = new double[vektor.dimenzija];
        delete[] koordinate; koordinate = novi_prostor;
    }

    dimenzija = vektor.dimenzija;
    std::copy(vektor.koordinate, vektor.koordinate + vektor.dimenzija, koordinate);

    return *this;
}

/*
Vektor &Vektor::operator =(Vektor lokalni) {
    std::swap(dimenzija, lokalni.dimenzija);
    std::swap(koordinate, lokalni.koordinate);

    return *this;
}
*/

Vektor &Vektor::operator =(Vektor &&vektor) {
    if (&vektor != this) {
        delete[] koordinate; koordinate = vektor.koordinate;
        dimenzija = vektor.dimenzija; vektor.koordinate = nullptr;
    }

    return *this;
}

/* // samo ako kopirajuci operator dodjele nije izveden tehnikom copy&swap
Vektor &Vektor::operator =(Vektor &&lokalni) {
    std::swap(dimenzija, lokalni.dimenzija);
    std::swap(koordinate, lokalni.koordinate);

    return *this;
}
*/

void Vektor::PromijeniDimenziju(int nova_dimenzija) {
    if (nova_dimenzija > dimenzija || nova_dimenzija <= dimenzija * granica_za_realokaciju) {
        double *novi_prostor = new double[nova_dimenzija]{};
        std::copy(koordinate, koordinate + (dimenzija < nova_dimenzija ? dimenzija : nova_dimenzija), novi_prostor);

        delete[] koordinate; koordinate = novi_prostor;
    }

    dimenzija = nova_dimenzija;
}

Vektor ZbirVektora(const Vektor &prvi, const Vektor &drugi) {
    if (prvi.dimenzija != drugi.dimenzija) throw std::domain_error("Nesaglasne dimenzije!");

    Vektor zbir(prvi.dimenzija);
    for (int i = 0; i < prvi.dimenzija; i++)
        zbir.koordinate[i] = prvi.koordinate[i] + drugi.koordinate[i];

    return zbir;
}

void Vektor::Ispisi() const {
    std::cout << "{";

    for (int i = 0; i < dimenzija; i++)
        std::cout << koordinate[i] << (i != dimenzija - 1 ? "," : "");

    std::cout << "}";
}

#endif //LABORATORY_VEKTOR_H
