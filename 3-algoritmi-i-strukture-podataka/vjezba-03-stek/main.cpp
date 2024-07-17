// ====================================================================================================
// NAPOMENA: Za pokretanje testnih funkcija ukloniti komentar u main() funkciji
// ====================================================================================================

#include <iostream>
#include <stdexcept>

using std::cout; // zbog AT4

// ==================================================================================================== BEGIN - STEK

template <typename Tip>
class Stek {
    struct Cvor {
        Tip vrijednost;
        Cvor* prethodni;

        Cvor(const Tip& vrijednost, Cvor* prethodni): vrijednost(vrijednost), prethodni(prethodni) {}
    };

    int velicina;
    Cvor* top;

    // --------------------

    void kopiraj(const Stek&);

    // --------------------

public:
    Stek(): velicina(0), top(nullptr) {}

    Stek(const Stek& stek): velicina(0), top(nullptr) { this->kopiraj(stek); }
    Stek& operator =(const Stek&);

    ~Stek() { this->brisi(); }

    // --------------------

    int brojElemenata() const { return this->velicina; }

    void brisi() {
        while (this->brojElemenata() != 0)
            this->skini();
    }

    void stavi(const Tip& vrijednost) {
        this->top = new Cvor(vrijednost, this->top);
        this->velicina++;
    }

    Tip skini();

    Tip& vrh() {
        if (this->brojElemenata() == 0) throw std::logic_error("Prazan stek");
        return this->top->vrijednost;
    }
};

template <typename Tip>
Stek<Tip>& Stek<Tip>::operator =(const Stek& stek) {
    if (&stek != this) {
        this->brisi();
        this->kopiraj(stek);
    }

    return *this;
}

template <typename Tip>
void Stek<Tip>::kopiraj(const Stek& stek) {
    Cvor *citaj(stek.top), *novi;

    if (stek.brojElemenata() != 0) {
        this->top = novi = new Cvor(citaj->vrijednost, nullptr);
        citaj = citaj->prethodni;

        this->velicina = 1;
    }

    while (citaj != nullptr) {
        novi->prethodni = new Cvor(citaj->vrijednost, nullptr);
        citaj = citaj->prethodni;
        novi = novi->prethodni;

        this->velicina++;
    }
}

template <typename Tip>
Tip Stek<Tip>::skini() {
    if (this->brojElemenata() == 0) throw std::logic_error("Prazan stek");

    Tip vrijednostPosljednjeg(this->top->vrijednost);
    Cvor* ispod(this->top->prethodni);

    delete this->top;
    this->top = ispod;

    this->velicina--;
    return vrijednostPosljednjeg;
}

// ==================================================================================================== END - STEK

// ==================================================================================================== BEGIN - ZAGLAVLJA

template <typename Tip>
void testBrojElemenata(Stek<Tip>);

template <typename Tip>
void testBrisi(Stek<Tip>);

template <typename Tip>
void testStavi(Stek<Tip>);

template <typename Tip>
void testSkini(Stek<Tip>);

template <typename Tip>
void testVrh(Stek<Tip>);

template <typename Tip>
void testStek(const Stek<Tip>&);

// ==================================================================================================== END - ZAGLAVLJA

int main() {
    Stek<int> stek;

    for (int i = 1; i <= 10; i++)
        stek.stavi(i);

    //testStek(stek);

    return 0;
}

// ==================================================================================================== BEGIN - TEST FUNKCIJE

template <typename Tip>
void dajInfo(Stek<Tip> stek) {
    std::cout << "[ broj elemenata = " << stek.brojElemenata() << " ] --> { ";
    while (stek.brojElemenata() != 0) std::cout << stek.skini() << " ";
    std::cout << "} " << std::endl;
}

template <typename Tip>
void testBrojElemenata(Stek<Tip> stek) {
    std::cout << std::string(65, '-') << " TEST brojElemenata()\n";

    dajInfo(stek); std::cout << stek.brojElemenata() << std::endl;
    stek.skini();
    dajInfo(stek); std::cout << stek.brojElemenata() << std::endl;
    stek.brisi();
    dajInfo(stek); std::cout << stek.brojElemenata() << std::endl;
}

template <typename Tip>
void testBrisi(Stek<Tip> stek) {
    std::cout << std::string(65, '-') << " TEST brisi()\n";

    dajInfo(stek);
    stek.brisi(); dajInfo(stek);
    stek.brisi(); dajInfo(stek);
}

template <typename Tip>
void testStavi(Stek<Tip> stek) {
    std::cout << std::string(65, '-') << " TEST stavi()\n";

    dajInfo(stek);
    stek.stavi(1); dajInfo(stek);
    stek.stavi(2); dajInfo(stek);
    stek.stavi(3); dajInfo(stek);

    while (stek.brojElemenata() != 0) stek.skini(); dajInfo(stek);

    stek.stavi(1); dajInfo(stek);
    stek.stavi(2); dajInfo(stek);
    stek.stavi(3); dajInfo(stek);
}

template <typename Tip>
void testSkini(Stek<Tip> stek) {
    std::cout << std::string(65, '-') << " TEST skini()\n";

    dajInfo(stek); stek.skini();
    dajInfo(stek); stek.skini();

    dajInfo(stek); while (stek.brojElemenata() != 4) stek.skini();
    dajInfo(stek); while (stek.brojElemenata() != 0) stek.skini();

    try {
        dajInfo(stek); stek.skini();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testVrh(Stek<Tip> stek) {
    std::cout << std::string(65, '-') << " TEST vrh()\n";

    dajInfo(stek); std::cout << stek.vrh() << std::endl;
    stek.skini();
    dajInfo(stek); std::cout << stek.vrh() << std::endl;

    while (stek.brojElemenata() != 1) stek.skini();

    dajInfo(stek); std::cout << stek.vrh() << std::endl;
    stek.stavi(10);
    dajInfo(stek); std::cout << stek.vrh() << std::endl;

    while (stek.brojElemenata() != 0) stek.skini();

    try {
        stek.vrh();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testStek(const Stek<Tip>& stek) {
    testBrojElemenata(stek);
    testBrisi(stek);
    testStavi(stek);
    testSkini(stek);
    testVrh(stek);
}

// ==================================================================================================== END - TEST FUNKCIJE