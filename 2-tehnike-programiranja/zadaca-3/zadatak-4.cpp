#include <iostream>
#include <list>
#include <stdexcept>
#include <vector>

int Poglavica(int broj_punoljetnih, int korak, bool ispisi = false) {
    if (broj_punoljetnih < 1 || korak < 1) return 0;

    std::list<int> pozicije;
    for (int i = 1; i <= broj_punoljetnih; i++) pozicije.push_back(i);

    auto odabir(pozicije.begin());
    while (broj_punoljetnih-- != 1) {
        for (int i = 1; i < korak; i++) if (++odabir == pozicije.end()) odabir = pozicije.begin();
        if (ispisi) std::cout << *odabir << ", ";
        if ((odabir = pozicije.erase(odabir)) == pozicije.end()) odabir = pozicije.begin();
    }

    return *odabir;
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