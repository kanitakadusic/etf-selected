#include <iostream>
#include <string>
#include <stdexcept>
#include <map>
#include <memory>

// ___________________________________________________________ KORISNIK

class Korisnik {
    int clanski_broj;
    std::string ime_prezime, adresa, telefon;

public:
    Korisnik(int clanski_broj, std::string ime_prezime, std::string adresa, std::string telefon);


    int DajClanskiBroj() const { return this->clanski_broj; }
    std::string DajImeIPrezime() const { return this->ime_prezime; }
    std::string DajAdresu() const { return this->adresa; }
    std::string DajTelefon() const { return this->telefon; }

    void Ispisi() const;
};

// ---------------

Korisnik::Korisnik(int clanski_broj, std::string ime_prezime, std::string adresa, std::string telefon):
        clanski_broj(clanski_broj), ime_prezime(std::move(ime_prezime)), adresa(std::move(adresa)), telefon(std::move(telefon)) {}

// ---------------

void Korisnik::Ispisi() const {
    std::cout << "Clanski broj: " << DajClanskiBroj();
    std::cout << "\nIme i prezime: " << DajImeIPrezime();
    std::cout << "\nAdresa: " << DajAdresu();
    std::cout << "\nTelefon: " << DajTelefon() << std::endl;
}

// ___________________________________________________________ FILM

class Film {
    int evidencijski_broj, godina_produkcije;
    bool sniman_na_DVD_u;
    std::string naziv, zanr;
    Korisnik *zaduzenje;

public:
    Film(int evidencijski_broj, bool sniman_na_DVD_u, std::string naziv, std::string zanr, int godina_produkcije);


    int DajEvidencijskiBroj() const { return this->evidencijski_broj; }
    std::string DajNaziv() const { return this->naziv; }
    std::string DajZanr() const { return this->zanr; }
    int DajGodinuProdukcije() const { return this->godina_produkcije; }
    bool DaLiJeDVD() const { return this->sniman_na_DVD_u; }
    bool DaLiJeZaduzen() const { return this->zaduzenje != nullptr; }

    Korisnik &DajKodKogaJe() { if (!DaLiJeZaduzen()) throw std::domain_error("Film nije zaduzen"); return *(this->zaduzenje); }
    Korisnik *DajPokKodKogaJe() { return this->zaduzenje; }

    void Ispisi() const;

    Film &ZaduziFilm(Korisnik &korisnik) { this->zaduzenje = &korisnik; return *this; }
    Film &RazduziFilm() { this->zaduzenje = nullptr; return *this; }
};

// ---------------

Film::Film(int evidencijski_broj, bool sniman_na_DVD_u, std::string naziv, std::string zanr, int godina_produkcije):
        evidencijski_broj(evidencijski_broj), sniman_na_DVD_u(sniman_na_DVD_u), naziv(std::move(naziv)), zanr(std::move(zanr)), godina_produkcije(godina_produkcije), zaduzenje(nullptr) {}

// ---------------

void Film::Ispisi() const {
    std::cout << "Evidencijski broj: " << DajEvidencijskiBroj();
    std::cout << "\nMedij: " << (DaLiJeDVD() ? "DVD" : "Video traka");
    std::cout << "\nNaziv filma: " << DajNaziv();
    std::cout << "\nZanr: " << DajZanr();
    std::cout << "\nGodina produkcije: " << DajGodinuProdukcije() << std::endl;
}

// ___________________________________________________________ VIDEOTEKA

class Videoteka {
    std::map<int, std::shared_ptr<Korisnik>> korisnici;
    std::map<int, std::shared_ptr<Film>> filmovi;

public:
    Videoteka() = default;

    Videoteka(const Videoteka &videoteka);
    Videoteka &operator =(const Videoteka &videoteka);

    Videoteka(Videoteka &&videoteka) = default;
    Videoteka &operator =(Videoteka &&videoteka) = default;


    Videoteka &RegistrirajNovogKorisnika(int clanski_broj, std::string ime_prezime, std::string adresa, std::string telefon);
    Videoteka &RegistrirajNoviFilm(int evidencijski_broj, bool sniman_na_DVD_u, std::string naziv, std::string zanr, int godina_produkcije);

    Korisnik &NadjiKorisnika(int clanski_broj);
    Film &NadjiFilm(int evidencijski_broj);

    void IzlistajKorisnike() const;
    void IzlistajFilmove() const;

    Videoteka &ZaduziFilm(int evidencijski_broj, int clanski_broj);
    Videoteka &RazduziFilm(int evidencijski_broj);

    void PrikaziZaduzenja(int clanski_broj) const;
};

// ---------------

Videoteka::Videoteka(const Videoteka &nova) {
    for (auto &korisnik : nova.korisnici)
        this->korisnici[korisnik.first] = std::make_shared<Korisnik>(*(korisnik.second));

    for (auto &film : nova.filmovi)
        this->filmovi[film.first] = std::make_shared<Film>(*(film.second));
}

Videoteka &Videoteka::operator =(const Videoteka &nova) {
    for (auto &stari_korisnik : this->korisnici)
        if (!nova.korisnici.count(stari_korisnik.first)) this->korisnici.erase(stari_korisnik.first);

    for (auto &novi_korisnik : nova.korisnici)
        this->korisnici[novi_korisnik.first] = std::make_shared<Korisnik>(*(novi_korisnik.second));

    for (auto &stari_film : this->filmovi)
        if (!nova.filmovi.count(stari_film.first)) this->filmovi.erase(stari_film.first);

    for (auto &novi_film : nova.filmovi)
        this->filmovi[novi_film.first] = std::make_shared<Film>(*(novi_film.second));

/*
    // II nacin - mozda efikasniji ?

    this->korisnici.clear(); // nema u predavanjima, ali nije neka nauka
    for (auto &korisnik : nova.korisnici)
        this->korisnici[korisnik.first] = std::make_shared<Korisnik>(*(korisnik.second));

    this->filmovi.clear();
    for (auto &film : nova.filmovi)
        this->filmovi[film.first] = std::make_shared<Film>(*(film.second));
*/

    return *this;
}

// ---------------

Videoteka &Videoteka::RegistrirajNovogKorisnika(int clanski_broj, std::string ime_prezime, std::string adresa, std::string telefon) {
    if (this->korisnici.count(clanski_broj)) throw std::logic_error("Vec postoji korisnik s tim clanskim brojem");
    else this->korisnici[clanski_broj] = std::make_shared<Korisnik>(clanski_broj, std::move(ime_prezime), std::move(adresa), std::move(telefon));

    return *this;
}

Videoteka &Videoteka::RegistrirajNoviFilm(int evidencijski_broj, bool sniman_na_DVD_u, std::string naziv, std::string zanr, int godina_produkcije) {
    if (this->filmovi.count(evidencijski_broj)) throw std::logic_error("Film s tim evidencijskim brojem vec postoji");
    else this->filmovi[evidencijski_broj] = std::make_shared<Film>(evidencijski_broj, sniman_na_DVD_u, std::move(naziv), std::move(zanr), godina_produkcije);

    return *this;
}

Korisnik &Videoteka::NadjiKorisnika(int clanski_broj) {
    try { return *(this->korisnici.at(clanski_broj)); }
    catch (...) { throw std::logic_error("Korisnik nije nadjen"); }
}

Film &Videoteka::NadjiFilm(int evidencijski_broj) {
    try { return *(this->filmovi.at(evidencijski_broj)); }
    catch (...) { throw std::logic_error("Film nije nadjen"); }
}

void Videoteka::IzlistajKorisnike() const {
    for (auto &korisnik : this->korisnici) {
        korisnik.second->Ispisi();

        std::cout << std::endl;
    }
}

void Videoteka::IzlistajFilmove() const {
    for (auto &film : this->filmovi) {
        film.second->Ispisi();

        if (film.second->DaLiJeZaduzen())
            std::cout << "Zaduzen kod korisnika: " << film.second->DajKodKogaJe().DajImeIPrezime()
                      << " (" << film.second->DajKodKogaJe().DajClanskiBroj() << ")\n";

        std::cout << std::endl;
    }
}

Videoteka &Videoteka::ZaduziFilm(int evidencijski_broj, int clanski_broj) {
    auto i_film = this->filmovi.find(evidencijski_broj);
    if (i_film == this->filmovi.end()) throw std::logic_error("Film nije nadjen");

    auto i_korisnik = this->korisnici.find(clanski_broj);
    if (i_korisnik == this->korisnici.end()) throw std::logic_error("Korisnik nije nadjen");

    if (i_film->second->DaLiJeZaduzen()) throw std::logic_error("Film vec zaduzen");
    i_film->second->ZaduziFilm(*(i_korisnik->second));

    return *this;
}

Videoteka &Videoteka::RazduziFilm(int evidencijski_broj) {
    auto i_film = this->filmovi.find(evidencijski_broj);
    if (i_film == this->filmovi.end()) throw std::logic_error("Film nije nadjen");
    if (!i_film->second->DaLiJeZaduzen()) throw std::logic_error("Film nije zaduzen");

    i_film->second->RazduziFilm();
    return *this;
}

void Videoteka::PrikaziZaduzenja(int clanski_broj) const {
    if (!this->korisnici.count(clanski_broj)) throw std::logic_error("Korisnik nije nadjen");

    bool ima_zaduzenja = false;

    for (auto &film : this->filmovi)
        if (film.second->DajPokKodKogaJe() != nullptr && film.second->DajPokKodKogaJe()->DajClanskiBroj() == clanski_broj) {
            if (ima_zaduzenja) std::cout << std::endl;
            film.second->Ispisi();

            ima_zaduzenja = true;
        }

    if (!ima_zaduzenja) std::cout << "Korisnik nema zaduzenja!\n";
    std::cout << std::endl;
}

// ___________________________________________________________ MAIN

int main() {
    std::cout << "1 - Registriraj novog korisnika\n"
              << "2 - Registriraj novi film\n"
              << "3 - Izlistaj korisnike\n"
              << "4 - Izlistaj filmove\n"
              << "5 - Zaduzi film\n"
              << "6 - Razduzi film\n"
              << "7 - Prikazi zaduzenja\n"
              << "0 - Kraj\n\n";

    void RNG(Videoteka &videoteka);
    void RNF(Videoteka &videoteka);
    void IK(Videoteka &videoteka);
    void IF(Videoteka &videoteka);
    void ZF(Videoteka &videoteka);
    void RF(Videoteka &videoteka);
    void PZ(Videoteka &videoteka);

    Videoteka videoteka;

    for (;;) {
        int opcija;
        std::cout << std::string(35, '-') << "\n=> Odaberite opciju: ";
        std::cin >> opcija;
        std::cout << std::string(35, '-') << std::endl;

        if (opcija < 1 || opcija > 7) break;
        void (*opcije[7])(Videoteka &videoteka){RNG, RNF, IK, IF, ZF, RF, PZ};

        try { opcije[opcija - 1](videoteka); }
        catch (std::exception &izuzetak) { std::cout << izuzetak.what() << "\n\n"; }
    }

    return 0;
}

void RNG(Videoteka &videoteka) {
    int clanski_broj;
    std::cout << "Clanski broj: ";
    std::cin >> clanski_broj;

    std::cin.ignore(10000, '\n');

    std::string ime_prezime;
    std::cout << "Ime i prezime: ";
    std::getline(std::cin, ime_prezime);

    std::string adresa;
    std::cout << "Adresa: ";
    std::getline(std::cin, adresa);

    std::string telefon;
    std::cout << "Telefon: ";
    std::getline(std::cin, telefon);

    std::cout << std::endl;
    videoteka.RegistrirajNovogKorisnika(clanski_broj, ime_prezime, adresa, telefon);
}

void RNF(Videoteka &videoteka) {
    int evidencijski_broj;
    std::cout << "Evidencijski broj: ";
    std::cin >> evidencijski_broj;

    bool sniman_na_DVD_u;
    std::cout << "Da li je sniman na DVD-u? (1/0): ";
    std::cin >> sniman_na_DVD_u;

    std::cin.ignore(10000, '\n');

    std::string naziv;
    std::cout << "Naziv: ";
    std::getline(std::cin, naziv);

    std::string zanr;
    std::cout << "Zanr: ";
    std::getline(std::cin, zanr);

    int godina_produkcije;
    std::cout << "Godina produkcije: ";
    std::cin >> godina_produkcije;

    std::cout << std::endl;
    videoteka.RegistrirajNoviFilm(evidencijski_broj, sniman_na_DVD_u, naziv, zanr, godina_produkcije);
}

void IK(Videoteka &videoteka) {
    videoteka.IzlistajKorisnike();
}

void IF(Videoteka &videoteka) {
    videoteka.IzlistajFilmove();
}

void ZF(Videoteka &videoteka) {
    int evidencijski_broj;
    std::cout << "Evidencijski broj filma: ";
    std::cin >> evidencijski_broj;

    int clanski_broj;
    std::cout << "Clanski broj korisnika: ";
    std::cin >> clanski_broj;

    std::cout << std::endl;
    videoteka.ZaduziFilm(evidencijski_broj, clanski_broj);
}

void RF(Videoteka &videoteka) {
    int evidencijski_broj;
    std::cout << "Evidencijski broj: ";
    std::cin >> evidencijski_broj;

    std::cout << std::endl;
    videoteka.RazduziFilm(evidencijski_broj);
}

void PZ(Videoteka &videoteka) {
    int clanski_broj;
    std::cout << "Clanski broj: ";
    std::cin >> clanski_broj;

    std::cout << std::endl;
    videoteka.PrikaziZaduzenja(clanski_broj);
}