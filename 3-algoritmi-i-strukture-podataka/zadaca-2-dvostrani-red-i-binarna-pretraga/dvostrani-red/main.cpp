// ====================================================================================================
// NAPOMENA: Za pokretanje testnih funkcija ukloniti komentar u main() funkciji
// ====================================================================================================

#include <iostream>
#include <stdexcept>

// ==================================================================================================== BEGIN - DVOSTRANI RED

template <typename Tip>
class DvostraniRed {
    struct Cvor {
        Tip info;
        Cvor *previous, *next;

        Cvor (const Tip& info, Cvor* previousLocal, Cvor* nextLocal): info(info), previous(previousLocal), next(nextLocal) {
            if (previousLocal != nullptr) previousLocal->next = this;
            if (nextLocal != nullptr) nextLocal->previous = this;
        }

        Cvor (Cvor* previousLocal, Cvor* nextLocal): previous(previousLocal), next(nextLocal) {
            if (previousLocal != nullptr) previousLocal->next = this;
            if (nextLocal != nullptr) nextLocal->previous = this;
        }
    };

    int size;
    Cvor *begin, *end;

    // --------------------

    void kopiraj(const DvostraniRed&);
    void unisti(bool sve);

    Tip& daj(Cvor*);
    void stavi(const Tip&, Cvor*, Cvor*);
    Tip skini(Cvor*);

    // --------------------

public:
    DvostraniRed(): size(0) {
        this->begin = new Cvor(nullptr, nullptr);
        this->end = new Cvor(this->begin, nullptr);
    }

    DvostraniRed(const DvostraniRed& lista): size(0) {
        this->kopiraj(lista);
    }

    DvostraniRed& operator =(const DvostraniRed& lista) {
        if (&lista != this) { this->unisti(true); this->kopiraj(lista); }
        return *this;
    }

    DvostraniRed(DvostraniRed&&) = default;
    DvostraniRed& operator =(DvostraniRed&&) = default;

    ~DvostraniRed() { this->unisti(true); }

    // --------------------

    int brojElemenata() const { return this->size; }
    void brisi() { this->unisti(false); }

    Tip& celo() { return this->daj(this->begin->next); }
    Tip& vrh() { return this->daj(this->end->previous); }

    void staviNaCelo(const Tip& info) { this->stavi(info, this->begin, this->begin->next); }
    void staviNaVrh(const Tip& info) { this->stavi(info, this->end->previous, this->end); }

    Tip skiniSaCela() { return this->skini(this->begin->next); }
    Tip skiniSaVrha() { return this->skini(this->end->previous); }
};

// ------------------------------------------------------- BEGIN - PRIVATNI DIO KLASE

template <typename Tip>
void DvostraniRed<Tip>::kopiraj(const DvostraniRed& lista) {
    this->begin = new Cvor(nullptr, nullptr);
    this->end = new Cvor(this->begin, nullptr);

    Cvor* element = lista.begin->next;
    while (element != lista.end) {
        this->staviNaVrh(element->info);
        element = element->next;
    }
}

template <typename Tip>
void DvostraniRed<Tip>::unisti(bool sve) {
    while (this->brojElemenata() != 0) this->skiniSaVrha();

    if (sve) {
        delete this->begin; this->begin = nullptr;
        delete this->end; this->end = nullptr;
    }
}

// --------------------

template <typename Tip>
Tip& DvostraniRed<Tip>::daj(Cvor* cvor) {
    if (this->brojElemenata() == 0) throw std::logic_error("Red je prazan");
    return cvor->info;
}

template <typename Tip>
void DvostraniRed<Tip>::stavi(const Tip& info, Cvor* prvi, Cvor* drugi) {
    if (this->brojElemenata() == 0) new Cvor(info, this->begin, this->end);
    else new Cvor(info, prvi, drugi);
    this->size++;
}

template <typename Tip>
Tip DvostraniRed<Tip>::skini(Cvor* cvor) {
    if (this->brojElemenata() == 0) throw std::logic_error("Red je prazan");

    Tip vrijednost = cvor->info;

    cvor->next->previous = cvor->previous;
    cvor->previous->next = cvor->next;

    delete cvor;
    this->size--;

    return vrijednost;
}

// ------------------------------------------------------- END - PRIVATNI DIO KLASE

// ==================================================================================================== END - DVOSTRANI RED

// ==================================================================================================== BEGIN - ZAGLAVLJA

template <typename Tip>
void testBrojElemenata(DvostraniRed<Tip>);

template <typename Tip>
void testBrisi(DvostraniRed<Tip>);

template <typename Tip>
void testCelo(DvostraniRed<Tip>);

template <typename Tip>
void testVrh(DvostraniRed<Tip>);

template <typename Tip>
void testStaviNaCelo(DvostraniRed<Tip>);

template <typename Tip>
void testStaviNaVrh(DvostraniRed<Tip>);

template <typename Tip>
void testSkiniSaCela(DvostraniRed<Tip>);

template <typename Tip>
void testSkiniSaVrha(DvostraniRed<Tip>);

template <typename Tip>
void testDvostraniRed(const DvostraniRed<Tip>&);

// ==================================================================================================== END - ZAGLAVLJA

int main() {
    DvostraniRed<int> red;

    for (int i = 1; i <= 10; i++)
        red.staviNaVrh(i);

    //testDvostraniRed(red);

    return 0;
}

// ==================================================================================================== BEGIN - TEST FUNKCIJE

template <typename Tip>
void dajInfo(DvostraniRed<Tip> red) {
    std::cout << "[ broj elemenata = " << red.brojElemenata() << " ] --> { ";
    while (red.brojElemenata() != 0) std::cout << red.skiniSaCela() << " ";
    std::cout << "} " << std::endl;
}

template <typename Tip>
void testBrojElemenata(DvostraniRed<Tip> red) {
    std::cout << std::string(65, '-') << " TEST brojElemenata()\n";

    dajInfo(red); std::cout << red.brojElemenata() << std::endl;
    red.skiniSaCela();
    dajInfo(red); std::cout << red.brojElemenata() << std::endl;
    red.brisi();
    dajInfo(red); std::cout << red.brojElemenata() << std::endl;
    red.staviNaVrh(1);
    dajInfo(red); std::cout << red.brojElemenata() << std::endl;
}

template <typename Tip>
void testBrisi(DvostraniRed<Tip> red) {
    std::cout << std::string(65, '-') << " TEST brisi()\n";

    dajInfo(red);
    red.brisi(); dajInfo(red);
    red.brisi(); dajInfo(red);
}

template <typename Tip>
void testCelo(DvostraniRed<Tip> red) {
    std::cout << std::string(65, '-') << " TEST celo()\n";

    dajInfo(red); std::cout << red.celo() << std::endl;
    red.skiniSaCela();
    dajInfo(red); std::cout << red.celo() << std::endl;

    while (red.brojElemenata() != 1) red.skiniSaCela();

    dajInfo(red); std::cout << red.celo() << std::endl;
    red.staviNaCelo(1);
    dajInfo(red); std::cout << red.celo() << std::endl;

    while (red.brojElemenata() != 0) red.skiniSaCela();

    try {
        dajInfo(red); red.celo();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testVrh(DvostraniRed<Tip> red) {
    std::cout << std::string(65, '-') << " TEST vrh()\n";

    dajInfo(red); std::cout << red.vrh() << std::endl;
    red.skiniSaVrha();
    dajInfo(red); std::cout << red.vrh() << std::endl;

    while (red.brojElemenata() != 1) red.skiniSaVrha();

    dajInfo(red); std::cout << red.vrh() << std::endl;
    red.staviNaVrh(10);
    dajInfo(red); std::cout << red.vrh() << std::endl;

    while (red.brojElemenata() != 0) red.skiniSaVrha();

    try {
        dajInfo(red); red.vrh();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testStaviNaCelo(DvostraniRed<Tip> red) {
    std::cout << std::string(65, '-') << " TEST staviNaCelo()\n";

    dajInfo(red);
    red.staviNaCelo(1); dajInfo(red);
    red.staviNaCelo(2); dajInfo(red);
    red.staviNaCelo(3); dajInfo(red);

    while (red.brojElemenata() != 0) red.skiniSaCela(); dajInfo(red);

    red.staviNaCelo(1); dajInfo(red);
    red.staviNaCelo(2); dajInfo(red);
    red.staviNaCelo(3); dajInfo(red);
}

template <typename Tip>
void testStaviNaVrh(DvostraniRed<Tip> red) {
    std::cout << std::string(65, '-') << " TEST staviNaVrh()\n";

    dajInfo(red);
    red.staviNaVrh(1); dajInfo(red);
    red.staviNaVrh(2); dajInfo(red);
    red.staviNaVrh(3); dajInfo(red);

    while (red.brojElemenata() != 0) red.skiniSaVrha(); dajInfo(red);

    red.staviNaVrh(1); dajInfo(red);
    red.staviNaVrh(2); dajInfo(red);
    red.staviNaVrh(3); dajInfo(red);
}

template <typename Tip>
void testSkiniSaCela(DvostraniRed<Tip> red) {
    std::cout << std::string(65, '-') << " TEST skiniSaCela()\n";

    dajInfo(red); red.skiniSaCela();
    dajInfo(red); red.skiniSaCela();

    dajInfo(red); while (red.brojElemenata() != 4) red.skiniSaCela();
    dajInfo(red); while (red.brojElemenata() != 0) red.skiniSaCela();

    try {
        dajInfo(red); red.skiniSaCela();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testSkiniSaVrha(DvostraniRed<Tip> red) {
    std::cout << std::string(65, '-') << " TEST skiniSaVrha()\n";

    dajInfo(red); red.skiniSaVrha();
    dajInfo(red); red.skiniSaVrha();

    dajInfo(red); while (red.brojElemenata() != 4) red.skiniSaVrha();
    dajInfo(red); while (red.brojElemenata() != 0) red.skiniSaVrha();

    try {
        dajInfo(red); red.skiniSaVrha();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testDvostraniRed(const DvostraniRed<Tip>& red) {
    testBrojElemenata(red);
    testBrisi(red);
    testCelo(red);
    testVrh(red);
    testStaviNaCelo(red);
    testStaviNaVrh(red);
    testSkiniSaCela(red);
    testSkiniSaVrha(red);
}

// ==================================================================================================== END - TEST FUNKCIJE