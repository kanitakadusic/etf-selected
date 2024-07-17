// ====================================================================================================
// NAPOMENA: Za pokretanje testnih funkcija ukloniti komentare u main() funkciji
// ====================================================================================================

#include <iostream>
#include <stdexcept>
#include <vector>

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

    Stek& operator =(const Stek& stek) {
        if (&stek != this) { this->brisi(); this->kopiraj(stek); }
        return *this;
    }

    ~Stek() { this->brisi(); }

    // --------------------

    int brojElemenata() const { return this->velicina; }

    void brisi() {
        while (this->brojElemenata() != 0)
            this->skini();
    }

    Tip& vrh() {
        if (this->brojElemenata() == 0) throw std::logic_error("Prazan stek");
        return this->top->vrijednost;
    }

    void stavi(const Tip& vrijednost) {
        this->top = new Cvor(vrijednost, this->top);
        this->velicina++;
    }

    Tip skini();
};

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

// ==================================================================================================== BEGIN - PRETRAGA

int binarnoPretrazivanje(const std::vector<int>& vektor, int trazeni) {
    int dno = 0, vrh = vektor.size() - 1, sredina;

    while (vrh >= dno) {
        sredina = (dno + vrh) / 2;

        if (vektor[sredina] == trazeni) return sredina;
        else if (vektor[sredina] < trazeni) dno = sredina + 1;
        else vrh = sredina - 1;
    }

    return -1;
}

void pretraga(Stek<std::vector<int>>& stek, int trazeni) {
    if (stek.brojElemenata() == 0) {
        std::cout << "Nema elementa";
        return;
    }

    std::vector<int> vektor(stek.vrh());

    if (!vektor.empty() && vektor[0] <= trazeni) {
        int indeks = binarnoPretrazivanje(vektor, trazeni);

        if (indeks == -1) std::cout << "Nema elementa";
        else std::cout << indeks << " " << stek.brojElemenata() - 1;

        return;
    }

    stek.skini();

    pretraga(stek, trazeni);
    stek.stavi(vektor);
}

// ==================================================================================================== END - PRETRAGA

// ==================================================================================================== BEGIN - TEST FUNKCIJE

void testPositive() {
    std::cout << std::endl << std::string(40, '-') << " TEST positive()\n";

    Stek<std::vector<int>> stek;
    stek.stavi({1, 3, 5, 7});
    stek.stavi({});
    stek.stavi({12, 14});
    stek.stavi({23, 24, 25, 26, 27, 29});
    stek.stavi({});
    stek.stavi({38});

    pretraga(stek, 5);
}

void testNegative() {
    std::cout << std::endl << std::string(40, '-') << " TEST negative()\n";

    Stek<std::vector<int>> stek;
    stek.stavi({1, 3, 5, 7});
    stek.stavi({});
    stek.stavi({12, 14});
    stek.stavi({23, 24, 25, 26, 27, 29});
    stek.stavi({});
    stek.stavi({38});

    pretraga(stek, 0);
}

void testStanje() {
    std::cout << std::endl << std::string(40, '-') << " TEST stanje()\n";

    Stek<std::vector<int>> stek;
    stek.stavi({1, 3, 5, 7});
    stek.stavi({});
    stek.stavi({12, 14});
    stek.stavi({23, 24, 25, 26, 27, 29});
    stek.stavi({});
    stek.stavi({38});

    pretraga(stek, 12); std::cout << std::endl;

    while (stek.brojElemenata() != 0) {
        auto vektor(stek.skini());
        for (int e : vektor) std::cout << e << " ";
    }
}

// ==================================================================================================== END - TEST FUNKCIJE

int main() {
    //testPositive();
    //testNegative();
    //testStanje();

    return 0;
}