#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <iterator>


class Padavine {
    std::vector<int> padavine;
    int max_kolicina;

public:
    explicit Padavine(int max_kolicina): padavine(0), max_kolicina(max_kolicina) {
        if (max_kolicina < 0) throw std::range_error("Ilegalna maksimalna kolicina");
    }


    int DajBrojRegistriranihPadavina() const { return this->padavine.size(); }
    int DajBrojDanaSaPadavinamaVecimOd(int) const;
    int DajMinimalnuKolicinuPadavina() const;
    int DajMaksimalnuKolicinuPadavina() const;

    void Ispisi() const;

    Padavine& RegistrirajPadavine(int);
    Padavine& BrisiSve() { this->padavine.clear(); return *this; }


    int operator [](int) const;

    Padavine& operator ++();
    Padavine operator ++(int);

    Padavine& operator +=(int);
    friend Padavine operator +(const Padavine&, int);
    friend Padavine operator +(int, const Padavine&);

    Padavine& operator -=(int);
    friend Padavine operator -(const Padavine&, int);

    friend Padavine operator -(int, const Padavine&);
    friend Padavine operator -(const Padavine& oduzmi) { return oduzmi.max_kolicina - oduzmi; }

    Padavine& operator -=(const Padavine&);
    friend Padavine operator -(const Padavine&, const Padavine&);

    friend bool operator ==(const Padavine&, const Padavine&);
    friend bool operator !=(const Padavine& prve, const Padavine& druge) { return !(prve == druge); }
};


Padavine& Padavine::RegistrirajPadavine(int kolicina) {
    if (kolicina < 0 || kolicina > this->max_kolicina) throw std::range_error("Ilegalna kolicina padavina");
    this->padavine.push_back(kolicina);
    return *this;
}

int Padavine::DajMinimalnuKolicinuPadavina() const {
    if (this->padavine.empty()) throw std::range_error("Nema registriranih padavina");
    return *std::min_element(this->padavine.begin(), this->padavine.end());
}

int Padavine::DajMaksimalnuKolicinuPadavina() const {
    if (this->padavine.empty()) throw std::range_error("Nema registriranih padavina");
    return *std::max_element(this->padavine.begin(), this->padavine.end());
}

int Padavine::DajBrojDanaSaPadavinamaVecimOd(int kolicina) const {
    if (this->padavine.empty()) throw std::range_error("Nema registriranih padavina");
    return std::count_if(this->padavine.begin(), this->padavine.end(), std::bind(std::greater<int>(), std::placeholders::_1, kolicina));
}

void Padavine::Ispisi() const {
    auto pomocne(this->padavine);
    std::sort(pomocne.begin(), pomocne.end(), std::greater<int>());
    std::copy(pomocne.begin(), pomocne.end(), std::ostream_iterator<int>(std::cout, "\n"));
}


int Padavine::operator [](int indeks) const {
    if (indeks < 1 || indeks > this->padavine.size()) throw std::range_error("Neispravan indeks");
    return this->padavine[indeks - 1];
}

Padavine& Padavine::operator ++() {
    this->max_kolicina++;
    std::transform(this->padavine.begin(), this->padavine.end(), this->padavine.begin(), std::bind(std::plus<int>(), std::placeholders::_1, 1));
    return *this;
}

inline Padavine Padavine::operator ++(int) {
    Padavine pomocne(*this); ++(*this); return pomocne;
}

Padavine& Padavine::operator +=(int kolicina) {
    if (DajMinimalnuKolicinuPadavina() + kolicina < 0)
        throw std::domain_error("Nekorektan rezultat operacije");

    this->max_kolicina += kolicina;
    std::transform(this->padavine.begin(), this->padavine.end(), this->padavine.begin(), std::bind(std::plus<int>(), std::placeholders::_1, kolicina));

    return *this;
}

inline Padavine operator +(const Padavine& dodaj, int kolicina) {
    Padavine zbir(dodaj); zbir += kolicina; return zbir;
}

inline Padavine operator +(int kolicina, const Padavine& dodaj) {
    Padavine zbir(dodaj); zbir += kolicina; return zbir;
}

Padavine& Padavine::operator -=(int kolicina) {
    if (DajMinimalnuKolicinuPadavina() - kolicina < 0)
        throw std::domain_error("Nekorektan rezultat operacije");

    this->max_kolicina -= kolicina;
    std::transform(this->padavine.begin(), this->padavine.end(), this->padavine.begin(), std::bind(std::minus<int>(), std::placeholders::_1, kolicina));

    return *this;
}

inline Padavine operator -(const Padavine& oduzmi, int kolicina) {
    Padavine razlika(oduzmi); razlika -= kolicina; return razlika;
}

Padavine operator -(int kolicina, const Padavine& oduzmi) {
    Padavine razlika(kolicina);
    razlika.padavine.resize(oduzmi.DajBrojRegistriranihPadavina(), kolicina);

    razlika -= oduzmi;
    return razlika;
}

Padavine& Padavine::operator -=(const Padavine& oduzmi) {
    if (this->DajBrojRegistriranihPadavina() != oduzmi.DajBrojRegistriranihPadavina())
        throw std::domain_error("Nesaglasni operandi");

    std::transform(this->padavine.begin(), this->padavine.end(), oduzmi.padavine.begin(), this->padavine.begin(), std::minus<int>());

    if (this->DajMinimalnuKolicinuPadavina() < 0)
        throw std::domain_error("Nesaglasni operandi");

    return *this;
}

inline Padavine operator -(const Padavine& prve, const Padavine& druge) {
    Padavine razlika(prve); razlika -= druge; return razlika;
}

bool operator ==(const Padavine& prve, const Padavine& druge) {
    if (prve.DajBrojRegistriranihPadavina() != druge.DajBrojRegistriranihPadavina()) return false;
    return std::equal(prve.padavine.begin(), prve.padavine.end(), druge.padavine.begin());
}


int main() {
    try {
        Padavine sarajevo(7);

        int unosi[]{5, 3, 7, 1, 2, 4, 6};
        for (int i = 0; i < 7; i++) sarajevo.RegistrirajPadavine(unosi[i]);

        std::cout << "min " << sarajevo.DajMinimalnuKolicinuPadavina() << " / max " << sarajevo.DajMaksimalnuKolicinuPadavina() << std::endl;
        std::cout << "> 4 : " << sarajevo.DajBrojDanaSaPadavinamaVecimOd(4) << std::endl;
        std::cout << "utorak: " << sarajevo[2] << std::endl;

        (++sarajevo)++;
        Padavine mostar(4);

        for (int i = 0; i < sarajevo.DajBrojRegistriranihPadavina(); i++)
            mostar.RegistrirajPadavine(i % 4);

        std::cout << "sa != mo = " << std::boolalpha << (sarajevo != mostar) << std::endl;
        (-mostar).Ispisi();

        Padavine bihac(10 - (sarajevo - mostar));
        mostar = (bihac -= 2) + 3;

        sarajevo.BrisiSve();

    } catch (std::exception &izuzetak) {
        std::cout << izuzetak.what();
    }

    return 0;
}