#include <iostream>
#include <stdexcept>
#include <cmath>
#include <algorithm>

// ______________________________________________________________________ KRUG

class Krug {
    std::pair<double, double> centar;
    double poluprecnik;


    static const double PI, EPSILON;
    static bool SlabaJednakost(double x, double y) { return std::fabs(x - y) <= Krug::EPSILON * (std::fabs(x) + std::fabs(y)); }

public:
    explicit Krug(double poluprecnik = 0) { PostaviCentar({0, 0}); PostaviPoluprecnik(poluprecnik); }
    explicit Krug(const std::pair<double, double> &centar, double poluprecnik = 0) { PostaviCentar(centar); PostaviPoluprecnik(poluprecnik); }


    Krug &PostaviCentar(const std::pair<double, double> &centar) { this->centar = centar; return *this; }
    Krug &PostaviPoluprecnik(double poluprecnik);

    Krug &Transliraj(double delta_x, double delta_y);
    Krug &Rotiraj(double alpha) { Rotiraj({0, 0}, alpha); return *this; }
    Krug &Rotiraj(const std::pair<double, double> &centar_rotacije, double alpha);


    void Ispisi() const { std::cout << "{(" << DajCentar().first << "," << DajCentar().second << ")," << DajPoluprecnik() << "}"; }

    std::pair<double, double> DajCentar() const { return this->centar; }
    double DajPoluprecnik() const { return this->poluprecnik; }
    double DajObim() const { return 2 * DajPoluprecnik() * Krug::PI; }
    double DajPovrsinu() const { return DajPoluprecnik() * DajPoluprecnik() * Krug::PI; }


    static double RastojanjeCentara(const Krug &prvi, const Krug &drugi);
    bool DaLiSadrzi(const Krug &krug) const;

    static bool DaLiSuIdenticni(const Krug &prvi, const Krug &drugi);
    static bool DaLiSuPodudarni(const Krug &prvi, const Krug &drugi);
    static bool DaLiSuKoncentricni(const Krug &prvi, const Krug &drugi);
    static bool DaLiSeDodirujuIzvani(const Krug &prvi, const Krug &drugi);
    static bool DaLiSeDodirujuIznutri(const Krug &prvi, const Krug &drugi);
    static bool DaLiSePreklapaju(const Krug &prvi, const Krug &drugi);
    static bool DaLiSeSijeku(const Krug &prvi, const Krug &drugi);


    friend Krug TransliraniKrug(Krug krug, double delta_x, double delta_y) { return krug.Transliraj(delta_x, delta_y); }
    friend Krug RotiraniKrug(Krug krug, double alpha) { return krug.Rotiraj(alpha); }
    friend Krug RotiraniKrug(Krug krug, const std::pair<double, double> &centar_rotacije, double alpha) { return krug.Rotiraj(centar_rotacije, alpha); }
};

// --------------------

const double Krug::PI = 4 * std::atan(1);
const double Krug::EPSILON = 1e-10;

// --------------------

Krug &Krug::PostaviPoluprecnik(double poluprecnik) {
    if (poluprecnik < 0) throw std::domain_error("Nedozvoljen poluprecnik");
    this->poluprecnik = poluprecnik;
    return *this;
}


Krug &Krug::Transliraj(double delta_x, double delta_y) {
    PostaviCentar({DajCentar().first + delta_x, DajCentar().second + delta_y});
    return *this;
}

Krug &Krug::Rotiraj(const std::pair<double, double> &centar_rotacije, double alpha) {
    double x = DajCentar().first - centar_rotacije.first, y = DajCentar().second - centar_rotacije.second;
    double cosinus = std::cos(alpha), sinus = std::sin(alpha);

    PostaviCentar({centar_rotacije.first + x * cosinus - y * sinus, centar_rotacije.second + x * sinus + y * cosinus});
    return *this;
}


double Krug::RastojanjeCentara(const Krug &prvi, const Krug &drugi) {
    double x = prvi.DajCentar().first - drugi.DajCentar().first, y = prvi.DajCentar().second - drugi.DajCentar().second;
    return std::sqrt(x * x + y * y);
}

bool Krug::DaLiSadrzi(const Krug &krug) const {
    return RastojanjeCentara(*this, krug) < this->DajPoluprecnik() - krug.DajPoluprecnik() ||
           Krug::SlabaJednakost(RastojanjeCentara(*this, krug), this->DajPoluprecnik() - krug.DajPoluprecnik());
}

bool Krug::DaLiSuIdenticni(const Krug &prvi, const Krug &drugi) {
    return DaLiSuPodudarni(prvi, drugi) &&
           DaLiSuKoncentricni(prvi, drugi);
}

bool Krug::DaLiSuPodudarni(const Krug &prvi, const Krug &drugi) {
    return Krug::SlabaJednakost(prvi.DajPoluprecnik(), drugi.DajPoluprecnik());
}

bool Krug::DaLiSuKoncentricni(const Krug &prvi, const Krug &drugi) {
    return Krug::SlabaJednakost(prvi.DajCentar().first, drugi.DajCentar().first) &&
           Krug::SlabaJednakost(prvi.DajCentar().second, drugi.DajCentar().second);
}

bool Krug::DaLiSeDodirujuIzvani(const Krug &prvi, const Krug &drugi) {
    return Krug::SlabaJednakost(RastojanjeCentara(prvi, drugi), prvi.DajPoluprecnik() + drugi.DajPoluprecnik());
}

bool Krug::DaLiSeDodirujuIznutri(const Krug &prvi, const Krug &drugi) {
    return !DaLiSuPodudarni(prvi, drugi) &&
           Krug::SlabaJednakost(RastojanjeCentara(prvi, drugi), std::fabs(prvi.DajPoluprecnik() - drugi.DajPoluprecnik()));
}

bool Krug::DaLiSePreklapaju(const Krug &prvi, const Krug &drugi) {
    return RastojanjeCentara(prvi, drugi) < prvi.DajPoluprecnik() + drugi.DajPoluprecnik();
}

bool Krug::DaLiSeSijeku(const Krug &prvi, const Krug &drugi) {
    double rastojanje = RastojanjeCentara(prvi, drugi);
    return std::fabs(prvi.DajPoluprecnik() - drugi.DajPoluprecnik()) < rastojanje &&
           rastojanje < prvi.DajPoluprecnik() + drugi.DajPoluprecnik();
}

// ______________________________________________________________________ MAIN

int main() {
    int broj_krugova;
    std::cout << "Unesite broj krugova: ";
    std::cin >> broj_krugova;
    if (broj_krugova < 1) { std::cout << "Uneseni broj nije prirodan!"; return 0; }

    try {
        Krug **krugovi = new Krug*[broj_krugova]{};

        for (int i = 0; i < broj_krugova; i++) {
            try {
                double x, y;
                std::cout << "Unesite centar " << i + 1 << ". kruga: ";
                std::cin >> x >> y;
                if (!std::cin) throw std::domain_error("Neispravne koordinate centra");

                double poluprecnik;
                std::cout << "Unesite poluprecnik " << i + 1 << ". kruga: ";
                std::cin >> poluprecnik;
                if (!std::cin || poluprecnik < 0) throw std::domain_error("Neispravan poluprecnik");

                krugovi[i] = new Krug({x, y}, poluprecnik);
            } catch (std::domain_error &izuzetak) {
                std::cout << izuzetak.what() << "! Pokusajte ponovo:\n\n"; i--;
                std::cin.clear(); std::cin.ignore(10000, '\n');
            }
        }

        double delta_x, delta_y;
        std::cout << "Unesite parametre translacije (delta_x,delta_y): ";
        std::cin >> delta_x >> delta_y;

        double alpha;
        std::cout << "Unesite ugao rotacije oko tacke (0,0) u stepenima: ";
        std::cin >> alpha;

        std::transform(krugovi, krugovi + broj_krugova, krugovi, [delta_x, delta_y, alpha](Krug *const krug) { krug->Transliraj(delta_x, delta_y).Rotiraj(alpha * std::atan(1) / 45); return krug; });
        std::sort(krugovi, krugovi + broj_krugova, [](const Krug *prvi, const Krug *drugi) { return prvi->DajPovrsinu() < drugi->DajPovrsinu(); });

        std::cout << "Parametri krugova nakon obavljene transformacije su:\n";
        std::for_each(krugovi, krugovi + broj_krugova, [](const Krug *krug) { krug->Ispisi(); std::cout << std::endl; });

        std::cout << "Krug sa najvecim obimom je: ";
        (*std::max_element(krugovi, krugovi + broj_krugova, [](const Krug *prvi, const Krug *drugi) { return prvi->DajObim() < drugi->DajObim(); }))->Ispisi();

        int broj_trenutnog_kruga = 0;
        bool sijeku_se = false;

        std::for_each(krugovi, krugovi + broj_krugova, [krugovi, broj_krugova, &broj_trenutnog_kruga, &sijeku_se](const Krug *i) {
            std::for_each(krugovi + ++broj_trenutnog_kruga, krugovi + broj_krugova, [i, &sijeku_se](const Krug *j) {
                if (Krug::DaLiSeSijeku(*i, *j)) {
                    sijeku_se = true;

                    std::cout << "\nPresjecaju se krugovi:\n";
                    i->Ispisi(); std::cout << " i "; j->Ispisi();
                }
            });
        });

        if (!sijeku_se && broj_krugova != 1) std::cout << "\nNe postoje krugovi koji se presjecaju!";

        for (int i = 0; i < broj_krugova; i++) delete krugovi[i];
        delete[] krugovi;
    } catch (...) {
        std::cout << "Problemi sa alokacijom memorije!";
    }

    return 0;
}