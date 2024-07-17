// ====================================================================================================
// NAPOMENA: Za pokretanje testnih funkcija ukloniti komentar u main() funkciji
// ====================================================================================================

#include <iostream>
#include <stdexcept>

using std::cout; // zbog AT

// ==================================================================================================== BEGIN - RED

template <typename Tip>
class Red {
    struct Cvor {
        Tip info;
        Cvor* sljedeci;

        Cvor(const Tip& info, Cvor* sljedeci): info(info), sljedeci(sljedeci) {}
    };

    int velicina;
    Cvor *pocetak, *kraj;

    // --------------------

    void kopiraj(const Red& red) {
        Cvor* pomocni(red.pocetak);

        while (pomocni != nullptr) {
            this->stavi(pomocni->info);
            pomocni = pomocni->sljedeci;
        }
    }

    // --------------------

public:
    Red(): velicina(0), pocetak(nullptr), kraj(nullptr) {}

    Red(const Red& red): velicina(0), pocetak(nullptr), kraj(nullptr) {
        this->kopiraj(red);
    }

    Red& operator =(const Red& red) {
        if (&red != this) {
            this->brisi();
            this->kopiraj(red);
        }

        return *this;
    }

    ~Red() { this->brisi(); }

    // --------------------

    void brisi() {
        while (this->brojElemenata() != 0)
            this->skini();
    }

    void stavi(const Tip& info) {
        Cvor* novi = new Cvor(info, nullptr);

        if (this->brojElemenata() == 0) this->pocetak = this->kraj = novi;
        else { this->kraj->sljedeci = novi; this->kraj = novi; }

        this->velicina++;
    }

    Tip skini() {
        if (this->brojElemenata() == 0) throw std::logic_error("Prazan red");

        Tip pomocnaInfo(this->pocetak->info);
        Cvor* pomocnaCvor(this->pocetak);

        if (this->pocetak == this->kraj) this->pocetak = this->kraj = nullptr;
        else this->pocetak = this->pocetak->sljedeci;

        delete pomocnaCvor;
        this->velicina--;

        return pomocnaInfo;
    }

    Tip& celo() {
        if (this->brojElemenata() == 0) throw std::logic_error("Prazan red");
        return this->pocetak->info;
    }

    int brojElemenata() { return this->velicina; }
};

// ==================================================================================================== END - RED

// ==================================================================================================== BEGIN - ZAGLAVLJA

template <typename Tip>
void testBrojElemenata(Red<Tip>);

template <typename Tip>
void testBrisi(Red<Tip>);

template <typename Tip>
void testStavi(Red<Tip>);

template <typename Tip>
void testSkini(Red<Tip>);

template <typename Tip>
void testVrh(Red<Tip>);

template <typename Tip>
void testRed(const Red<Tip>&);

// ==================================================================================================== END - ZAGLAVLJA

int main() {
    Red<int> red;

    for (int i = 1; i <= 10; i++)
        red.stavi(i);

    //testRed(red);

    return 0;
}

// ==================================================================================================== BEGIN - TEST FUNKCIJE

template <typename Tip>
void dajInfo(Red<Tip> red) {
    std::cout << "[ broj elemenata = " << red.brojElemenata() << " ] --> { ";
    while (red.brojElemenata() != 0) std::cout << red.skini() << " ";
    std::cout << "} " << std::endl;
}

template <typename Tip>
void testBrojElemenata(Red<Tip> red) {
    std::cout << std::string(65, '-') << " TEST brojElemenata()\n";

    dajInfo(red); std::cout << red.brojElemenata() << std::endl;
    red.skini();
    dajInfo(red); std::cout << red.brojElemenata() << std::endl;
    red.brisi();
    dajInfo(red); std::cout << red.brojElemenata() << std::endl;
}

template <typename Tip>
void testBrisi(Red<Tip> red) {
    std::cout << std::string(65, '-') << " TEST brisi()\n";

    dajInfo(red);
    red.brisi(); dajInfo(red);
    red.brisi(); dajInfo(red);
}

template <typename Tip>
void testStavi(Red<Tip> red) {
    std::cout << std::string(65, '-') << " TEST stavi()\n";

    dajInfo(red);
    red.stavi(1); dajInfo(red);
    red.stavi(2); dajInfo(red);
    red.stavi(3); dajInfo(red);

    while (red.brojElemenata() != 0) red.skini(); dajInfo(red);

    red.stavi(1); dajInfo(red);
    red.stavi(2); dajInfo(red);
    red.stavi(3); dajInfo(red);
}

template <typename Tip>
void testSkini(Red<Tip> red) {
    std::cout << std::string(65, '-') << " TEST skini()\n";

    dajInfo(red); red.skini();
    dajInfo(red); red.skini();

    dajInfo(red); while (red.brojElemenata() != 4) red.skini();
    dajInfo(red); while (red.brojElemenata() != 0) red.skini();

    try {
        dajInfo(red); red.skini();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testCelo(Red<Tip> red) {
    std::cout << std::string(65, '-') << " TEST celo()\n";

    dajInfo(red); std::cout << red.celo() << std::endl;
    red.skini();
    dajInfo(red); std::cout << red.celo() << std::endl;

    while (red.brojElemenata() != 1) red.skini();

    dajInfo(red); std::cout << red.celo() << std::endl;
    red.stavi(1);
    dajInfo(red); std::cout << red.celo() << std::endl;

    while (red.brojElemenata() != 0) red.skini();

    try {
        dajInfo(red); red.celo();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testRed(const Red<Tip>& red) {
    testBrojElemenata(red);
    testBrisi(red);
    testStavi(red);
    testSkini(red);
    testCelo(red);
}

// ==================================================================================================== END - TEST FUNKCIJE