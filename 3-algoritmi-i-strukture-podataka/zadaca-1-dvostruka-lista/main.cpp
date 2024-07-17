// ====================================================================================================
// NAPOMENA: Za pokretanje testnih funkcija ukloniti komentar u main() funkciji
// ====================================================================================================

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <memory>
#include <typeinfo>

using std::cout;

template <typename Tip>
class Iterator;

// ==================================================================================================== BEGIN - LISTA

template <typename Tip>
class Lista {
public:
    Lista() = default;

    Lista(const Lista&) = default;
    Lista& operator =(const Lista&) = default;

    Lista(Lista&&) = default;
    Lista& operator =(Lista&&) = default;

    virtual ~Lista() = default;

    // --------------------

    virtual int brojElemenata() const = 0;

    virtual Tip& trenutni() = 0;
    virtual Tip trenutni() const = 0;

    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;

    virtual void pocetak() = 0;
    virtual void kraj() = 0;

    virtual void obrisi() = 0;

    virtual void dodajIspred(const Tip&) = 0;
    virtual void dodajIza(const Tip&) = 0;

    virtual Tip& operator [](int) = 0;
    virtual Tip operator [](int) const = 0;
};

// ==================================================================================================== END - LISTA

// ==================================================================================================== BEGIN - DVOSTRUKA LISTA

template <typename Tip>
class DvostrukaLista: public Lista<Tip> {
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
    Cvor *begin, *end, *current;

    // --------------------

    void kopiraj(const DvostrukaLista&);
    void unisti();

    // --------------------

public:
    DvostrukaLista(): size(0), current(nullptr) {
        this->begin = new Cvor(nullptr, nullptr);
        this->end = new Cvor(this->begin, nullptr);
    }

    DvostrukaLista(const DvostrukaLista& lista): size(0), current(nullptr) {
        this->kopiraj(lista);
    }

    DvostrukaLista& operator =(const DvostrukaLista& lista) {
        if (&lista != this) { this->unisti(); this->kopiraj(lista); }
        return *this;
    }

    DvostrukaLista(DvostrukaLista&&) = default;
    DvostrukaLista& operator =(DvostrukaLista&&) = default;

    ~DvostrukaLista() { this->unisti(); }

    // --------------------

    friend class Iterator<Tip>;

    // --------------------

    int brojElemenata() const override { return this->size; }

    Tip& trenutni() override;
    Tip trenutni() const override;

    bool prethodni() override;
    bool sljedeci() override;

    void pocetak() override;
    void kraj() override;

    void obrisi() override;

    void dodajIspred(const Tip&) override;
    void dodajIza(const Tip&) override;

    Tip& operator [](int) override;
    Tip operator [](int) const override;
};

// ------------------------------------------------------- BEGIN - PRIVATNI DIO KLASE

template <typename Tip>
void DvostrukaLista<Tip>::unisti() {
    while (this->begin != nullptr) {
        this->current = this->begin;
        this->begin = this->begin->next;
        delete this->current;
    }

    this->size = 0;
    this->end = this->current = nullptr;
}

template <typename Tip>
void DvostrukaLista<Tip>::kopiraj(const DvostrukaLista& lista) {
    this->begin = new Cvor(nullptr, nullptr);
    this->end = new Cvor(this->begin, nullptr);

    Cvor* element = lista.begin->next;
    while (element != lista.end) {
        this->dodajIza(element->info);
        this->sljedeci();
        element = element->next;
    }

    if (this->brojElemenata() != 0) this->pocetak();
}

// ------------------------------------------------------- END - PRIVATNI DIO KLASE

// ------------------------------------------------------- BEGIN - IMPLEMENTACIJA INTERFEJSA

template <typename Tip>
Tip& DvostrukaLista<Tip>::trenutni() {
    if (this->brojElemenata() == 0) throw std::logic_error("Lista je prazna");
    return this->current->info;
}

template <typename Tip>
Tip DvostrukaLista<Tip>::trenutni() const {
    if (this->brojElemenata() == 0) throw std::logic_error("Lista je prazna");
    return this->current->info;
}

template <typename Tip>
bool DvostrukaLista<Tip>::prethodni() {
    if (this->brojElemenata() == 0) throw std::logic_error("Lista je prazna");
    if (this->current == this->begin->next) return false;

    this->current = this->current->previous;
    return true;
}

template <typename Tip>
bool DvostrukaLista<Tip>::sljedeci() {
    if (this->brojElemenata() == 0) throw std::logic_error("Lista je prazna");
    if (this->current == this->end->previous) return false;

    this->current = this->current->next;
    return true;
}

template <typename Tip>
void DvostrukaLista<Tip>::pocetak() {
    if (this->brojElemenata() == 0) throw std::logic_error("Lista je prazna");
    this->current = this->begin->next;
}

template <typename Tip>
void DvostrukaLista<Tip>::kraj() {
    if (this->brojElemenata() == 0) throw std::logic_error("Lista je prazna");
    this->current = this->end->previous;
}

template <typename Tip>
void DvostrukaLista<Tip>::obrisi() {
    if (this->brojElemenata() == 0) throw std::logic_error("Lista je prazna");

    Cvor* tekuci = this->current;
    if (!this->sljedeci() && !this->prethodni()) this->current = nullptr;

    tekuci->next->previous = tekuci->previous;
    tekuci->previous->next = tekuci->next;

    delete tekuci;
    this->size--;
}

template <typename Tip>
void DvostrukaLista<Tip>::dodajIspred(const Tip& info) {
    if (this->brojElemenata() == 0) this->current = new Cvor(info, this->begin, this->end);
    else new Cvor(info, this->current->previous, this->current);
    this->size++;
}

template <typename Tip>
void DvostrukaLista<Tip>::dodajIza(const Tip& info) {
    if (this->brojElemenata() == 0) this->current = new Cvor(info, this->begin, this->end);
    else new Cvor(info, this->current, this->current->next);
    this->size++;
}

template <typename Tip>
Tip& DvostrukaLista<Tip>::operator [](int indeks) {
    if (indeks < 0 || indeks >= this->brojElemenata()) throw std::range_error("Indeks nije valjan");

    Cvor* element(this->begin->next);
    for (int i = 0; i < indeks; i++)
        element = element->next;

    return element->info;
}

template <typename Tip>
Tip DvostrukaLista<Tip>::operator [](int indeks) const {
    if (indeks < 0 || indeks >= this->brojElemenata()) throw std::range_error("Indeks nije valjan");

    Cvor* element(this->begin->next);
    for (int i = 0; i < indeks; i++)
        element = element->next;

    return element->info;
}

// ------------------------------------------------------- END - IMPLEMENTACIJA INTERFEJSA

// ==================================================================================================== END - DVOSTRUKA LISTA

// ==================================================================================================== BEGIN - ITERATOR

template <typename Tip>
class Iterator {
    const DvostrukaLista<Tip>* lista;
    typename DvostrukaLista<Tip>::Cvor* tekuci;

public:
    Iterator(const DvostrukaLista<Tip>& lista): lista(&lista), tekuci(nullptr) {}

    // --------------------

    void setBegin() { this->tekuci = this->lista->begin->next; }
    void setEnd() { this->tekuci = this->lista->end->previous; }

    Tip& operator *() { return this->tekuci->info; }

    Iterator<Tip> operator ++(int) {
        Iterator<Tip> pomocni(*this);

        if (this->tekuci == this->lista->end) this->tekuci = nullptr;
        else this->tekuci = this->tekuci->next;

        return pomocni;
    }

    Iterator<Tip> operator --(int) {
        Iterator<Tip> pomocni(*this);

        if (this->tekuci == this->lista->begin) this->tekuci = nullptr;
        else this->tekuci = this->tekuci->previous;

        return pomocni;
    }

    bool operator ==(const Iterator& it) { return this->tekuci == it.tekuci; }
    bool operator !=(const Iterator& it) { return this->tekuci != it.tekuci; }

    Iterator<Tip> begin() { // prije pocetka (legalno zbog fiktivnog cvora)
        Iterator<Tip> it(*lista);
        it.tekuci = this->lista->begin;
        return it;
    }

    Iterator<Tip> end() { // prije kraja (legalno zbog fiktivnog cvora)
        Iterator<Tip> it(*lista);
        it.tekuci = this->lista->end;
        return it;
    }
};

// ==================================================================================================== END - ITERATOR

template <typename Tip>
Tip dajMaksimum(const Lista<Tip>& lista) {
    Iterator<Tip> it(dynamic_cast<const DvostrukaLista<Tip>&>(lista));
    it.setBegin();

    Tip maks = *it;

    while (it != it.end()) {
        if (*it > maks) maks = *it;
        it++;
    }

    return maks;
}

// ==================================================================================================== BEGIN - ZAGLAVLJA

template <typename Tip>
void testDajMaksimum(DvostrukaLista<Tip>);

template <typename Tip>
void testBrojElemenata(DvostrukaLista<Tip>);

template <typename Tip>
void testTrenutni(DvostrukaLista<Tip>);

template <typename Tip>
void testPrethodni(DvostrukaLista<Tip>);

template <typename Tip>
void testSljedeci(DvostrukaLista<Tip>);

template <typename Tip>
void testPocetak(DvostrukaLista<Tip>);

template <typename Tip>
void testKraj(DvostrukaLista<Tip>);

template <typename Tip>
void testObrisi(DvostrukaLista<Tip>);

template <typename Tip>
void testDodajIspred(DvostrukaLista<Tip>);

template <typename Tip>
void testDodajIza(DvostrukaLista<Tip>);

template <typename Tip>
void testOperator(DvostrukaLista<Tip>);

template <typename Tip>
void testLista(const DvostrukaLista<Tip>&);

// ==================================================================================================== END - ZAGLAVLJA

int main() {
    DvostrukaLista<int> lista;

    for (int i = 1; i <= 10; i++)
        lista.dodajIza(i);

    //testLista(lista);

    return 0;
}

// ==================================================================================================== BEGIN - TEST FUNKCIJE

template <typename Tip>
void dajInfo(DvostrukaLista<Tip> lista) {
    if (lista.brojElemenata() != 0) {
        std::cout << "[ trenutni = " << lista.trenutni(); lista.pocetak();
        std::cout << " | pocetak = " << lista.trenutni(); lista.kraj();
        std::cout << " | kraj = " << lista.trenutni() << " ] ---";
    }

    std::cout << " { ";
    for (int i = 0; i < lista.brojElemenata(); i++) std::cout << lista[i] << " ";
    std::cout << "} " << std::endl;
}

template <typename Tip>
void testDajMaksimum(DvostrukaLista<Tip> lista) {
    std::cout << std::string(75, '-') << " TEST dajMaksimum()\n";

    dajInfo(lista);
    std::cout << dajMaksimum(lista) << std::endl;

    while (lista.brojElemenata() != 0) lista.obrisi();

    lista.dodajIspred(9);
    lista.dodajIspred(2);
    lista.dodajIspred(4);
    lista.dodajIspred(3);
    lista.dodajIspred(7);

    dajInfo(lista);
    std::cout << dajMaksimum(lista) << std::endl;

    while (lista.brojElemenata() != 0) lista.obrisi();

    lista.dodajIspred(2);
    lista.dodajIspred(8);
    lista.dodajIspred(4);
    lista.dodajIspred(3);
    lista.dodajIspred(7);

    dajInfo(lista);
    std::cout << dajMaksimum(lista) << std::endl;
}

template <typename Tip>
void testBrojElemenata(DvostrukaLista<Tip> lista) {
    std::cout << std::string(75, '-') << " TEST brojElemenata()\n";

    dajInfo(lista); std::cout << lista.brojElemenata() << std::endl;
    lista.obrisi();
    dajInfo(lista); std::cout << lista.brojElemenata() << std::endl;
}

template <typename Tip>
void testTrenutni(DvostrukaLista<Tip> lista) {
    std::cout << std::string(75, '-') << " TEST trenutni()\n";

    dajInfo(lista);
    lista.trenutni() = 0; dajInfo(lista);

    try {
        const DvostrukaLista<int> test;
        std::cout << test.trenutni();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testPrethodni(DvostrukaLista<Tip> lista) {
    std::cout << std::string(75, '-') << " TEST prethodni()\n";

    dajInfo(lista);
    lista.kraj(); dajInfo(lista);
    lista.prethodni(); dajInfo(lista);

    while (lista.prethodni());
    dajInfo(lista);

    try {
        DvostrukaLista<int> test;
        test.prethodni();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testSljedeci(DvostrukaLista<Tip> lista) {
    std::cout << std::string(75, '-') << " TEST sljedeci()\n";

    dajInfo(lista);
    lista.pocetak(); dajInfo(lista);
    lista.sljedeci(); dajInfo(lista);

    while (lista.sljedeci());
    dajInfo(lista);

    try {
        DvostrukaLista<int> test;
        test.sljedeci();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testPocetak(DvostrukaLista<Tip> lista) {
    std::cout << std::string(75, '-') << " TEST pocetak()\n";

    dajInfo(lista);
    lista.pocetak(); dajInfo(lista);

    try {
        DvostrukaLista<int> test;
        test.pocetak();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testKraj(DvostrukaLista<Tip> lista) {
    std::cout << std::string(75, '-') << " TEST kraj()\n";

    dajInfo(lista);
    lista.kraj(); dajInfo(lista);

    try {
        DvostrukaLista<int> test;
        test.kraj();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testObrisi(DvostrukaLista<Tip> lista) {
    std::cout << std::string(75, '-') << " TEST obrisi()\n";

    lista.pocetak(); dajInfo(lista); lista.obrisi(); dajInfo(lista);
    lista.sljedeci(); dajInfo(lista); lista.obrisi(); dajInfo(lista);
    lista.kraj(); dajInfo(lista); lista.obrisi(); dajInfo(lista);
    lista.prethodni(); dajInfo(lista); lista.obrisi(); dajInfo(lista);

    while (lista.brojElemenata() != 4) lista.obrisi(); dajInfo(lista); lista.pocetak();
    while (lista.brojElemenata() != 1) lista.obrisi(); dajInfo(lista); lista.obrisi(); dajInfo(lista);

    try {
        lista.obrisi();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testDodajIspred(DvostrukaLista<Tip> lista) {
    std::cout << std::string(75, '-') << " TEST dodajIspred()\n";

    while (lista.brojElemenata() != 0) lista.obrisi(); dajInfo(lista);

    lista.dodajIspred(1); dajInfo(lista);
    lista.dodajIspred(2); dajInfo(lista);
    lista.dodajIspred(3); dajInfo(lista);

    lista.obrisi(); lista.pocetak(); dajInfo(lista);

    lista.dodajIspred(4); dajInfo(lista);
    lista.dodajIspred(5); dajInfo(lista);
}

template <typename Tip>
void testDodajIza(DvostrukaLista<Tip> lista) {
    std::cout << std::string(75, '-') << " TEST dodajIza()\n";

    while (lista.brojElemenata() != 0) lista.obrisi(); dajInfo(lista);

    lista.dodajIza(1); dajInfo(lista);
    lista.dodajIza(2); dajInfo(lista);
    lista.dodajIza(3); dajInfo(lista);

    lista.obrisi(); lista.kraj(); dajInfo(lista);

    lista.dodajIza(4); dajInfo(lista);
    lista.dodajIza(5); dajInfo(lista);
}

template <typename Tip>
void testOperator(DvostrukaLista<Tip> lista) {
    std::cout << std::string(75, '-') << " TEST operator []()\n";

    dajInfo(lista);
    for (int i = 0; i < lista.brojElemenata(); i++)
        std::cout << lista[i] << " ";

    try {
        std::cout << std::endl << lista[-1];
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }

    try {
        std::cout << lista[lista.brojElemenata()];
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testLista(const DvostrukaLista<Tip>& lista) {
    testDajMaksimum(lista);
    testBrojElemenata(lista);
    testTrenutni(lista);
    testPrethodni(lista);
    testSljedeci(lista);
    testPocetak(lista);
    testKraj(lista);
    testObrisi(lista);
    testDodajIspred(lista);
    testDodajIza(lista);
    testOperator(lista);
}

// ==================================================================================================== END - TEST FUNKCIJE