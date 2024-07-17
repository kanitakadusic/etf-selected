#include <exception>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <fstream>


class ApstraktnoVozilo {
    int tezina;

public:
    ApstraktnoVozilo(int tezina): tezina(tezina) {}
    virtual ~ApstraktnoVozilo() {}

    int DajTezinu() const { return this->tezina; }

    virtual int DajUkupnuTezinu() const = 0;
    virtual ApstraktnoVozilo* DajKopiju() const = 0;
    virtual void IspisiPodatke() const = 0;
};


class Automobil: public ApstraktnoVozilo {
    std::vector<int> tezine_putnika;

public:
    Automobil(int tezina, std::vector<int> tezine_putnika):
            ApstraktnoVozilo(tezina), tezine_putnika(std::move(tezine_putnika)) {}

    int DajUkupnuTezinu() const override {
        int suma = 0;
        for (int element : this->tezine_putnika) suma += element;
        return DajTezinu() + suma;
    }

    ApstraktnoVozilo* DajKopiju() const override {
        return new Automobil(*this);
    }

    void IspisiPodatke() const override {
        std::cout << "Vrsta vozila: Automobil\n"
                  << "Vlastita tezina: " << DajTezinu() << " kg\n"
                  << "Tezine putnika: ";

        for (int i = 0; i < this->tezine_putnika.size(); i++)
            std::cout << this->tezine_putnika[i] << " kg" << (i != this->tezine_putnika.size() - 1 ? ", " : "\n");

        std::cout << "Ukupna tezina: " << DajUkupnuTezinu() << " kg\n";
    }
};


class Kamion: public ApstraktnoVozilo {
    int tezina_tereta;

public:
    Kamion(int tezina, int tezina_tereta): ApstraktnoVozilo(tezina), tezina_tereta(tezina_tereta) {}

    int DajUkupnuTezinu() const override {
        return DajTezinu() + this->tezina_tereta;
    }

    ApstraktnoVozilo* DajKopiju() const override {
        return new Kamion(*this);
    }

    void IspisiPodatke() const override {
        std::cout << "Vrsta vozila: Kamion\n"
                  << "Vlastita tezina: " << DajTezinu() << " kg\n"
                  << "Tezina tereta: " << this->tezina_tereta << " kg\n"
                  << "Ukupna tezina: " << DajUkupnuTezinu() << " kg\n";
    }
};


class Autobus: public ApstraktnoVozilo {
    int broj_putnika, prosjecna_tezina_putnika;

public:
    Autobus(int tezina, int broj_putnika, int prosjecna_tezina_putnika):
            ApstraktnoVozilo(tezina), broj_putnika(broj_putnika), prosjecna_tezina_putnika(prosjecna_tezina_putnika) {}

    int DajUkupnuTezinu() const override {
        return DajTezinu() + this->broj_putnika * this->prosjecna_tezina_putnika;
    }

    ApstraktnoVozilo* DajKopiju() const override {
        return new Autobus(*this);
    }

    void IspisiPodatke() const override {
        std::cout << "Vrsta vozila: Autobus\n"
                  << "Vlastita tezina: " << DajTezinu() << " kg\n"
                  << "Broj putnika: " << this->broj_putnika << "\n"
                  << "Prosjecna tezina putnika: " << this->prosjecna_tezina_putnika << " kg\n"
                  << "Ukupna tezina: " << DajUkupnuTezinu() << " kg\n";
    }
};


class Vozilo {
    ApstraktnoVozilo* p_vozilo;

    void KonkretnoVozilo() const {
        if (!this->p_vozilo) throw std::logic_error("Nespecificirano vozilo");
    }

public:
    Vozilo(): p_vozilo(nullptr) {}
    Vozilo(const ApstraktnoVozilo& vozilo): p_vozilo(vozilo.DajKopiju()) {}
    ~Vozilo() { delete this->p_vozilo; }

    Vozilo(const Vozilo& novo) {
        if (!novo.p_vozilo) this->p_vozilo = nullptr;
        else this->p_vozilo = novo.p_vozilo->DajKopiju();
    }

    Vozilo(Vozilo&& privremeno) {
        this->p_vozilo = privremeno.p_vozilo;
        privremeno.p_vozilo = nullptr;
    }

    Vozilo& operator =(Vozilo vozilo) {
        std::swap(this->p_vozilo, vozilo.p_vozilo);
        return *this;
    }

    int DajTezinu() const { KonkretnoVozilo(); return this->p_vozilo->DajTezinu(); }
    int DajUkupnuTezinu() const { KonkretnoVozilo(); return this->p_vozilo->DajUkupnuTezinu(); }
    ApstraktnoVozilo* DajKopiju() const { KonkretnoVozilo(); return this->p_vozilo->DajKopiju(); }
    void IspisiPodatke() const { KonkretnoVozilo(); this->p_vozilo->IspisiPodatke(); }
};


int main() {
    Vozilo UcitajAutomobil(std::ifstream&);
    Vozilo UcitajKamion(std::ifstream&);
    Vozilo UcitajAutobus(std::ifstream&);

    try {
        std::ifstream datoteka("VOZILA.TXT");
        if (!datoteka) throw std::logic_error("Otvaranje datoteke nije uspjelo");

        std::vector<Vozilo> vozila;
        char vrsta_vozila = 0;

        while (datoteka >> vrsta_vozila) {
            Vozilo vozilo;

            switch (vrsta_vozila) {
                case 'A': vozilo = UcitajAutomobil(datoteka); break;
                case 'K': vozilo = UcitajKamion(datoteka); break;
                case 'B': vozilo = UcitajAutobus(datoteka); break;
            }

            if (datoteka) vozila.emplace_back(vozilo);
        }

        if (datoteka.bad()) throw std::logic_error("Datoteka je vjerovatno ostecena");
        else if (!datoteka.eof()) throw std::logic_error("Datoteka sadrzi neocekivane podatke");

        std::sort(vozila.begin(), vozila.end(), [](Vozilo& prvo, Vozilo& drugo) { return prvo.DajUkupnuTezinu() < drugo.DajUkupnuTezinu(); });
        std::for_each(vozila.begin(), vozila.end(), [](Vozilo& vozilo) { std::cout << vozilo.DajUkupnuTezinu() << std::endl; });

    } catch (...) {
        std::cout << "Izuzetak: NEPOSTOJECE VOZILO!";
    }

    return 0;
}

Vozilo UcitajAutomobil(std::ifstream& tok) {
    int tezina_vozila = 0, broj_putnika = 0;
    tok >> tezina_vozila >> broj_putnika;

    std::vector<int> tezine_putnika;

    while (tok && tok.get() != '\n' && broj_putnika--) {
        int tezina_pojedinca;
        tok >> tezina_pojedinca;

        tezine_putnika.push_back(tezina_pojedinca);
    }

    return Automobil(tezina_vozila, tezine_putnika);
}

Vozilo UcitajKamion(std::ifstream& tok) {
    int tezina_vozila = 0, tezina_tereta = 0;
    tok >> tezina_vozila >> tezina_tereta;

    return Kamion(tezina_vozila, tezina_tereta);
}

Vozilo UcitajAutobus(std::ifstream& tok) {
    int tezina_vozila = 0, broj_putnika = 0, prosjecna_tezina_putnika = 0;
    tok >> tezina_vozila >> broj_putnika >> prosjecna_tezina_putnika;

    return Autobus(tezina_vozila, broj_putnika, prosjecna_tezina_putnika);
}