#include <iostream>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <memory>
#include <algorithm>
#include <iomanip>

// _____________________________________________________________________ NEPREKLAPAJUCI KRUG

class NepreklapajuciKrug {
    std::pair<double, double> centar;
    double poluprecnik;

    NepreklapajuciKrug *prethodnik;
    static NepreklapajuciKrug *posljednji;

    void ProvjeriPreklapanja();


    static const double PI, EPSILON;

    static bool SlabaJednakost(double x, double y);
    static bool DaLiSeDodirujuIzvani(const NepreklapajuciKrug &prvi, const NepreklapajuciKrug &drugi);
    static bool DaLiSePreklapaju(const NepreklapajuciKrug &prvi, const NepreklapajuciKrug &drugi);

public:
    explicit NepreklapajuciKrug(double poluprecnik = 0);
    explicit NepreklapajuciKrug(const std::pair<double, double> &centar, double poluprecnik = 0);

    NepreklapajuciKrug(const NepreklapajuciKrug &krug) = delete;
    NepreklapajuciKrug &operator =(const NepreklapajuciKrug &krug) = delete;

    ~NepreklapajuciKrug();


    NepreklapajuciKrug &PostaviCentar(const std::pair<double, double> &centar);
    NepreklapajuciKrug &PostaviPoluprecnik(double poluprecnik);

    NepreklapajuciKrug &Transliraj(double delta_x, double delta_y);
    NepreklapajuciKrug &Rotiraj(double alpha) { Rotiraj({0, 0}, alpha); return *this; }
    NepreklapajuciKrug &Rotiraj(const std::pair<double, double> &centar_rotacije, double alpha);

    void Ispisi() const { std::cout << "{(" << DajCentar().first << "," << DajCentar().second << ")," << DajPoluprecnik() << "}\n"; }

    std::pair<double, double> DajCentar() const { return this->centar; }
    double DajPoluprecnik() const { return this->poluprecnik; }
    double DajObim() const { return 2 * DajPoluprecnik() * NepreklapajuciKrug::PI; }
    double DajPovrsinu() const { return DajPoluprecnik() * DajPoluprecnik() * NepreklapajuciKrug::PI; }

    static double RastojanjeCentara(const NepreklapajuciKrug &prvi, const NepreklapajuciKrug &drugi);
};

// --------------------

NepreklapajuciKrug *NepreklapajuciKrug::posljednji = nullptr;

void NepreklapajuciKrug::ProvjeriPreklapanja() {
    for (auto p_krug = NepreklapajuciKrug::posljednji; p_krug != nullptr; p_krug = p_krug->prethodnik)
        if (p_krug != this && DaLiSePreklapaju(*this, *p_krug))
            throw std::logic_error("Nedozvoljeno preklapanje");
}


const double NepreklapajuciKrug::PI = 4 * std::atan(1);
const double NepreklapajuciKrug::EPSILON = 10e-10;

bool NepreklapajuciKrug::SlabaJednakost(double x, double y) {
    return std::fabs(x - y) <= NepreklapajuciKrug::EPSILON * (std::fabs(x) + std::fabs(y));
}

bool NepreklapajuciKrug::DaLiSeDodirujuIzvani(const NepreklapajuciKrug &prvi, const NepreklapajuciKrug &drugi) {
    return NepreklapajuciKrug::SlabaJednakost(RastojanjeCentara(prvi, drugi), prvi.DajPoluprecnik() + drugi.DajPoluprecnik());
}

bool NepreklapajuciKrug::DaLiSePreklapaju(const NepreklapajuciKrug &prvi, const NepreklapajuciKrug &drugi) {
    return RastojanjeCentara(prvi, drugi) < prvi.DajPoluprecnik() + drugi.DajPoluprecnik() || DaLiSeDodirujuIzvani(prvi, drugi);
}

// --------------------

NepreklapajuciKrug::NepreklapajuciKrug(double poluprecnik): centar({0, 0}), prethodnik(posljednji) {
    PostaviPoluprecnik(poluprecnik);
    NepreklapajuciKrug::posljednji = this;
}

NepreklapajuciKrug::NepreklapajuciKrug(const std::pair<double, double> &centar, double poluprecnik): centar(centar), prethodnik(posljednji) {
    PostaviPoluprecnik(poluprecnik);
    NepreklapajuciKrug::posljednji = this;
}

NepreklapajuciKrug::~NepreklapajuciKrug() {
    NepreklapajuciKrug *sljedbenik = nullptr;
    for (auto p_krug = NepreklapajuciKrug::posljednji; p_krug != this; p_krug = p_krug->prethodnik)
        sljedbenik = p_krug;

    if (sljedbenik == nullptr) NepreklapajuciKrug::posljednji = this->prethodnik;
    else sljedbenik->prethodnik = this->prethodnik;
}

// --------------------

NepreklapajuciKrug &NepreklapajuciKrug::PostaviCentar(const std::pair<double, double> &centar) {
    std::pair<double, double> stari_centar(DajCentar());
    this->centar = centar;

    try { ProvjeriPreklapanja(); }
    catch (...) { this->centar = stari_centar; throw; }

    return *this;
}

NepreklapajuciKrug &NepreklapajuciKrug::PostaviPoluprecnik(double poluprecnik) {
    if (poluprecnik < 0) throw std::domain_error("Nedozvoljen poluprecnik");

    double stari_poluprecnik(DajPoluprecnik());
    this->poluprecnik = poluprecnik;

    try { ProvjeriPreklapanja(); }
    catch (...) { this->poluprecnik = stari_poluprecnik; throw; }

    return *this;
}


NepreklapajuciKrug &NepreklapajuciKrug::Transliraj(double delta_x, double delta_y) {
    PostaviCentar({DajCentar().first + delta_x, DajCentar().second + delta_y});
    return *this;
}

NepreklapajuciKrug &NepreklapajuciKrug::Rotiraj(const std::pair<double, double> &centar_rotacije, double alpha) {
    double x = DajCentar().first - centar_rotacije.first, y = DajCentar().second - centar_rotacije.second;
    double cosinus = std::cos(alpha * NepreklapajuciKrug::PI / 180), sinus = std::sin(alpha * NepreklapajuciKrug::PI / 180);

    PostaviCentar({centar_rotacije.first + x * cosinus - y * sinus, centar_rotacije.second + x * sinus + y * cosinus});
    return *this;
}


double NepreklapajuciKrug::RastojanjeCentara(const NepreklapajuciKrug &prvi, const NepreklapajuciKrug &drugi) {
    double x = prvi.DajCentar().first - drugi.DajCentar().first, y = prvi.DajCentar().second - drugi.DajCentar().second;
    return std::sqrt(x * x + y * y);
}

// _____________________________________________________________________ MAIN

int main() {
    auto SrediSpremnik = []() { std::cin.clear(); std::cin.ignore(10000, '\n'); };

    int broj_krugova;
    std::cout << "Unesite broj krugova: ";

    for (;;) {
        std::cin >> broj_krugova;
        if (std::cin && broj_krugova > 0) break;

        std::cout << "Neispravan broj krugova, unesite ponovo! ";
        SrediSpremnik();
    }

    std::vector<std::shared_ptr<NepreklapajuciKrug>> krugovi;
    bool prvi_unos = true;

    for (int i = 0; i < broj_krugova; i++) {
        try {
            double x, y;
            if (prvi_unos) std::cout << "Unesite centar za " << i + 1 << ". krug: ";
            std::cin >> x >> y;

            if (!std::cin) throw std::domain_error("Neispravan centar");
            prvi_unos = true;

            do {
                try {
                    double poluprecnik;
                    if (prvi_unos) std::cout << "Unesite poluprecnik: ";
                    std::cin >> poluprecnik;

                    if (!std::cin || poluprecnik < 0) throw std::domain_error("Neispravan poluprecnik");
                    prvi_unos = true;

                    krugovi.emplace_back(std::make_shared<NepreklapajuciKrug>(std::make_pair(x, y), poluprecnik));
                } catch (std::domain_error &domain) {
                    std::cout << domain.what() << "!\nUnesite ponovo poluprecnik za " << i + 1 << ". krug: ";
                    SrediSpremnik(); prvi_unos = false;
                }
            } while (!prvi_unos);
        } catch (std::domain_error &domain) {
            std::cout << domain.what() << "!\nUnesite ponovo centar za " << i-- + 1 << ". krug: ";
            SrediSpremnik(); prvi_unos = false;
        } catch (std::exception &exception) {
            std::cout << exception.what() << std::endl;
            i--; prvi_unos = true;
        }
    }

    std::sort(krugovi.begin(), krugovi.end(), [](auto &prvi, auto &drugi) {
        double povrsina_prvog = prvi->DajPovrsinu(), povrsina_drugog = drugi->DajPovrsinu();
        return (povrsina_prvog == povrsina_drugog ? prvi->DajCentar().second > drugi->DajCentar().second : povrsina_prvog > povrsina_drugog);
    });

    std::cout << "Krugovi nakon obavljenog sortiranja:\n";
    std::for_each(krugovi.begin(), krugovi.end(), [](auto &krug) { krug->Ispisi(); });

    return 0;
}