// bolje rjesenje - vanjskom petljom kroz zabranjene rijeci, a unutrasnjom kroz string

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

bool SlovoCifra(char x) {
    return (x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') || (x >= '0' && x <= '9');
}

bool IstiKarakter(char x, char y) {
    return (x == y) || (x == y + 32) || (x == y - 32);
}

bool IstiSadrzaj(const std::string &x, const std::string &y) {
    if (x.length() != y.length()) return false;

    for (int i = 0; i < x.length(); i++)
        if (!IstiKarakter(x.at(i), y.at(i))) return false;

    return true;
}

bool IspravneRijeci(const std::vector<std::string> &spisak) {
    for (const std::string &rijec : spisak)
        for (char slovo : rijec)
            if (!SlovoCifra(slovo)) return false;

    return true;
}

std::string Cenzura(std::string recenica, const std::vector<std::string> &spisak) {
    if (!IspravneRijeci(spisak)) throw std::logic_error("Neispravna riječ");

    bool rijec_u_toku = false;
    int brojac_karaktera = 0;

    for (int i = 0; i < recenica.length(); i++) {
        if (SlovoCifra(recenica.at(i))) brojac_karaktera++;

        if (rijec_u_toku && (i == recenica.length() - 1 || !SlovoCifra(recenica.at(i)))) {
            bool kraj = SlovoCifra(recenica.at(i));

            for (const std::string &vulgarizam : spisak) {
                if (IstiSadrzaj(recenica.substr(i + kraj - brojac_karaktera, brojac_karaktera), vulgarizam)) {
                    for (int j = i + kraj - brojac_karaktera; j < i + kraj; j++) {
                        recenica.at(j) = '*';
                    } break;
                }
            }

            brojac_karaktera = 0;
        }

        rijec_u_toku = SlovoCifra(recenica.at(i));
    }

    return recenica;
}

int main() {
    std::string recenica;
    std::cout << "Unesite recenicu:";
    std::getline(std::cin, recenica);

    try {
        std::string rijec;
        std::vector<std::string> spisak;
        std::cout << "\nUnesite zabranjene rijeci (. za kraj):";
        while (rijec != ".") {
            std::cin >> rijec;
            if (rijec != ".") spisak.push_back(rijec);
        }

        recenica = Cenzura(recenica, spisak);
        std::cout << "\nCenzurisana recenica: " << recenica;
    } catch (...) {
        std::cout << "\nGRESKA: Nelegalne zabranjene rijeci!";
    }

    return 0;
}