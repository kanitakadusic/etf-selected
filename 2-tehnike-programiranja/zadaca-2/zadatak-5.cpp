#include <iostream>
#include <stdexcept>
#include <vector>
#include <deque>

template <typename IPS, typename IDS>
auto KreirajTablicuSabiranja(IPS pocetak_prve, IPS kraj_prve, IDS pocetak_druge) {
    try {
        int duzina_sekvence = 0;
        IPS pomocni_pocetak_prve = pocetak_prve;
        while (pomocni_pocetak_prve != kraj_prve) {
            duzina_sekvence++;
            pomocni_pocetak_prve++;
        }

        auto tablica = new decltype(*pocetak_prve + *pocetak_druge)*[duzina_sekvence];
        try {
            tablica[0] = new decltype(*pocetak_prve + *pocetak_druge)[duzina_sekvence * (duzina_sekvence + 1) / 2];
        } catch (...) {
            delete[] tablica;
            throw;
        }

        IDS pomocni_pocetak_druge = pocetak_druge;
        for (int i = 0; i < duzina_sekvence; i++) {
            if (i < duzina_sekvence - 1) tablica[i + 1] = tablica[i] + i + 1;

            for (int j = 0; j <= i; j++) {
                if (*pocetak_prve + *pocetak_druge != *pocetak_druge + *pocetak_prve) {
                    delete[] tablica[0];
                    delete[] tablica;
                    throw std::logic_error("Nije ispunjena pretpostavka o komutativnosti");
                }

                tablica[i][j] = *pocetak_prve + *pocetak_druge++;
            }

            pocetak_prve++;
            pocetak_druge = pomocni_pocetak_druge;
        }

        return tablica;
    } catch (std::logic_error &izuzetak) {
        throw izuzetak;
    } catch (...) {
        throw std::range_error("Nema dovoljno memorije!");
    }
}

int main() {
    try {
        int duzina_sekvence;
        std::cout << "Duzina sekvenci: ";
        std::cin >> duzina_sekvence;

        std::vector<double> prva_sekvenca(duzina_sekvence);
        std::cout << "\nElementi prve sekvence: ";
        for (double &element : prva_sekvenca) std::cin >> element;

        std::deque<double> druga_sekvenca(duzina_sekvence);
        std::cout << "\nElementi druge sekvence: ";
        for (double &element : druga_sekvenca) std::cin >> element;

        double **tablica = KreirajTablicuSabiranja(prva_sekvenca.begin(), prva_sekvenca.end(), druga_sekvenca.begin());
        std::cout << "\nTablica sabiranja:\n";
        for (int i = 0; i < duzina_sekvence; i++) {
            for (int j = 0; j <= i; j++) std::cout << tablica[i][j] << " ";
            std::cout << std::endl;
        }

        delete[] tablica[0];
        delete[] tablica;
    } catch (std::exception &izuzetak) {
        std::cout << izuzetak.what();
    }

    return 0;
}