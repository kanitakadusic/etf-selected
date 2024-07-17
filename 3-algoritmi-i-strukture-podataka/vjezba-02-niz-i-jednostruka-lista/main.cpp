// ====================================================================================================
// NAPOMENA: Za pokretanje testnih funkcija ukloniti komentare u main() funkciji
// ====================================================================================================

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <memory>
#include <typeinfo>

// ==================================================================================================== BEGIN - LISTA

template <typename Tip>
class Lista {
public:
    Lista() = default;
    //Lista(std::initializer_list<Tip>);

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

// ==================================================================================================== BEGIN - NIZ LISTA

template <typename Tip>
class NizLista: public Lista<Tip> {
    int kapacitet, duzina, tekuci;
    Tip** elementi;

    // --------------------

    static void oslobodiMemoriju(Tip** elementi, int duzina);
    void isprazniListu();

    // --------------------

public:
    explicit NizLista(int kapacitet = 2);
    NizLista(std::initializer_list<Tip>);

    NizLista(const NizLista&);
    NizLista& operator =(const NizLista&);

    NizLista(NizLista&&) noexcept;
    NizLista& operator =(NizLista&&) noexcept;

    ~NizLista();

    // --------------------

    int brojElemenata() const override { return this->duzina; }

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

// ------------------------------------------------------- BEGIN - UPRAVLJACKI DIO KLASE

template <typename Tip>
NizLista<Tip>::NizLista(int kapacitet):
        kapacitet(kapacitet <= 0 ? 2 : kapacitet), duzina(0), tekuci(0), elementi(new Tip*[kapacitet <= 0 ? 2 : kapacitet]{}) {
    if (kapacitet < 0) throw std::logic_error("Kapacitet nije valjan");
}

template <typename Tip>
NizLista<Tip>::NizLista(std::initializer_list<Tip> niz):
        kapacitet(niz.size() == 0 ? 2 : niz.size()), duzina(niz.size()), tekuci(0), elementi(new Tip*[niz.size() == 0 ? 2 : niz.size()]{}) {
    typename std::initializer_list<Tip>::iterator i_niz = niz.begin();

    for (int i = 0; i < niz.size(); i++)
        try {
            this->elementi[i] = new Tip{*i_niz++};
        } catch (...) {
            oslobodiMemoriju(this->elementi, i); throw;
        }
}

template <typename Tip>
NizLista<Tip>::NizLista(const NizLista& niz):
        kapacitet(niz.kapacitet), duzina(niz.duzina), tekuci(0), elementi(new Tip*[niz.kapacitet]{}) {
    for (int i = 0; i < niz.duzina; i++)
        try {
            this->elementi[i] = new Tip{*niz.elementi[i]};
        } catch (...) {
            oslobodiMemoriju(this->elementi, i); throw;
        }
}

template <typename Tip>
NizLista<Tip>& NizLista<Tip>::operator =(const NizLista& niz) {
    if (niz.kapacitet > this->kapacitet) {
        auto novi_elementi = new Tip*[niz.kapacitet]{};

        for (int i = 0; i < niz.duzina; i++)
            try {
                novi_elementi[i] = new Tip{*niz.elementi[i]};
            } catch (...) {
                oslobodiMemoriju(novi_elementi, i); throw;
            }

        oslobodiMemoriju(this->elementi, this->duzina);

        this->kapacitet = niz.kapacitet;
        this->elementi = novi_elementi;
    } else if (&niz != this) {
        isprazniListu();

        for (int i = 0; i < niz.duzina; i++)
            try {
                this->elementi[i] = new Tip{*niz.elementi[i]};
            } catch (...) {
                this->duzina = i - 1;
                throw std::logic_error("Neuspjela dodjela - nekonzistentni podaci");
            }
    }

    this->duzina = niz.duzina;
    this->tekuci = 0;

    return *this;
}

template <typename Tip>
NizLista<Tip>::NizLista(NizLista&& niz) noexcept:
        kapacitet(niz.kapacitet), duzina(niz.duzina), tekuci(0), elementi(niz.elementi) {
    niz.duzina = 0; niz.elementi = nullptr;
}

template <typename Tip>
NizLista<Tip>& NizLista<Tip>::operator =(NizLista&& niz) noexcept {
    std::swap(this->kapacitet, niz.kapacitet);
    std::swap(this->duzina, niz.duzina);
    std::swap(this->elementi, niz.elementi);

    this->tekuci = 0;
    return *this;
}

template <typename Tip>
NizLista<Tip>::~NizLista() {
    oslobodiMemoriju(this->elementi, this->duzina);
}

// ------------------------------------------------------- END - UPRAVLJACKI DIO KLASE

// ------------------------------------------------------- BEGIN - PRIVATNI DIO KLASE

template <typename Tip>
void NizLista<Tip>::oslobodiMemoriju(Tip** elementi, int duzina) {
    for (int i = 0; i < duzina; i++) delete elementi[i];
    delete[] elementi;
}

template <typename Tip>
void NizLista<Tip>::isprazniListu() {
    std::for_each(this->elementi, this->elementi + this->duzina, [](Tip*& element) { delete element; element = nullptr; });
    this->duzina = 0;
}

// ------------------------------------------------------- END - PRIVATNI DIO KLASE

// ------------------------------------------------------- BEGIN - IMPLEMENTACIJA INTERFEJSA

template <typename Tip>
Tip& NizLista<Tip>::trenutni() {
    if (this->brojElemenata() == 0) throw std::logic_error("Niz je prazan");
    return *this->elementi[this->tekuci];
}

template <typename Tip>
Tip NizLista<Tip>::trenutni() const {
    if (this->brojElemenata() == 0) throw std::logic_error("Niz je prazan");
    return *this->elementi[this->tekuci];
}

template <typename Tip>
bool NizLista<Tip>::prethodni() {
    if (this->brojElemenata() == 0) throw std::logic_error("Niz je prazan");
    if (this->tekuci == 0) return false;

    this->tekuci--;
    return true;
}

template <typename Tip>
bool NizLista<Tip>::sljedeci() {
    if (this->brojElemenata() == 0) throw std::logic_error("Niz je prazan");
    if (this->tekuci == this->brojElemenata() - 1) return false;

    this->tekuci++;
    return true;
}

template <typename Tip>
void NizLista<Tip>::pocetak() {
    if (this->brojElemenata() == 0) throw std::logic_error("Niz je prazan");
    this->tekuci = 0;
}

template <typename Tip>
void NizLista<Tip>::kraj() {
    if (this->brojElemenata() == 0) throw std::logic_error("Niz je prazan");
    this->tekuci = this->brojElemenata() - 1;
}

template <typename Tip>
void NizLista<Tip>::obrisi() {
    if (this->brojElemenata() == 0) throw std::logic_error("Niz je prazan");

    Tip* pomocna = this->elementi[this->tekuci];
    for (int i = this->tekuci; i < this->duzina - 1; i++)
        this->elementi[i] = this->elementi[i + 1];

    delete pomocna;

    this->duzina--;
    if (this->tekuci == this->duzina && this->tekuci != 0) this->tekuci--;
}

template <typename Tip>
void NizLista<Tip>::dodajIspred(const Tip& vrijednost) {
    if (this->duzina == this->kapacitet) {
        auto novi_elementi = new Tip*[this->kapacitet *= 2]{};

        std::copy(this->elementi, this->elementi + this->tekuci, novi_elementi);
        std::copy(this->elementi + this->tekuci, this->elementi + this->duzina, novi_elementi + 1 + this->tekuci);

        delete[] this->elementi;
        this->elementi = novi_elementi;
    } else {
        std::copy_backward(this->elementi + this->tekuci, this->elementi + this->duzina, this->elementi + this->duzina + 1);
    }

    this->elementi[this->tekuci] = new Tip{vrijednost};
    this->duzina++;
    this->tekuci += this->duzina == 1 ? 0 : 1;
}

template <typename Tip>
void NizLista<Tip>::dodajIza(const Tip& vrijednost) {
    if (this->duzina == this->kapacitet) {
        auto novi_elementi = new Tip*[this->kapacitet *= 2]{};

        std::copy(this->elementi, this->elementi + this->duzina, novi_elementi);

        delete[] this->elementi;
        this->elementi = novi_elementi;
    }

    for (int i = this->duzina; i > this->tekuci + 1; i--)
        this->elementi[i] = this->elementi[i - 1];

    this->duzina++;
    this->elementi[this->duzina == 1 ? 0 : this->tekuci + 1] = new Tip{vrijednost};
}

template <typename Tip>
Tip& NizLista<Tip>::operator [](int indeks) {
    if (indeks < 0 || indeks >= this->brojElemenata()) throw std::range_error("Indeks nije valjan");
    return *this->elementi[indeks];
}

template <typename Tip>
Tip NizLista<Tip>::operator [](int indeks) const {
    if (indeks < 0 || indeks >= this->brojElemenata()) throw std::range_error("Indeks nije valjan");
    return *this->elementi[indeks];
}

// ------------------------------------------------------- END - IMPLEMENTACIJA INTERFEJSA

// ==================================================================================================== END - NIZ LISTA

// ==================================================================================================== BEGIN - JEDNOSTRUKA LISTA

template <typename Tip>
class JednostrukaLista: public Lista<Tip> {
    struct Cvor {
        Tip vrijednost;
        Cvor* sljedeci;

        Cvor (const Tip& vrijednost, Cvor* sljedeci): vrijednost(vrijednost), sljedeci(sljedeci) {}
    };

    /*
     * ANALIZA RAZLICITIH (ALI SLICNIH) PRISTUPA IMPLEMENTACIJI
     * Prikaz metoda u kojima se, za razlicit set atributa, mora prolaziti kroz listu:
     *
     * Atributi: pocetni, krajnji, tekuci
     *   - metoda "dodajIspred()" zbog modifikovanja internog pokazivaca u cvoru prije tekuceg
     *   - metoda "obrisi()" u slucaju brisanja cvora na nekoj od pozicija u sredini
     *   - metoda "obrisi()" u slucaju brisanja posljednjeg cvora
     *   - metoda "prethodni()"
     *
     * Atributi: pocetni, krajnji, prije_tekuceg
     *   - metoda "kraj()" zbog usmjeravanja pokazivaca "prije_tekuceg" na pretposljednji cvor kojem ne mozemo pristupiti
     *   - metoda "obrisi()" u slucaju brisanja posljednjeg cvora
     *   - metoda "prethodni()"
     *
     * Atributi: pocetni, prije_krajnjeg, prije_tekuceg
     *   - metoda "obrisi()" u slucaju brisanja posljednjeg cvora
     *   - metoda "prethodni()"
     *
     */

    int duzina;
    Cvor *pocetni, *prije_krajnjeg, *prije_trenutnog;

    // --------------------

    void dodajKraj(const Tip&, Cvor* sljedeci = nullptr);

    // --------------------

public:
    JednostrukaLista();
    //JednostrukaLista(std::initializer_list<Tip>);

    JednostrukaLista(const JednostrukaLista&);
    JednostrukaLista& operator =(const JednostrukaLista&);

    JednostrukaLista(JednostrukaLista&&) = default;
    JednostrukaLista& operator =(JednostrukaLista&&) = default;

    ~JednostrukaLista();

    // --------------------

    int brojElemenata() const override { return this->duzina; }

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

// ------------------------------------------------------- BEGIN - UPRAVLJACKI DIO KLASE

template <typename Tip>
JednostrukaLista<Tip>::JednostrukaLista():
        duzina(0), pocetni(nullptr), prije_krajnjeg(nullptr), prije_trenutnog(nullptr) {}

//template <typename Tip>
//JednostrukaLista<Tip>::JednostrukaLista(std::initializer_list<Tip> lista) {}

template <typename Tip>
JednostrukaLista<Tip>::JednostrukaLista(const JednostrukaLista& lista):
        duzina(0), pocetni(nullptr), prije_krajnjeg(nullptr), prije_trenutnog(nullptr) {
    Cvor* element(lista.pocetni);

    while (element != nullptr) {
        this->dodajKraj(element->vrijednost);
        element = element->sljedeci;
    }
}

template <typename Tip>
JednostrukaLista<Tip>& JednostrukaLista<Tip>::operator =(const JednostrukaLista& lista) {
    if (&lista == this) return *this;

    while (this->duzina--) {
        auto pomocni = this->pocetni;
        this->pocetni = this->pocetni->sljedeci;
        delete pomocni;
    }

    this->duzina = 0;
    this->pocetni = this->prije_krajnjeg = this->prije_trenutnog = nullptr;

    Cvor* element(lista.pocetni);
    while (element != nullptr) {
        this->dodajKraj(element->vrijednost);
        element = element->sljedeci;
    }

    return *this;
}

//template <typename Tip>
//JednostrukaLista<Tip>::JednostrukaLista(JednostrukaLista&& lista) {}

//template <typename Tip>
//JednostrukaLista<Tip>& JednostrukaLista<Tip>::operator =(JednostrukaLista&& lista) {}

template <typename Tip>
JednostrukaLista<Tip>::~JednostrukaLista() {
    while (this->duzina--) {
        auto pomocni = this->pocetni;
        this->pocetni = this->pocetni->sljedeci;
        delete pomocni;
    }
}

// ------------------------------------------------------- END - UPRAVLJACKI DIO KLASE

// ------------------------------------------------------- BEGIN - PRIVATNI DIO KLASE

template <typename Tip>
void JednostrukaLista<Tip>::dodajKraj(const Tip& vrijednost, Cvor* sljedeci) {
    auto novi_element = new Cvor(vrijednost, sljedeci);
    this->duzina++;

    if (this->brojElemenata() == 1) {
        this->pocetni = novi_element;
        this->prije_krajnjeg = this->prije_trenutnog = nullptr;
    } else if (this->brojElemenata() == 2) {
        this->pocetni->sljedeci = novi_element;
        this->prije_krajnjeg = this->pocetni;
    } else {
        this->prije_krajnjeg = this->prije_krajnjeg->sljedeci;
        this->prije_krajnjeg->sljedeci = novi_element;
    }
}

// ------------------------------------------------------- END - PRIVATNI DIO KLASE

// ------------------------------------------------------- BEGIN - IMPLEMENTACIJA INTERFEJSA

template <typename Tip>
Tip& JednostrukaLista<Tip>::trenutni() {
    if (this->brojElemenata() == 0) throw std::logic_error("Lista je prazna");

    if (this->prije_trenutnog == nullptr) {
        return this->pocetni->vrijednost;
    }

    return this->prije_trenutnog->sljedeci->vrijednost;
}

template <typename Tip>
Tip JednostrukaLista<Tip>::trenutni() const {
    if (this->brojElemenata() == 0) throw std::logic_error("Lista je prazna");

    if (this->prije_trenutnog == nullptr) {
        return this->pocetni->vrijednost;
    }

    return this->prije_trenutnog->sljedeci->vrijednost;
}

template <typename Tip>
bool JednostrukaLista<Tip>::prethodni() {
    if (this->brojElemenata() == 0) throw std::logic_error("Lista je prazna");
    if (this->prije_trenutnog == nullptr) return false;

    if (this->prije_trenutnog == this->pocetni) {
        this->prije_trenutnog = nullptr;
    } else {
        Cvor* pomocni(this->pocetni);
        while (pomocni->sljedeci != this->prije_trenutnog) pomocni = pomocni->sljedeci;
        this->prije_trenutnog = pomocni;
    }

    return true;
}

template <typename Tip>
bool JednostrukaLista<Tip>::sljedeci() {
    if (this->brojElemenata() == 0) throw std::logic_error("Lista je prazna");
    if (this->prije_trenutnog == this->prije_krajnjeg) return false;

    if (this->prije_trenutnog == nullptr) {
        this->prije_trenutnog = this->pocetni;
    } else {
        this->prije_trenutnog = this->prije_trenutnog->sljedeci;
    }

    return true;
}

template <typename Tip>
void JednostrukaLista<Tip>::pocetak() {
    if (this->brojElemenata() == 0) throw std::logic_error("Lista je prazna");
    this->prije_trenutnog = nullptr;
}

template <typename Tip>
void JednostrukaLista<Tip>::kraj() {
    if (this->brojElemenata() == 0) throw std::logic_error("Lista je prazna");
    this->prije_trenutnog = this->prije_krajnjeg;
}

template <typename Tip>
void JednostrukaLista<Tip>::obrisi() {
    if (this->brojElemenata() == 0) throw std::logic_error("Lista je prazna");
    Cvor* cuvaj;

    if (this->brojElemenata() == 1) {
        delete this->pocetni;
        this->pocetni = nullptr;
    } else if (this->brojElemenata() == 2) {
        if (this->prije_trenutnog == nullptr) {
            this->pocetni = this->pocetni->sljedeci;
            delete this->prije_krajnjeg;
            this->prije_krajnjeg = nullptr;
        } else {
            delete this->prije_trenutnog->sljedeci;
            this->pocetni->sljedeci = nullptr;
            this->prije_krajnjeg = nullptr;
            this->prije_trenutnog = nullptr;
        }
    } else if (this->prije_trenutnog == nullptr) {
        cuvaj = this->pocetni;
        this->pocetni = this->pocetni->sljedeci;
        delete cuvaj;
    } else if (this->prije_trenutnog == this->prije_krajnjeg) {
        cuvaj = this->prije_trenutnog->sljedeci;
        this->prije_trenutnog->sljedeci = nullptr;
        delete cuvaj;

        cuvaj = this->pocetni;
        while (cuvaj->sljedeci != this->prije_trenutnog) cuvaj = cuvaj->sljedeci;

        this->prije_krajnjeg = cuvaj;
        this->prije_trenutnog = cuvaj;
    } else if (this->prije_trenutnog->sljedeci == this->prije_krajnjeg) {
        cuvaj = this->prije_trenutnog->sljedeci;
        this->prije_trenutnog->sljedeci = this->prije_trenutnog->sljedeci->sljedeci;
        delete cuvaj;
        this->prije_krajnjeg = this->prije_trenutnog;
    } else {
        cuvaj = this->prije_trenutnog->sljedeci;
        this->prije_trenutnog->sljedeci = this->prije_trenutnog->sljedeci->sljedeci;
        delete cuvaj;
    }

    this->duzina--;
}

template <typename Tip>
void JednostrukaLista<Tip>::dodajIspred(const Tip& vrijednost) {
    auto novi_element = new Cvor(vrijednost, nullptr);

    if (this->brojElemenata() == 0) {
        this->pocetni = novi_element;
        this->prije_krajnjeg = this->prije_trenutnog = nullptr;
    } else if (this->brojElemenata() == 1) {
        novi_element->sljedeci = this->pocetni;
        this->pocetni = this->prije_krajnjeg = this->prije_trenutnog = novi_element;
    } else if (this->prije_trenutnog == nullptr) {
        novi_element->sljedeci = this->pocetni;
        this->pocetni = this->prije_trenutnog = novi_element;
    } else if (this->prije_trenutnog == this->prije_krajnjeg) {
        novi_element->sljedeci = this->prije_trenutnog->sljedeci;
        this->prije_trenutnog->sljedeci = novi_element;
        this->prije_krajnjeg = this->prije_trenutnog = novi_element;
    } else {
        novi_element->sljedeci = this->prije_trenutnog->sljedeci;
        this->prije_trenutnog->sljedeci = novi_element;
        this->prije_trenutnog = novi_element;
    }

    this->duzina++;
}

template <typename Tip>
void JednostrukaLista<Tip>::dodajIza(const Tip& vrijednost) {
    auto novi_element = new Cvor(vrijednost, nullptr);

    if (this->brojElemenata() == 0) {
        this->pocetni = novi_element;
        this->prije_krajnjeg = this->prije_trenutnog = nullptr;
    } else if (this->brojElemenata() == 1) {
        this->pocetni->sljedeci = novi_element;
        this->prije_krajnjeg = this->pocetni;
    } else if (this->brojElemenata() == 2) {
        if (this->prije_trenutnog == nullptr) {
            novi_element->sljedeci = this->pocetni->sljedeci;
            this->pocetni->sljedeci = this->prije_krajnjeg = novi_element;
        } else {
            this->prije_krajnjeg = this->prije_krajnjeg->sljedeci;
            this->prije_krajnjeg->sljedeci = novi_element;
        }
    } else if (this->prije_trenutnog == nullptr) {
        novi_element->sljedeci = this->pocetni->sljedeci;
        this->pocetni->sljedeci = novi_element;
    } else if (this->prije_trenutnog == this->prije_krajnjeg) {
        this->prije_krajnjeg = this->prije_krajnjeg->sljedeci;
        this->prije_krajnjeg->sljedeci = novi_element;
    } else if (this->prije_trenutnog->sljedeci == this->prije_krajnjeg) {
        novi_element->sljedeci = this->prije_krajnjeg->sljedeci;
        this->prije_krajnjeg->sljedeci = novi_element;
        this->prije_krajnjeg = novi_element;
    } else {
        novi_element->sljedeci = this->prije_trenutnog->sljedeci->sljedeci;
        this->prije_trenutnog->sljedeci->sljedeci = novi_element;
    }

    this->duzina++;
}

template <typename Tip>
Tip& JednostrukaLista<Tip>::operator [](int indeks) {
    if (indeks < 0 || indeks >= this->brojElemenata()) throw std::range_error("Indeks nije valjan");

    Cvor* pomocni(this->pocetni);
    for (int i = 0; i < indeks; i++)
        pomocni = pomocni->sljedeci;

    return pomocni->vrijednost;
}

template <typename Tip>
Tip JednostrukaLista<Tip>::operator [](int indeks) const {
    if (indeks < 0 || indeks >= this->brojElemenata()) throw std::range_error("Indeks nije valjan");

    Cvor* pomocni(this->pocetni);
    for (int i = 0; i < indeks; i++)
        pomocni = pomocni->sljedeci;

    return pomocni->vrijednost;
}

// ------------------------------------------------------- END - IMPLEMENTACIJA INTERFEJSA

// ==================================================================================================== END - JEDNOSTRUKA LISTA

// ==================================================================================================== BEGIN - ZAGLAVLJA

template <typename Tip>
void testBrojElemenata(std::shared_ptr<Lista<Tip>>);

template <typename Tip>
void testTrenutni(std::shared_ptr<Lista<Tip>>);

template <typename Tip>
void testPrethodni(std::shared_ptr<Lista<Tip>>);

template <typename Tip>
void testSljedeci(std::shared_ptr<Lista<Tip>>);

template <typename Tip>
void testPocetak(std::shared_ptr<Lista<Tip>>);

template <typename Tip>
void testKraj(std::shared_ptr<Lista<Tip>>);

template <typename Tip>
void testObrisi(std::shared_ptr<Lista<Tip>>);

template <typename Tip>
void testDodajIspred(std::shared_ptr<Lista<Tip>>);

template <typename Tip>
void testDodajIza(std::shared_ptr<Lista<Tip>>);

template <typename Tip>
void testOperator(std::shared_ptr<Lista<Tip>>);

template <typename Tip>
void testLista(std::shared_ptr<Lista<Tip>>);

// ==================================================================================================== END - ZAGLAVLJA

int main() {
    std::shared_ptr<Lista<int>> lista;

    lista = std::make_shared<NizLista<int>>();
    for (int i = 1; i <= 10; i++) lista->dodajIza(i);
    //testLista(lista);

    lista = std::make_shared<JednostrukaLista<int>>();
    for (int i = 1; i <= 10; i++) lista->dodajIza(i);
    //testLista(lista);

    return 0;
}

// ==================================================================================================== BEGIN - TEST FUNKCIJE

template <typename Tip>
void dajInfo(std::shared_ptr<Lista<Tip>> lista) {
    if (lista->brojElemenata() != 0) {
        std::shared_ptr<Lista<Tip>> pomocna;

        if (typeid(*lista) == typeid(NizLista<Tip>)) pomocna = std::make_shared<NizLista<Tip>>();
        else if (typeid(*lista) == typeid(JednostrukaLista<Tip>)) pomocna = std::make_shared<JednostrukaLista<Tip>>();

        for (int i = 0; i < lista->brojElemenata(); i++) {
            pomocna->dodajIza((*lista)[i]);
            pomocna->sljedeci();
        }

        std::cout << "[ trenutni = " << lista->trenutni(); pomocna->pocetak();
        std::cout << " | pocetak = " << pomocna->trenutni(); pomocna->kraj();
        std::cout << " | kraj = " << pomocna->trenutni() << " ] ---";
    }

    std::cout << " { ";
    for (int i = 0; i < lista->brojElemenata(); i++) std::cout << (*lista)[i] << " ";
    std::cout << "} " << std::endl;
}

template <typename Tip>
void testBrojElemenata(std::shared_ptr<Lista<Tip>> lista) {
    std::cout << std::string(75, '-') << " TEST brojElemenata()\n";

    dajInfo(lista); std::cout << lista->brojElemenata() << std::endl;
    lista->obrisi();
    dajInfo(lista); std::cout << lista->brojElemenata() << std::endl;
}

template <typename Tip>
void testTrenutni(std::shared_ptr<Lista<Tip>> lista) {
    std::cout << std::string(75, '-') << " TEST trenutni()\n";

    dajInfo(lista);
    lista->trenutni() = 0; dajInfo(lista);

    try {
        const NizLista<int> test;
        std::cout << test.trenutni();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }

    try {
        const JednostrukaLista<int> test;
        std::cout << test.trenutni();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testPrethodni(std::shared_ptr<Lista<Tip>> lista) {
    std::cout << std::string(75, '-') << " TEST prethodni()\n";

    dajInfo(lista);
    lista->kraj(); dajInfo(lista);
    lista->prethodni(); dajInfo(lista);

    while (lista->prethodni());
    dajInfo(lista);

    try {
        NizLista<int> test;
        test.prethodni();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }

    try {
        JednostrukaLista<int> test;
        test.prethodni();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testSljedeci(std::shared_ptr<Lista<Tip>> lista) {
    std::cout << std::string(75, '-') << " TEST sljedeci()\n";

    dajInfo(lista);
    lista->pocetak(); dajInfo(lista);
    lista->sljedeci(); dajInfo(lista);

    while (lista->sljedeci());
    dajInfo(lista);

    try {
        NizLista<int> test;
        test.sljedeci();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }

    try {
        JednostrukaLista<int> test;
        test.sljedeci();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testPocetak(std::shared_ptr<Lista<Tip>> lista) {
    std::cout << std::string(75, '-') << " TEST pocetak()\n";

    dajInfo(lista);
    lista->pocetak(); dajInfo(lista);

    try {
        NizLista<int> test;
        test.pocetak();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }

    try {
        JednostrukaLista<int> test;
        test.pocetak();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testKraj(std::shared_ptr<Lista<Tip>> lista) {
    std::cout << std::string(75, '-') << " TEST kraj()\n";

    dajInfo(lista);
    lista->kraj(); dajInfo(lista);

    try {
        NizLista<int> test;
        test.kraj();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }

    try {
        JednostrukaLista<int> test;
        test.kraj();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testObrisi(std::shared_ptr<Lista<Tip>> lista) {
    std::cout << std::string(75, '-') << " TEST obrisi()\n";

    lista->pocetak(); dajInfo(lista); lista->obrisi(); dajInfo(lista);
    lista->sljedeci(); dajInfo(lista); lista->obrisi(); dajInfo(lista);
    lista->kraj(); dajInfo(lista); lista->obrisi(); dajInfo(lista);
    lista->prethodni(); dajInfo(lista); lista->obrisi(); dajInfo(lista);

    while (lista->brojElemenata() != 4) lista->obrisi(); dajInfo(lista); lista->pocetak();
    while (lista->brojElemenata() != 1) lista->obrisi(); dajInfo(lista); lista->obrisi(); dajInfo(lista);

    try {
        lista->obrisi();
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testDodajIspred(std::shared_ptr<Lista<Tip>> lista) {
    std::cout << std::string(75, '-') << " TEST dodajIspred()\n";

    while (lista->brojElemenata() != 0) lista->obrisi(); dajInfo(lista);

    lista->dodajIspred(1); dajInfo(lista);
    lista->dodajIspred(2); dajInfo(lista);
    lista->dodajIspred(3); dajInfo(lista);

    lista->obrisi(); lista->pocetak(); dajInfo(lista);

    lista->dodajIspred(4); dajInfo(lista);
    lista->dodajIspred(5); dajInfo(lista);
}

template <typename Tip>
void testDodajIza(std::shared_ptr<Lista<Tip>> lista) {
    std::cout << std::string(75, '-') << " TEST dodajIza()\n";

    while (lista->brojElemenata() != 0) lista->obrisi(); dajInfo(lista);

    lista->dodajIza(1); dajInfo(lista);
    lista->dodajIza(2); dajInfo(lista);
    lista->dodajIza(3); dajInfo(lista);

    lista->obrisi(); lista->kraj(); dajInfo(lista);

    lista->dodajIza(4); dajInfo(lista);
    lista->dodajIza(5); dajInfo(lista);
}

template <typename Tip>
void testOperator(std::shared_ptr<Lista<Tip>> lista) {
    std::cout << std::string(75, '-') << " TEST operator []()\n";

    dajInfo(lista);
    for (int i = 0; i < lista->brojElemenata(); i++)
        std::cout << (*lista)[i] << " ";

    try {
        std::cout << std::endl << (*lista)[-1];
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }

    try {
        std::cout << (*lista)[lista->brojElemenata()];
    } catch (std::exception& izuzetak) {
        std::cout << "IZUZETAK: " << izuzetak.what() << std::endl;
    }
}

template <typename Tip>
void testLista(std::shared_ptr<Lista<Tip>> lista) {
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