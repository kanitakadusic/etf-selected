#include <iostream>
#include <cmath>

bool Prost(int broj) {
    if (broj == 0 || broj == 1) return false;

    int korijen = int(std::sqrt(std::abs(static_cast<long long int>(broj))));
    for (int i = 2; i <= korijen; i++) if (broj % i == 0) return false;

    return true;
}

int BrojDijeljenja(int &broj, int faktor, int &baza, int &kvadrat) {
    int stepen = 0;
    while (broj % faktor == 0) { stepen++; broj /= faktor; }

    baza *= (stepen % 2 == 0) ? 1 : faktor;
    kvadrat *= int(std::pow(faktor, stepen / 2));

    return stepen;
}

void RastavaBroja(int broj, int &baza, int &kvadrat) {
    if (broj == 0) { baza = 0; kvadrat = 1; return; }
    if (Prost(broj)) { baza = broj; kvadrat = 1; return; }

    baza = kvadrat = 1;

    if (BrojDijeljenja(broj, 2, baza, kvadrat) != 0 && Prost(broj)) { baza *= broj; return; }

    for (int faktor = 3; broj != 1 && broj != -1; faktor += 2)
        if (Prost(faktor) && BrojDijeljenja(broj, faktor, baza, kvadrat) != 0 && Prost(broj)) { baza *= broj; return; }
}

int main() {
    int broj, baza, kvadrat;
    std::cout << "Unesi broj: ";
    std::cin >> broj;

    RastavaBroja(broj, baza, kvadrat);
    std::cout << std::endl << broj << " = " << baza << "*" << kvadrat << "^2";

    return 0;
}