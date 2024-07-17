#include <iostream>
#include <vector>

int NZD_LK_2(int a, int b, int& k_a, int& k_b) {
    int K11 = 1, K12 = 0, K21 = 0, K22 = 1;

    while (b != 0) {
        int K31 = K11 - a / b * K21, K32 = K12 - a / b * K22;

        int ostatak = a % b;
        a = b;
        b = ostatak;

        K11 = K21; K12 = K22;
        K21 = K31; K22 = K32;
    }

    if (a < 0) a *= -1, K11 *= -1, K12 *= -1;

    k_a = K11;
    k_b = K12;

    return a;
}

int NZD_LK_N(const std::vector<int>& brojevi, std::vector<int>& koeficijenti) {
    if (brojevi.size() < 2) throw std::domain_error("Ocekuju se minimalno 2 broja");

    koeficijenti.resize(brojevi.size());
    int x, nzd = NZD_LK_2(brojevi.at(0), brojevi.at(1), koeficijenti.at(0), koeficijenti.at(1));

    for (int i = 2; i < brojevi.size(); i++) {
        nzd = NZD_LK_2(nzd, brojevi.at(i), x, koeficijenti.at(i));

        for (int j = 0; j < i; j++)
            koeficijenti.at(j) *= x;
    }

    return nzd;
}

int main() {
    std::vector<int> brojevi{150, -105, -12, 8}, koeficijenti;
    int nzd = NZD_LK_N(brojevi, koeficijenti);

    std::cout << nzd << " = ";
    for (int i = 0; i < brojevi.size(); i++)
        std::cout << "(" << koeficijenti.at(i) << ") * " << brojevi.at(i) << (i != brojevi.size() - 1 ? " + " : "\n");

    return 0;
}
