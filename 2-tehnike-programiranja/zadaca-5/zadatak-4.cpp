#include <iostream>
#include <iomanip>
#include <cstring>
#include <stdexcept>
#include <new>
#include <string>
#include <fstream>


template <typename TipElemenata>
class Matrica {
    char ime;
    int broj_redova, broj_kolona;
    TipElemenata **elementi;

    static TipElemenata **AlocirajMemoriju(int broj_redova, int broj_kolona);
    static void DealocirajMemoriju(TipElemenata **elementi, int broj_redova);

    void KopirajElemente(TipElemenata **elementi);
    void TestIndeksa(int red, int kolona) const {
        if (red < 1 || red > this->broj_redova || kolona < 1 || kolona > this->broj_kolona)
            throw std::range_error("Neispravan indeks");
    }

public:
    Matrica(int broj_redova, int broj_kolona, char ime = 0);

    Matrica(const Matrica &matrica);
    Matrica &operator =(const Matrica &matrica);

    Matrica(Matrica &&matrica);
    Matrica &operator =(Matrica &&matrica);

    ~Matrica() { DealocirajMemoriju(this->elementi, this->broj_redova); }


    Matrica(const std::string &naziv, bool binarna);

    void SacuvajUTekstualnuDatoteku(const std::string &naziv) const;
    void SacuvajUBinarnuDatoteku(const std::string &naziv) const;
    void ObnoviIzTekstualneDatoteke(const std::string &naziv);
    void ObnoviIzBinarneDatoteke(const std::string &naziv);


    template <typename Tip>
    friend std::istream &operator >>(std::istream &tok, Matrica<Tip> &matrica);
    template <typename Tip>
    friend std::ostream &operator <<(std::ostream &tok, const Matrica<Tip> &matrica);


    Matrica &operator +=(const Matrica &matrica);
    template <typename Tip>
    friend Matrica<Tip> operator +(Matrica<Tip> prva, const Matrica<Tip> &druga);

    Matrica &operator -=(const Matrica &matrica);
    template <typename Tip>
    friend Matrica<Tip> operator -(Matrica<Tip> prva, const Matrica<Tip> &druga);

    Matrica &operator *=(const Matrica &matrica);
    template <typename Tip>
    friend Matrica<Tip> operator *(Matrica<Tip> prva, const Matrica<Tip> &druga);


    Matrica &operator *=(double skalar);
    template <typename Tip>
    friend Matrica<Tip> operator *(Matrica<Tip> matrica, double skalar);
    template <typename Tip>
    friend Matrica<Tip> operator *(double skalar, Matrica<Tip> matrica);


    TipElemenata operator ()(int red, int kolona) const { TestIndeksa(red, kolona); return this->elementi[red - 1][kolona - 1]; }
    TipElemenata &operator ()(int red, int kolona) { TestIndeksa(red, kolona); return this->elementi[red - 1][kolona - 1]; }

    TipElemenata *operator [](int red) const { return this->elementi[red]; }
    TipElemenata *&operator [](int red) { return this->elementi[red]; }


    operator std::string() const;
};


template <typename TipElemenata>
Matrica<TipElemenata>::Matrica(const std::string &naziv, bool binarna):
broj_redova(0), broj_kolona(0), ime(0), elementi(nullptr) {
    if (binarna) ObnoviIzBinarneDatoteke(naziv);
    else ObnoviIzTekstualneDatoteke(naziv);
}

template <typename TipElemenata>
void Matrica<TipElemenata>::SacuvajUTekstualnuDatoteku(const std::string &naziv) const {
    std::ofstream datoteka(naziv);

    for (int i = 0; i < this->broj_redova; i++) {
        for (int j = 0; j < this->broj_kolona; j++)
            if (!(datoteka << this->elementi[i][j] << (j == this->broj_kolona - 1 ? "" : ",")))
                throw std::logic_error("Problemi sa upisom u datoteku");

        if (i != this->broj_redova - 1) datoteka << '\n';
    }
}

template <typename TipElemenata>
void Matrica<TipElemenata>::SacuvajUBinarnuDatoteku(const std::string &naziv) const {
    std::ofstream datoteka(naziv, std::ios::binary);

    datoteka.write(reinterpret_cast<const char*>(this), sizeof(*this));
    for (int i = 0; i < this->broj_redova; i++)
        if (!(datoteka.write(reinterpret_cast<char*>(this->elementi[i]), this->broj_kolona * sizeof(*this->elementi[i]))))
            throw std::logic_error("Problemi sa upisom u datoteku");
}

template <typename TipElemenata>
void Matrica<TipElemenata>::ObnoviIzTekstualneDatoteke(const std::string &naziv) {
    std::ifstream datoteka(naziv);
    if (!datoteka) throw std::logic_error("Trazena datoteka ne postoji");

    int redovi = 1, kolone = 1;

    {
        TipElemenata element;
        std::logic_error izuzetak("Datoteka sadrzi besmislene podatke");

        while (datoteka >> element && datoteka.peek() != '\n' && datoteka.peek() != EOF) {
            if (datoteka.get() != ',') throw izuzetak;
            kolone++;
        }

        for (;;) {
            if (!datoteka && datoteka.eof() || datoteka.peek() == EOF || datoteka.get() == EOF) break;
            int trenutne_kolone = 1;

            while (datoteka >> element && datoteka.peek() != '\n' && datoteka.peek() != EOF) {
                if (datoteka.get() != ',') throw izuzetak;
                trenutne_kolone++;
            }

            if (!datoteka && !datoteka.eof() || trenutne_kolone != kolone) throw izuzetak;
            redovi++;
        }
    }

    datoteka.clear();
    datoteka.seekg(0);

    Matrica<TipElemenata> obnovljena_matrica(redovi, kolone);

    for (int i = 0; i < obnovljena_matrica.broj_redova; i++)
        for (int j = 0; j < obnovljena_matrica.broj_kolona; j++) {
            datoteka >> obnovljena_matrica.elementi[i][j];
            if (datoteka.peek() == ',') datoteka.get();
        }

    *this = std::move(obnovljena_matrica);
}

template <typename TipElemenata>
void Matrica<TipElemenata>::ObnoviIzBinarneDatoteke(const std::string &naziv) {
    std::ifstream datoteka(naziv, std::ios::binary);
    if (!datoteka) throw std::logic_error("Trazena datoteka ne postoji");

    Matrica<TipElemenata> obnovljena_matrica(0, 0);
    DealocirajMemoriju(obnovljena_matrica.elementi, obnovljena_matrica.broj_redova);

    datoteka.read(reinterpret_cast<char*>(&obnovljena_matrica), sizeof(obnovljena_matrica));
    obnovljena_matrica.elementi = AlocirajMemoriju(obnovljena_matrica.broj_redova, obnovljena_matrica.broj_kolona);

    for (int i = 0; i < obnovljena_matrica.broj_redova; i++)
        if (!(datoteka.read(reinterpret_cast<char*>(obnovljena_matrica.elementi[i]), obnovljena_matrica.broj_kolona * sizeof(*obnovljena_matrica.elementi[i]))))
            throw std::logic_error("Datoteka sadrzi besmislene podatke");

    *this = std::move(obnovljena_matrica);
}


template <typename TipElemenata>
Matrica<TipElemenata>::operator std::string() const {
    std::string string_matrica = "{";

    for (int i = 0; i < this->broj_redova; i++) {
        string_matrica += "{";

        for (int j = 0; j < this->broj_kolona; j++) {
            string_matrica += std::to_string(int(elementi[i][j]));
            if (j < this->broj_kolona - 1) string_matrica += ",";
        }

        string_matrica += "}";
        if (i < this->broj_redova - 1) string_matrica += ",";
    }

    string_matrica += "}";
    return string_matrica;
}


template <typename TipElemenata>
TipElemenata **Matrica<TipElemenata>::AlocirajMemoriju(int broj_redova, int broj_kolona) {
    auto elementi = new TipElemenata*[broj_redova]{};

    try {
        for (int i = 0; i < broj_redova; i++)
            elementi[i] = new TipElemenata[broj_kolona]{};
    } catch (...) {
        DealocirajMemoriju(elementi, broj_redova); throw;
    }

    return elementi;
}

template <typename TipElemenata>
void Matrica<TipElemenata>::DealocirajMemoriju(TipElemenata **elementi, int broj_redova) {
    for (int i = 0; i < broj_redova; i++) delete[] elementi[i];
    delete[] elementi;
}


template <typename TipElemenata>
Matrica<TipElemenata>::Matrica(int broj_redova, int broj_kolona, char ime):
broj_redova(broj_redova), broj_kolona(broj_kolona), ime(ime), elementi(AlocirajMemoriju(broj_redova, broj_kolona)) {}

template <typename TipElemenata>
void Matrica<TipElemenata>::KopirajElemente(TipElemenata **elementi) {
    for (int i = 0; i < this->broj_redova; i++)
        for (int j = 0; j < this->broj_kolona; j++)
            this->elementi[i][j] = elementi[i][j];
}

template <typename TipElemenata>
Matrica<TipElemenata>::Matrica(const Matrica<TipElemenata> &matrica):
broj_redova(matrica.broj_redova), broj_kolona(matrica.broj_kolona), ime(matrica.ime), elementi(AlocirajMemoriju(matrica.broj_redova, matrica.broj_kolona)) {
    KopirajElemente(matrica.elementi);
}

template <typename TipElemenata>
Matrica<TipElemenata>::Matrica(Matrica<TipElemenata> &&matrica):
broj_redova(matrica.broj_redova), broj_kolona(matrica.broj_kolona), elementi(matrica.elementi), ime(matrica.ime) {
    matrica.broj_redova = 0; matrica.elementi = nullptr;
}

template <typename TipElemenata>
Matrica<TipElemenata> &Matrica<TipElemenata>::operator =(const Matrica<TipElemenata> &matrica) {
    if (this->broj_redova < matrica.broj_redova || this->broj_kolona < matrica.broj_kolona) {
        auto novi_prostor = AlocirajMemoriju(matrica.broj_redova, matrica.broj_kolona);
        DealocirajMemoriju(this->elementi, this->broj_redova);

        this->elementi = novi_prostor;
    } else if (this->broj_redova > matrica.broj_redova) {
        for (int i = matrica.broj_redova; i < this->broj_redova; i++)
            delete elementi[i];
    }

    this->broj_redova = matrica.broj_redova;
    this->broj_kolona = matrica.broj_kolona;
    this->ime = matrica.ime;

    KopirajElemente(matrica.elementi);
    return *this;
}

template <typename TipElemenata>
Matrica<TipElemenata> &Matrica<TipElemenata>::operator =(Matrica<TipElemenata> &&matrica) {
    std::swap(this->broj_redova, matrica.broj_redova);
    std::swap(this->broj_kolona, matrica.broj_kolona);
    std::swap(this->ime, matrica.ime);
    std::swap(this->elementi, matrica.elementi);

    return *this;
}


template <typename TipElemenata>
std::istream &operator >>(std::istream &tok, Matrica<TipElemenata> &matrica) {
    for (int i = 0; i < matrica.broj_redova; i++)
        for (int j = 0; j < matrica.broj_kolona; j++) {
            std::cout << matrica.ime << "(" << i + 1 << "," << j + 1 << ") = ";
            tok >> matrica.elementi[i][j];
        }

    return tok;
}

template <typename TipElemenata>
std::ostream &operator <<(std::ostream &tok, const Matrica<TipElemenata> &matrica) {
    auto sirina_ispisa = tok.width();

    for (int i = 0; i < matrica.broj_redova; i++) {
        for (int j = 0; j < matrica.broj_kolona; j++)
            tok << std::setw(sirina_ispisa) << matrica.elementi[i][j];

        tok << std::endl;
    }

    return tok;
}


template <typename TipElemenata>
Matrica<TipElemenata> &Matrica<TipElemenata>::operator +=(const Matrica<TipElemenata> &matrica) {
    if (this->broj_redova != matrica.broj_redova || this->broj_kolona != matrica.broj_kolona)
        throw std::domain_error("Matrice nemaju jednake dimenzije!");

    for (int i = 0; i < this->broj_redova; i++)
        for (int j = 0; j < this->broj_kolona; j++)
            this->elementi[i][j] += matrica.elementi[i][j];

    return *this;
}

template <typename TipElemenata>
inline Matrica<TipElemenata> operator +(Matrica<TipElemenata> prva, const Matrica<TipElemenata> &druga) { return prva += druga; }

template <typename TipElemenata>
Matrica<TipElemenata> &Matrica<TipElemenata>::operator -=(const Matrica<TipElemenata> &matrica) {
    if (this->broj_redova != matrica.broj_redova || this->broj_kolona != matrica.broj_kolona)
        throw std::domain_error("Matrice nemaju jednake dimenzije!");

    for (int i = 0; i < this->broj_redova; i++)
        for (int j = 0; j < this->broj_kolona; j++)
            this->elementi[i][j] -= matrica.elementi[i][j];

    return *this;
}

template <typename TipElemenata>
inline Matrica<TipElemenata> operator -(Matrica<TipElemenata> prva, const Matrica<TipElemenata> &druga) { return prva -= druga; }

template <typename TipElemenata>
Matrica<TipElemenata> &Matrica<TipElemenata>::operator *=(const Matrica<TipElemenata> &matrica) {
    if (this->broj_kolona != matrica.broj_redova)
        throw std::domain_error("Matrice nisu saglasne za mnozenje");

    auto proizvod = AlocirajMemoriju(this->broj_redova, matrica.broj_kolona);
    for (int i = 0; i < this->broj_redova; i++)
        for (int j = 0; j < matrica.broj_kolona; j++)
            for (int k = 0; k < this->broj_kolona; k++)
                proizvod[i][j] += this->elementi[i][k] * matrica.elementi[k][j];

    DealocirajMemoriju(this->elementi, this->broj_redova);
    this->elementi = proizvod;
    this->broj_kolona = matrica.broj_kolona;

    return *this;
}

template <typename TipElemenata> // trebala bi zasebna implementacija, ovako je jedno alociranje i kopiranje viska
inline Matrica<TipElemenata> operator *(Matrica<TipElemenata> prva, const Matrica<TipElemenata> &druga) { return prva *= druga; }


template <typename TipElemenata>
Matrica<TipElemenata> &Matrica<TipElemenata>::operator *=(double skalar) {
    for (int i = 0; i < this->broj_redova; i++)
        for (int j = 0; j < this->broj_kolona; j++)
            this->elementi[i][j] *= skalar;

    return *this;
}

template <typename TipElemenata>
inline Matrica<TipElemenata> operator *(Matrica<TipElemenata> matrica, double skalar) { return matrica *= skalar; }

template <typename TipElemenata>
inline Matrica<TipElemenata> operator *(double skalar, Matrica<TipElemenata> matrica) { return matrica *= skalar; }


int main() {
    try {
        Matrica<int> A(2, 2);
        std::cin >> A;

        A.SacuvajUTekstualnuDatoteku("testna_tekstualna.txt");
        A.SacuvajUBinarnuDatoteku("testna_binarna.dat");

        Matrica<int> B("testna_binarna.dat", true);
        std::cout << std::setw(4) << B;

        Matrica<int> C(3, 1);
        std::cout << C;

        C.ObnoviIzTekstualneDatoteke("testna_tekstualna.txt");
        std::cout << std::setw(4) << C;

    } catch (std::exception &izuzetak) {
        std::cout << izuzetak.what();
    }

    return 0;
}