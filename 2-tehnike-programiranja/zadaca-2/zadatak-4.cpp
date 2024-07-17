#include <iostream>
#include <vector>
#include <algorithm>

template <typename PomnoziviTip>
bool Kriterij(const std::vector<PomnoziviTip> &prvi_red, const std::vector<PomnoziviTip> &drugi_red) {
    if (prvi_red.empty() || drugi_red.empty()) return prvi_red < drugi_red;

    PomnoziviTip proizvod_prvog_reda(prvi_red.at(0)), proizvod_drugog_reda(drugi_red.at(0));

    for (int i = 1; i < prvi_red.size(); i++)
        proizvod_prvog_reda *= prvi_red.at(i);

    for (int i = 1; i < drugi_red.size(); i++)
        proizvod_drugog_reda *= drugi_red.at(i);

    return (proizvod_prvog_reda < proizvod_drugog_reda != proizvod_prvog_reda <= proizvod_drugog_reda) ? prvi_red < drugi_red : proizvod_prvog_reda < proizvod_drugog_reda;
}

template <typename UporediviTip>
void SortirajPoProizvoduRedova(std::vector<std::vector<UporediviTip>> &matrica) {
    std::sort(matrica.begin(), matrica.end(), Kriterij<UporediviTip>);
}

int main() {
    int broj_redova = 0, unos = 0;

    std::vector<std::vector<int>> matrica;
    std::cout << "Unesi elemente (* za kraj reda, * na pocetku reda za kraj unosa):\n";
    while (std::cin >> unos) {
        matrica.resize(++broj_redova);
        do { matrica.at(broj_redova - 1).push_back(unos); } while (std::cin >> unos);

        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    SortirajPoProizvoduRedova(matrica);
    std::cout << "\nMatrica nakon sortiranja:\n";
    for (auto &red : matrica) {
        for (int element : red) std::cout << element << " ";
        std::cout << std::endl;
    }

    std::cin.clear();
    std::cin.ignore(10000, '\n');

    std::vector<int> sekvenca;
    std::cout << "\nUnesite elemente sekvence koja se trazi (* za kraj reda): ";
    while (std::cin >> unos) sekvenca.push_back(unos);

    auto i_sekvenca = std::lower_bound(matrica.begin(), matrica.end(), sekvenca, Kriterij<int>);
    if (*i_sekvenca != sekvenca) std::cout << "\nTrazena sekvenca se ne nalazi u matrici";
    else std::cout << "\nTrazena sekvenca se nalazi u " << i_sekvenca - matrica.begin() + 1 << ". redu (nakon sortiranja)";

    return 0;
}