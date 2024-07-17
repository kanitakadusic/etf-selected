#include <iostream>
#include <string>
#include <stdexcept>
#include <iomanip>
#include <initializer_list>
#include <algorithm>

// ________________________________________________________________________ SAT

class Sat {
    int sati, minute;

public:
    explicit Sat(int sati = 0, int minute = 0) {
        if (sati < 0 || sati > 23 || minute < 0 || minute > 59) throw std::domain_error("Neispravno vrijeme");
        this->sati = sati; this->minute = minute;
    }

    Sat &PomjeriZa(int pomak) {
        this->sati = (this->sati + (this->minute + pomak) / 60) % 24;
        this->minute = (this->minute + pomak) % 60;
        return *this;
    }

    void Ispisi() const {
        std::cout << std::right << std::setw(2) << DajSate() << ":" << std::setfill('0') << std::setw(2) << DajMinute() << std::setfill(' ');
    }

    int DajSate() const { return this->sati; }
    int DajMinute() const { return this->minute; }
};

// ________________________________________________________________________ POLAZAK

class Polazak {
    std::string odrediste, oznaka;
    Sat polazak;
    int peron, trajanje, kasnjenje;

public:
    Polazak(std::string odrediste, std::string oznaka, int peron, int sati, int minute, int trajanje);

    void PostaviKasnjenje(int kasnjenje) {
        if (kasnjenje < 0) throw std::domain_error("Neispravno kasnjenje");
        this->kasnjenje = kasnjenje;
    }

    void Ispisi() const;
    void OcekivanoVrijemePolaska(int &sati, int &minute) const;
    void OcekivanoVrijemeDolaska(int &sati, int &minute) const;

    int DajTrajanje() const { return this->trajanje; }
    bool DaLiKasni() const { return this->kasnjenje > 0; }
};

// ----------------------

Polazak::Polazak(std::string odrediste, std::string oznaka, int peron, int sati, int minute, int trajanje):
        odrediste(std::move(odrediste)), oznaka(std::move(oznaka)), polazak(sati, minute), peron(peron), trajanje(trajanje), kasnjenje(0) {
    if (peron < 1 || peron > 15) throw std::domain_error("Neispravan peron");
    if (trajanje < 0) throw std::domain_error("Neispravno trajanje");
}

// ----------------------

void Polazak::Ispisi() const {
    Sat satnica(this->polazak);

    std::cout << std::left << std::setw(10) << this->oznaka << std::setw(30) << this->odrediste;
    std::cout << std::right << std::string(5, ' '); satnica.PomjeriZa(this->kasnjenje).Ispisi();

    if (DaLiKasni()) {
        std::cout << " (Planirano"; this->polazak.Ispisi();
        std::cout << ", Kasni " << this->kasnjenje << " min)";
    } else {
        std::cout << std::string(5, ' '); satnica.PomjeriZa(this->trajanje).Ispisi();
        std::cout << std::setw(8) << this->peron;
    }

    std::cout << std::endl;
}

void Polazak::OcekivanoVrijemePolaska(int &sati, int &minute) const {
    Sat polazak(this->polazak); polazak.PomjeriZa(this->kasnjenje);
    sati = polazak.DajSate(); minute = polazak.DajMinute();
}

void Polazak::OcekivanoVrijemeDolaska(int &sati, int &minute) const {
    Sat dolazak(this->polazak); dolazak.PomjeriZa(this->trajanje + this->kasnjenje);
    sati = dolazak.DajSate(); minute = dolazak.DajMinute();
}

// ________________________________________________________________________ POLASCI

class Polasci {
    int kapacitet, broj_polazaka;
    Polazak **kolekcija;

    static constexpr double granica_za_realokaciju = 0.2;

    static void OslobodiMemoriju(Polazak **polasci, int broj_polazaka);
    static bool KriterijPoVremenuPolaska(const Polazak *prvi, const Polazak *drugi);

public:
    explicit Polasci(int kapacitet): kapacitet(kapacitet), broj_polazaka(0), kolekcija(new Polazak*[kapacitet]{}) {}
    Polasci(std::initializer_list<Polazak> polasci);

    Polasci(const Polasci &polasci);
    Polasci &operator =(const Polasci &polasci);

    Polasci(Polasci &&polasci);
    Polasci &operator =(Polasci &&polasci);

    ~Polasci() { OslobodiMemoriju(this->kolekcija, this->broj_polazaka); }


    void RegistrirajPolazak(std::string odrediste, std::string oznaka, int peron, int sati, int minute, int trajanje);
    void RegistrirajPolazak(Polazak *polazak);

    int DajBrojPolazaka() const { return this->broj_polazaka; }
    int DajBrojPolazakaKojiKasne() const;

    Polazak DajPrviPolazak() const;
    Polazak &DajPrviPolazak();
    Polazak DajPosljednjiPolazak() const;
    Polazak &DajPosljednjiPolazak();

    void Ispisi() const;
    void IsprazniKolekciju();
};

// ----------------------

void Polasci::OslobodiMemoriju(Polazak **polasci, int broj_polazaka) {
    for (int i = 0; i < broj_polazaka; i++) delete polasci[i];
    delete[] polasci;
}

bool Polasci::KriterijPoVremenuPolaska(const Polazak *prvi, const Polazak *drugi) {
    int sati_prvog, minute_prvog, sati_drugog, minute_drugog;

    prvi->OcekivanoVrijemePolaska(sati_prvog, minute_prvog);
    drugi->OcekivanoVrijemePolaska(sati_drugog, minute_drugog);

    return (sati_prvog < sati_drugog) || (sati_prvog == sati_drugog && minute_prvog < minute_drugog);
}

// ----------------------

Polasci::Polasci(std::initializer_list<Polazak> polasci): kapacitet(polasci.size()), broj_polazaka(polasci.size()), kolekcija(new Polazak*[polasci.size()]{}) {
    std::initializer_list<Polazak>::iterator i_polasci = polasci.begin();

    for (int i = 0; i < polasci.size(); i++)
        try {
            this->kolekcija[i] = new Polazak{*i_polasci++};
        } catch (...) {
            OslobodiMemoriju(this->kolekcija, i); throw;
        }
}

Polasci::Polasci(const Polasci &novi): kapacitet(novi.kapacitet), broj_polazaka(novi.broj_polazaka), kolekcija(new Polazak*[novi.kapacitet]{}) {
    for (int i = 0; i < novi.broj_polazaka; i++)
        try {
            this->kolekcija[i] = new Polazak{*novi.kolekcija[i]};
        } catch (...) {
            OslobodiMemoriju(this->kolekcija, i); throw;
        }
}

Polasci &Polasci::operator =(const Polasci &novi) {
    if (novi.kapacitet > this->kapacitet || novi.kapacitet <= this->kapacitet * Polasci::granica_za_realokaciju) {
        Polazak **nova_kolekcija = new Polazak*[novi.kapacitet]{};

        for (int i = 0; i < novi.broj_polazaka; i++)
            try {
                nova_kolekcija[i] = new Polazak{*novi.kolekcija[i]};
            } catch (...) {
                OslobodiMemoriju(nova_kolekcija, i); throw;
            }

        OslobodiMemoriju(this->kolekcija, this->broj_polazaka);

        this->kapacitet = novi.kapacitet;
        this->kolekcija = nova_kolekcija;
    } else if (&novi != this) {
        IsprazniKolekciju();

        for (int i = 0; i < novi.broj_polazaka; i++)
            try {
                this->kolekcija[i] = new Polazak{*novi.kolekcija[i]};
            } catch (...) {
                this->broj_polazaka = i - 1;
                throw std::logic_error("Djelimicno kopiranje");
            }
    }

    this->broj_polazaka = novi.broj_polazaka;
    return *this;
}

Polasci::Polasci(Polasci &&privremeni): kapacitet(privremeni.kapacitet), broj_polazaka(privremeni.broj_polazaka), kolekcija(privremeni.kolekcija) {
    privremeni.broj_polazaka = 0; privremeni.kolekcija = nullptr;
}

Polasci &Polasci::operator =(Polasci &&privremeni) {
    std::swap(this->kapacitet, privremeni.kapacitet);
    std::swap(this->broj_polazaka, privremeni.broj_polazaka);
    std::swap(this->kolekcija, privremeni.kolekcija);

    return *this;
}

// ----------------------

void Polasci::RegistrirajPolazak(std::string odrediste, std::string oznaka, int peron, int sati, int minute, int trajanje) {
    if (this->broj_polazaka == this->kapacitet) throw std::range_error("Dostignut maksimalni broj polazaka");
    this->kolekcija[this->broj_polazaka++] = new Polazak(std::move(odrediste), std::move(oznaka), peron, sati, minute, trajanje);
}

void Polasci::RegistrirajPolazak(Polazak *polazak) {
    if (this->broj_polazaka == this->kapacitet) throw std::range_error("Dostignut maksimalni broj polazaka");
    this->kolekcija[this->broj_polazaka++] = polazak;
}

int Polasci::DajBrojPolazakaKojiKasne() const {
    return std::count_if(this->kolekcija, this->kolekcija + this->broj_polazaka, [](const Polazak *polazak) { return polazak->DaLiKasni(); });
}

Polazak Polasci::DajPrviPolazak() const {
    return **std::min_element(this->kolekcija, this->kolekcija + this->broj_polazaka, KriterijPoVremenuPolaska);
}

Polazak &Polasci::DajPrviPolazak() {
    return **std::min_element(this->kolekcija, this->kolekcija + this->broj_polazaka, KriterijPoVremenuPolaska);
}

Polazak Polasci::DajPosljednjiPolazak() const {
    return **std::max_element(this->kolekcija, this->kolekcija + this->broj_polazaka, KriterijPoVremenuPolaska);
}

Polazak &Polasci::DajPosljednjiPolazak() {
    return **std::max_element(this->kolekcija, this->kolekcija + this->broj_polazaka, KriterijPoVremenuPolaska);
}

void Polasci::Ispisi() const {
    std::cout << std::right << std::setw(10) << "Voznja" << std::setw(30) << "Odrediste" << std::setw(10) << "Polazak" << std::setw(10) << "Dolazak" << std::setw(8) << "Peron";
    std::cout << std::endl << std::string(70, '-') << std::endl;

    std::sort(this->kolekcija, this->kolekcija + this->broj_polazaka, KriterijPoVremenuPolaska);
    std::for_each(this->kolekcija, this->kolekcija + this->broj_polazaka, [](const Polazak *polazak) { polazak->Ispisi(); });
}

void Polasci::IsprazniKolekciju() {
    std::for_each(this->kolekcija, this->kolekcija + this->broj_polazaka, [](Polazak *&polazak) { delete polazak; polazak = nullptr; });
    this->broj_polazaka = 0;
}

// ________________________________________________________________________ MAIN

int main() {
    std::cout << "27.05.2023\n"; // sekvencijski konstruktor
    Polasci dan_27_05({{"London", "BH3", 7, 9, 15, 135}, {"Istanbul", "D7T", 1, 23, 30, 90}, {"Maldivi", "M97", 8, 18, 0, 240}});
    dan_27_05.DajPrviPolazak().PostaviKasnjenje(20);
    dan_27_05.Ispisi();

    std::cout << std::endl; // kopirajuci konstruktor
    Polasci dan_03_06(dan_27_05);
    dan_03_06.DajPosljednjiPolazak().Ispisi();
    std::cout << std::boolalpha << dan_03_06.DajPosljednjiPolazak().DaLiKasni() << std::endl;

    std::cout << std::endl; // kopirajuci operator dodjele
    Polasci dan_10_06(3);
    dan_10_06 = dan_03_06;
    std::cout << dan_10_06.DajBrojPolazakaKojiKasne() << " / " << dan_10_06.DajBrojPolazaka() << std::endl;

    std::cout << "\n17.06.2023\n"; // obicni konstruktor
    Polasci dan_17_06(2);
    dan_17_06.RegistrirajPolazak("Pariz", "P4S", 2, 14, 40, 70);
    dan_17_06.RegistrirajPolazak(new Polazak({"Oslo", "LOS", 6, 3, 5, 100}));

    dan_17_06.Ispisi(); // sekvencijski konstruktor i pomjerajuci operator dodjele
    dan_17_06 = {{"Rim", "R0M", 3, 8, 10, 65}, {"Bihac", "B1H", 5, 6, 45, 45}};
    std::cout << std::endl << dan_17_06.DajPrviPolazak().DajTrajanje();
    dan_17_06.IsprazniKolekciju();

    int sati, minute; // pomjerajuci konstruktor
    const Polasci dan_24_06(std::move(dan_10_06));
    dan_24_06.DajPrviPolazak().OcekivanoVrijemePolaska(sati, minute);
    std::cout << std::endl << sati << ":" << minute;

    return 0;
}