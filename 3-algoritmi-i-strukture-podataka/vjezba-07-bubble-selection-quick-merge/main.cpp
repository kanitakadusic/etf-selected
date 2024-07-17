#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include <stdexcept>

using std::cout;
using std::endl;

template <typename Tip>
void bubble_sort(Tip* niz, int velicina) {
    for (int i = velicina - 1; i > 0; i--)
        for (int j = 1; j <= i; j++)
            if (niz[j - 1] > niz[j])
                std::swap(niz[j - 1], niz[j]);
}

template <typename Tip>
void selection_sort(Tip* niz, int velicina) {
    for (int i = 0; i < velicina - 1; i++) {
        int min = i;

        for (int j = i + 1; j < velicina; j++)
            if (niz[j] < niz[min])
                min = j;

        std::swap(niz[i], niz[min]);
    }
}

template <typename Tip>
int particija(Tip* niz, const int l, const int u) {
    Tip pivot = niz[l];
    int p = l + 1;

    while (p <= u)
        if (niz[p] <= pivot) p++;
        else break;

    for (int i = p + 1; i <= u; i++)
        if (niz[i] < pivot) {
            std::swap(niz[p], niz[i]);
            p++;
        }

    std::swap(niz[l], niz[p - 1]);
    return p - 1;
}

template <typename Tip>
void _quick_sort(Tip* niz, const int l, const int u) {
    if (u > l) {
        int j = particija(niz, l, u);

        _quick_sort(niz, l, j - 1);
        _quick_sort(niz, j + 1, u);
    }
}

template <typename Tip>
void quick_sort(Tip* niz, int velicina) {
    _quick_sort(niz, 0, velicina - 1);
}

template <typename Tip>
void merge(Tip* niz, const int l, const int p, const int q, const int u) {
    int i = 0, j = q - l, k = l;
    Tip* pomocni = new Tip[u - l + 1];

    for (int m = 0; m <= u - l; m++)
        pomocni[m] = niz[l + m];

    while (i <= p - l && j <= u - l) {
        if (pomocni[i] < pomocni[j]) niz[k] = pomocni[i++];
        else niz[k] = pomocni[j++];

        k++;
    }

    while (i <= p - l) niz[k++] = pomocni[i++];
    while (j <= u - l) niz[k++] = pomocni[j++];

    delete[] pomocni;
}

template <typename Tip>
void _merge_sort(Tip* niz, const int l, const int u) {
    if (u > l) {
        int p = (l + u - 1) / 2;
        int q = p + 1;

        _merge_sort(niz, l, p);
        _merge_sort(niz, q, u);

        merge(niz, l, p, q, u);
    }
}

template <typename Tip>
void merge_sort(Tip* niz, int velicina) {
    _merge_sort(niz, 0, velicina - 1);
}

void generisi(std::string filename, int velicina) {
    std::ofstream pisi(filename);
    std::srand(unsigned(std::time(NULL)));

    for (int i = 0; i < velicina; i++)
        pisi << std::rand() << '\n';
}

void ucitaj(std::string filename, int*& niz, int& velicina) {
    std::ifstream citaj(filename);
    if (!citaj) throw std::runtime_error("");

    velicina = 0;
    int unos;
    while (citaj >> unos) velicina++;

    niz = new int[velicina];
    citaj.clear();
    citaj.seekg(0);

    for (int i = 0; i < velicina; i++)
        citaj >> niz[i];
}

void testiraj(std::string ulazna, std::string izlazna) {
    int* niz, velicina, unos;
    ucitaj(ulazna, niz, velicina);

    std::cout << "1. Bubble sort\n"
              << "2. Selection sort\n"
              << "3. Quick sort\n"
              << "4. Merge sort\n" << std::endl;

    std::cout << "Izaberite algoritam sortiranja: ";
    std::cin >> unos;

    clock_t start = 0, end = 0;

    switch (unos) {
        case 1: start = std::clock(); bubble_sort(niz, velicina); end = std::clock(); break;
        case 2: start = std::clock(); selection_sort(niz, velicina); end = std::clock(); break;
        case 3: start = std::clock(); quick_sort(niz, velicina); end = std::clock(); break;
        case 4: start = std::clock(); merge_sort(niz, velicina); end = std::clock(); break;
    }

    for (int i = 0; i < velicina - 1; i++)
        if (niz[i] > niz[i + 1]) {
            std::cout << "Niz nije dobro sortiran.";
            return;
        }

    double vrijeme = double(end - start) / double(CLOCKS_PER_SEC / 1000);
    std::cout << "Vrijeme izvrsavanja je " << std::fixed << std::setprecision(5) << vrijeme << " ms.";

    std::ofstream pisi(izlazna);
    for (int i = 0; i < velicina; i++)
        pisi << niz[i] << '\n';

    delete[] niz;
}

int main() {
    //generisi("brojevi.txt", 10);
    //testiraj("brojevi.txt", "sortirani.txt");

    return 0;
}