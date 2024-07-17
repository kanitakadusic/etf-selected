#include <iostream>
#include <stdexcept>
#include <vector>

struct Cvor {
    int redni_broj;
    Cvor *sljedeci;
};

Cvor *KreirajKruznuListu(int broj_elemenata) {
    Cvor *pocetak{}, *pomocni{};

    try {
        for (int i = 1; i <= broj_elemenata; i++) {
            Cvor *novi = new Cvor{i, nullptr};
            if (!pocetak) pocetak = novi; else pomocni -> sljedeci = novi;
            pomocni = novi;
        }

        pomocni -> sljedeci = pocetak; return pocetak;
    } catch (...) {
        while (pocetak -> sljedeci != nullptr) {
            pomocni = pocetak -> sljedeci;
            delete pocetak; pocetak = pomocni;
        }

        delete pocetak; throw std::runtime_error("Nedovoljno memorije");
    }
}

int Poglavica(int broj_punoljetnih, int korak, bool ispisi = false) {
    if (broj_punoljetnih < 1 || korak < 1) return 0;

    Cvor *pocetak(KreirajKruznuListu(broj_punoljetnih));

    while (broj_punoljetnih-- != 1) {
        Cvor *prethodni{pocetak};
        for (int i = 1; i < korak; i++) { prethodni = pocetak; pocetak = pocetak -> sljedeci; }
        if (ispisi) std::cout << pocetak -> redni_broj << ", ";

        Cvor *trenutni = pocetak;
        pocetak = pocetak -> sljedeci;
        prethodni -> sljedeci = pocetak;
        delete trenutni;
    }

    int pozicija = pocetak -> redni_broj;
    delete pocetak; return pozicija;
}

int SigurnoMjesto(int korak, int donja_granica, int gornja_granica) {
    if (korak < 1 || donja_granica < 1 || donja_granica > gornja_granica) return 0;

    std::vector<bool> odabrane_pozicije(gornja_granica, false);
    for (int i = donja_granica; i <= gornja_granica; i++) odabrane_pozicije.at(Poglavica(i, korak) - 1) = true;

    for (int i = donja_granica; i <= gornja_granica; i++) if (!odabrane_pozicije.at(i - 1)) return i;
    return 0;
}

int main() {
    int broj_clanova;
    std::cout << "Unesite broj punoljetnih clanova za odabir poglavice plemena Wabambe: ";
    std::cin >> broj_clanova;

    int korak;
    std::cout << "Unesite korak razbrajanja: ";
    std::cin >> korak;

    int donja_granica, gornja_granica;
    std::cout << "Unesite raspon za odabir (N1 i N2): ";
    std::cin >> donja_granica >> gornja_granica;

    std::cout << "Redni broj osobe koja postaje poglavica: " << Poglavica(broj_clanova, korak) << std::endl;

    int sigurno_mjesto = SigurnoMjesto(korak, donja_granica, gornja_granica);
    if (sigurno_mjesto) std::cout << "Mbebe Mgogo, stani na sigurno mjesto " << sigurno_mjesto << " da ne bi bio poglavica!";
    else std::cout << "Zao mi je Mbebe Mgogo, ali nema sigurnog mjesta.";

    return 0;
}