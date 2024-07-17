// ====================================================================================================
// NAPOMENA: Za pokretanje testnih funkcija ukloniti komentar u main() funkciji
// ====================================================================================================

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <string>

using std::cout;

// ==================================================================================================== BEGIN - MAPA

template <typename Key, typename Value>
class Mapa {
public:
    Mapa() = default;

    Mapa(const Mapa&) = default;
    Mapa& operator =(const Mapa&) = default;

    Mapa(Mapa&&) = default;
    Mapa& operator =(Mapa&&) = default;

    virtual ~Mapa() = default;

    // --------------------

    virtual int brojElemenata() const = 0;

    virtual void obrisi() = 0;
    virtual void obrisi(const Key&) = 0;

    virtual Value& operator [](const Key&) = 0;
    virtual Value operator [](const Key&) const = 0;
};

// ==================================================================================================== END - MAPA

// ==================================================================================================== BEGIN - NIZ MAPA

template <typename Key, typename Value>
class NizMapa: public Mapa<Key, Value> {
    int kapacitet, velicina;
    std::pair<Key, Value>** elementi;

    // --------------------

    static void dealociraj(std::pair<Key, Value>** elementi, int velicina);
    std::pair<Key, Value>* dodaj(const Key&);

    // --------------------

public:
    NizMapa(int kapacitet = 2);

    NizMapa(const NizMapa&);
    NizMapa& operator =(const NizMapa&);

    NizMapa(NizMapa&&);
    NizMapa& operator =(NizMapa&&);

    ~NizMapa() { NizMapa::dealociraj(this->elementi, this->velicina); }

    // --------------------

    int brojElemenata() const override { return this->velicina; }

    void obrisi() override;
    void obrisi(const Key&) override;

    Value& operator [](const Key&) override;
    Value operator [](const Key&) const override;
};

// ------------------------------------------------------- BEGIN - UPRAVLJACKI DIO KLASE

template <typename Key, typename Value>
NizMapa<Key, Value>::NizMapa(int kapacitet): velicina(0) {
    if (kapacitet < 0) throw std::logic_error("Neispravan kapacitet");

    this->kapacitet = kapacitet < 2 ? 2 : kapacitet;
    this->elementi = new std::pair<Key, Value>*[this->kapacitet]{};
}

template <typename Key, typename Value>
NizMapa<Key, Value>::NizMapa(const NizMapa& niz): kapacitet(niz.kapacitet), velicina(niz.velicina), elementi(new std::pair<Key, Value>*[niz.kapacitet]{}) {
    for (int i = 0; i < niz.velicina; i++)
        try {
            this->elementi[i] = new std::pair<Key, Value>{*niz.elementi[i]};
        } catch (...) {
            NizMapa::dealociraj(this->elementi, i); throw;
        }
}

template <typename Key, typename Value>
NizMapa<Key, Value>& NizMapa<Key, Value>::operator =(const NizMapa& niz) {
    if (niz.kapacitet > this->kapacitet) {
        auto novi_elementi = new std::pair<Key, Value>*[niz.kapacitet]{};

        for (int i = 0; i < niz.velicina; i++)
            try {
                novi_elementi[i] = new std::pair<Key, Value>{*niz.elementi[i]};
            } catch (...) {
                NizMapa::dealociraj(novi_elementi, i); throw;
            }

        NizMapa::dealociraj(this->elementi, this->velicina);

        this->kapacitet = niz.kapacitet;
        this->elementi = novi_elementi;
    } else if (&niz != this) {
        this->obrisi();

        for (int i = 0; i < niz.velicina; i++)
            try {
                this->elementi[i] = new std::pair<Key, Value>{*niz.elementi[i]};
            } catch (...) {
                NizMapa::dealociraj(this->elementi, i); throw;
            }
    }

    this->velicina = niz.velicina;
    return *this;
}

template <typename Key, typename Value>
NizMapa<Key, Value>::NizMapa(NizMapa&& niz): kapacitet(niz.kapacitet), velicina(niz.velicina), elementi(niz.elementi) {
    niz.velicina = 0; niz.elementi = nullptr;
}

template <typename Key, typename Value>
NizMapa<Key, Value>& NizMapa<Key, Value>::operator =(NizMapa&& niz) {
    std::swap(this->kapacitet, niz.kapacitet);
    std::swap(this->velicina, niz.velicina);
    std::swap(this->elementi, niz.elementi);

    return *this;
}

// ------------------------------------------------------- END - UPRAVLJACKI DIO KLASE

// ------------------------------------------------------- BEGIN - PRIVATNI DIO KLASE

template <typename Key, typename Value>
void NizMapa<Key, Value>::dealociraj(std::pair<Key, Value>** elementi, int velicina) {
    for (int i = 0; i < velicina; i++) delete elementi[i];
    delete[] elementi;
}

template <typename Key, typename Value>
std::pair<Key, Value>* NizMapa<Key, Value>::dodaj(const Key& key) {
    if (this->velicina == this->kapacitet) {
        auto novi_elementi = new std::pair<Key, Value>*[this->kapacitet *= 2]{};

        for (int i = 0; i < this->velicina; i++) {
            novi_elementi[i] = this->elementi[i];
            this->elementi[i] = nullptr;
        }

        delete[] this->elementi;
        this->elementi = novi_elementi;
    }

    this->elementi[this->velicina] = new std::pair<Key, Value>{key, Value()};
    return this->elementi[this->velicina++];
}

// ------------------------------------------------------- END - PRIVATNI DIO KLASE

// ------------------------------------------------------- BEGIN - IMPLEMENTACIJA INTERFEJSA

template <typename Key, typename Value>
void NizMapa<Key, Value>::obrisi() {
    std::for_each(this->elementi, this->elementi + this->velicina, [](std::pair<Key, Value>*& element) { delete element; element = nullptr; });
    this->velicina = 0;
}

template <typename Key, typename Value>
void NizMapa<Key, Value>::obrisi(const Key& key) {
    auto p = std::find_if(this->elementi, this->elementi + this->velicina, [&key](std::pair<Key, Value>* par) { return par->first == key; });

    if (p == this->elementi + this->velicina) throw std::logic_error("Nepostojeci kljuc");
    int indeks = p - this->elementi;

    delete this->elementi[indeks];
    std::copy(this->elementi + indeks + 1, this->elementi + this->velicina, this->elementi + indeks);

    this->velicina--;
}

template <typename Key, typename Value>
Value& NizMapa<Key, Value>::operator [](const Key& key) {
    auto p = std::find_if(this->elementi, this->elementi + this->velicina, [&key](std::pair<Key, Value>* par) { return par->first == key; });

    if (p == this->elementi + this->velicina) return this->dodaj(key)->second;
    return (*p)->second;
}

template <typename Key, typename Value>
Value NizMapa<Key, Value>::operator [](const Key& key) const {
    auto p = std::find_if(this->elementi, this->elementi + this->velicina, [&key](std::pair<Key, Value>* par) { return par->first == key; });

    if (p == this->elementi + this->velicina) return Value();
    return (*p)->second;
}

// ------------------------------------------------------- END - IMPLEMENTACIJA INTERFEJSA

// ==================================================================================================== END - NIZ MAPA

void testBrojElemenata(NizMapa<int, std::string>);
void testObrisiSve(NizMapa<int, std::string>);
void testObrisi(NizMapa<int, std::string>);
void testOperator(NizMapa<int, std::string>);
void testOperatorConst(const NizMapa<int, std::string>&);
void testMapa(const NizMapa<int, std::string>&);

int main() {
    NizMapa<int, std::string> mapa;

    mapa[1] = "jedan";
    mapa[9] = "devet";
    mapa[3] = "tri";
    mapa[2] = "dva";
    mapa[7] = "sedam";

    testMapa(mapa);

    return 0;
}

// ==================================================================================================== BEGIN - TEST FUNKCIJE

void testBrojElemenata(NizMapa<int, std::string> mapa) {
    std::cout << std::string(75, '-') << " TEST brojElemenata()\n";

    std::cout << mapa.brojElemenata() << std::endl;
    mapa.obrisi(3);
    std::cout << mapa.brojElemenata() << std::endl;
    mapa.obrisi();
    std::cout << mapa.brojElemenata() << std::endl;
}

void testObrisiSve(NizMapa<int, std::string> mapa) {
    std::cout << std::string(75, '-') << " TEST obrisiSve()\n";

    for (int i = 1; i <= 9; i++) std::cout << "'" << mapa[i] << "' ";
    std::cout << std::endl;

    mapa.obrisi();
    for (int i = 1; i <= 9; i++) std::cout << "'" << mapa[i] << "' ";
    std::cout << std::endl;

    mapa.obrisi();
    for (int i = 1; i <= 9; i++) std::cout << "'" << mapa[i] << "' ";
    std::cout << std::endl;
}

void testObrisi(NizMapa<int, std::string> mapa) {
    std::cout << std::string(75, '-') << " TEST obrisi()\n";

    for (int i = 1; i <= 9; i++) std::cout << "'" << mapa[i] << "' ";
    std::cout << std::endl;

    mapa.obrisi(2);
    for (int i = 1; i <= 9; i++) std::cout << "'" << mapa[i] << "' ";
    std::cout << std::endl;

    try {
        mapa.obrisi(11);
    } catch (std::logic_error e) {
        std::cout << "IZUZETAK: " << e.what() << std::endl;
    }
}

void testOperator(NizMapa<int, std::string> mapa) {
    std::cout << std::string(75, '-') << " TEST operator []()\n";

    for (int i = 1; i <= 9; i++) std::cout << "'" << mapa[i] << "' ";
    std::cout << std::endl;

    mapa[1] = "kec";
    mapa[5] = "pet";
    mapa[11] = "jedanaest";

    for (int i = 1; i <= 11; i++) std::cout << "'" << mapa[i] << "' ";
    std::cout << std::endl;
}

void testOperatorConst(const NizMapa<int, std::string>& mapa) {
    std::cout << std::string(75, '-') << " TEST operatorConst []()\n";

    for (int i = 1; i <= 15; i++) std::cout << "'" << mapa[i] << "' ";
    std::cout << std::endl << mapa.brojElemenata() << std::endl;
}

void testMapa(const NizMapa<int, std::string>& mapa) {
    testBrojElemenata(mapa);
    testObrisiSve(mapa);
    testObrisi(mapa);
    testOperator(mapa);
    testOperatorConst(mapa);
}

// ==================================================================================================== END - TEST FUNKCIJE