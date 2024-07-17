#include <iostream>
#include <string>
#include <tuple>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cctype>

using Knjiga = std::map<std::string, std::vector<std::string>>;
using Indeks = std::map<std::string, std::set<std::tuple<std::string, int, int>>>;

Indeks KreirajIndeksPojmova(const Knjiga &knjiga) {
    Indeks indeks;

    for (auto &poglavlje : knjiga) {
        int broj_stranice = 1;

        for (auto &stranica : poglavlje.second) {
            auto pocetak_rijeci(std::find_if(stranica.begin(), stranica.end(), [](char slovo) { return std::isalpha(slovo) || std::isdigit(slovo); }));

            while (pocetak_rijeci != stranica.end()) {
                auto kraj_rijeci(std::find_if(pocetak_rijeci, stranica.end(), [](char slovo) { return !(std::isalpha(slovo) || std::isdigit(slovo)); }));

                std::string rijec;
                std::transform(pocetak_rijeci, kraj_rijeci, std::back_inserter(rijec), [](char slovo) { return std::tolower(slovo); });

                indeks[rijec].insert({poglavlje.first, broj_stranice, pocetak_rijeci - stranica.begin()});

                pocetak_rijeci = std::find_if(kraj_rijeci, stranica.end(), [](char slovo) { return std::isalpha(slovo) || std::isdigit(slovo); });
            }

            broj_stranice++;
        }
    }

    return indeks;
}

auto PretraziIndeksPojmova(std::string rijec, const Indeks &indeks) {
    std::transform(rijec.begin(), rijec.end(), rijec.begin(), [](char slovo) { return std::tolower(slovo); });
    auto i_rijec(indeks.find(rijec));

    if (i_rijec != indeks.end()) return i_rijec -> second;
    throw std::logic_error("Pojam nije nadjen");
}

void IspisiIndeksPojmova(const Indeks &indeks) {
    for (auto &pojam_podaci : indeks) {
        std::cout << pojam_podaci.first << ": ";

        auto kraj_seta(pojam_podaci.second.end()); kraj_seta--;
        for (auto podatak(pojam_podaci.second.begin()); podatak != pojam_podaci.second.end(); podatak++)
            std::cout << std::get<0>(*podatak) << "/" << std::get<1>(*podatak) << "/" << std::get<2>(*podatak) << (podatak != kraj_seta ? ", " : "");

        std::cout << std::endl;
    }
}

int main() {
    Knjiga knjiga;

    for (;;) {
        std::string naziv_poglavlja;
        std::cout << "Unesite naziv poglavlja: ";
        std::getline(std::cin, naziv_poglavlja);

        if (naziv_poglavlja == ".") break;
        knjiga[naziv_poglavlja];

        int brojac = 0;
        for (;;) {
            std::string sadrzaj_stranice;
            std::cout << "Unesite sadrzaj stranice " << ++brojac << ": ";
            std::getline(std::cin, sadrzaj_stranice);

            if (sadrzaj_stranice == ".") break;
            knjiga[naziv_poglavlja].emplace_back(sadrzaj_stranice);
        }
    }

    Indeks indeks(KreirajIndeksPojmova(knjiga));
    std::cout << "\nKreirani indeks pojmova:\n";
    IspisiIndeksPojmova(indeks);

    for (;;) {
        std::string rijec_pretraga;
        std::cout << "\nUnesite rijec: ";
        std::cin >> rijec_pretraga;

        if (rijec_pretraga == ".") break;
        try {
            auto podaci(PretraziIndeksPojmova(rijec_pretraga, indeks));
            for (auto &podatak : podaci)
                std::cout << std::get<0>(podatak) << "/" << std::get<1>(podatak) << "/" << std::get<2>(podatak) << " ";
        } catch (...) {
            std::cout << "Unesena rijec nije nadjena!";
        }
    }

    return 0;
}