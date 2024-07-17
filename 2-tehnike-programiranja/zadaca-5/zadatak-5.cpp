#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <functional>
#include <vector>

template <typename Tip>
class DatotecniKontejner {
    std::fstream tok;

    void TestPozicije(int pozicija) {
        if (pozicija < 0 || pozicija >= DajBrojElemenata()) throw std::range_error("Neispravna pozicija");
    }

    void TestToka() {
        if (!this->tok) throw std::logic_error("Problemi prilikom pristupa datoteci");
    }

public:
    DatotecniKontejner(const std::string& naziv);

    int DajBrojElemenata();
    Tip DajElement(int pozicija);

    void DodajNoviElement(const Tip& element);
    void IzmijeniElement(int pozicija, const Tip& element);

    void Sortiraj(std::function<bool(const Tip&, const Tip&)> kriterij = std::less<Tip>());
};

template <typename Tip>
DatotecniKontejner<Tip>::DatotecniKontejner(const std::string& naziv): tok(naziv, std::ios::in) {
    if (!this->tok) this->tok.open(naziv, std::ios::out);
    this->tok.close();

    this->tok.open(naziv, std::ios::in | std::ios::out | std::ios::binary);
    if (!this->tok) throw std::logic_error("Problemi prilikom otvaranja ili kreiranja datoteke");
}

template <typename Tip>
int DatotecniKontejner<Tip>::DajBrojElemenata() {
    this->tok.seekg(0, std::ios::end);
    int duzina = this->tok.tellg();

    TestToka();
    return duzina / sizeof(Tip);
}

template <typename Tip>
Tip DatotecniKontejner<Tip>::DajElement(int pozicija) {
    TestPozicije(pozicija);

    Tip element;
    this->tok.seekg(pozicija * sizeof(Tip));
    this->tok.read(reinterpret_cast<char*>(&element), sizeof(element));

    TestToka();
    return element;
}

template <typename Tip>
void DatotecniKontejner<Tip>::DodajNoviElement(const Tip& element) {
    this->tok.seekp(0, std::ios::end);
    this->tok.write(reinterpret_cast<const char*>(&element), sizeof(element));
    this->tok.flush();

    TestToka();
}

template <typename Tip>
void DatotecniKontejner<Tip>::IzmijeniElement(int pozicija, const Tip& element) {
    TestPozicije(pozicija);

    this->tok.seekp(pozicija * sizeof(Tip));
    this->tok.write(reinterpret_cast<const char*>(&element), sizeof(element));

    TestToka();
}

template <typename Tip>
void DatotecniKontejner<Tip>::Sortiraj(std::function<bool(const Tip& prvi, const Tip& drugi)> kriterij) {
    int broj_elemenata = DajBrojElemenata();

    for (int i = 0; i < broj_elemenata; i++) {
        for (int j = i + 1; j < broj_elemenata; j++) {
            Tip prvi = DajElement(i);
            Tip drugi = DajElement(j);

            if (kriterij(drugi, prvi)) {
                IzmijeniElement(i, drugi);
                IzmijeniElement(j, prvi);
            }
        }
    }
}

int main() {
    try {
        DatotecniKontejner<int> kolekcija("testna.dat");
        int elementi[]{12, -6, 4, 18, -4, 0, 3, 11, -5, 1};

        for (int i = 0; i < 10; i++)
            kolekcija.DodajNoviElement(elementi[i]);

        int broj_elemenata = kolekcija.DajBrojElemenata();

        for (int i = 0; i < broj_elemenata; i += 3)
            kolekcija.IzmijeniElement(i, i + 3);

        kolekcija.Sortiraj();

        for (int i = 0; i < broj_elemenata; i++)
            std::cout << kolekcija.DajElement(i) << " ";

    } catch (std::exception& izuzetak) {
        std::cout << izuzetak.what();
    }

    return 0;
}