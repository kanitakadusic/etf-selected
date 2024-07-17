#include <iostream>
#include <tuple>
#include <stdexcept>
#include <string>
#include <cmath>
#include <iomanip>


class Sinusoida {
    double amplituda, frekvencija, faza;

    static constexpr double PI = 3.141592653589793;
    static constexpr double EPSILON = 1e-10;

    static bool SlabaJednakost(double x, double y) {
        return std::fabs(x - y) <= Sinusoida::EPSILON * (std::fabs(x) + std::fabs(y));
    }

public:
    Sinusoida(double amplituda, double frekvencija, double faza) {
        PostaviParametre({amplituda, frekvencija, faza});
    }


    double DajAmplitudu() const { // !
        return Sinusoida(this->amplituda, this->frekvencija, this->faza).amplituda;
    }
    double DajFrekvenciju() const { // !
        return Sinusoida(this->amplituda, this->frekvencija, this->faza).frekvencija;
    }
    double DajFazu() const { // !
        return Sinusoida(this->amplituda, this->frekvencija, this->faza).faza;
    }

    std::tuple<double, double, double> DajParametre() const {
        return {DajAmplitudu(), DajFrekvenciju(), DajFazu()};
    }

    Sinusoida& PostaviAmplitudu(double); // *
    Sinusoida& PostaviFrekvenciju(double); // *
    Sinusoida& PostaviFazu(double); // *

    Sinusoida& PostaviParametre(const std::tuple<double, double, double>&);


    friend Sinusoida operator -(Sinusoida sinusoida) {
        return sinusoida.PostaviFazu(sinusoida.DajFazu() + Sinusoida::PI);
    }

    Sinusoida& operator +=(const Sinusoida&);
    friend Sinusoida operator +(const Sinusoida& prva, const Sinusoida& druga) {
        Sinusoida zbir(prva); zbir += druga; return zbir;
    }

    Sinusoida& operator -=(const Sinusoida& oduzmi) { return *this += -oduzmi; }
    friend Sinusoida operator -(const Sinusoida& prva, const Sinusoida& druga) {
        Sinusoida razlika(prva); razlika -= druga; return razlika;
    }

    Sinusoida& operator *=(double broj) {
        PostaviAmplitudu(DajAmplitudu() * broj); return *this;
    }
    friend Sinusoida operator *(const Sinusoida& sinusoida, double broj) {
        Sinusoida proizvod(sinusoida); proizvod *= broj; return proizvod;
    }
    friend Sinusoida operator *(double broj, const Sinusoida& sinusoida) {
        Sinusoida proizvod(sinusoida); proizvod *= broj; return proizvod;
    }

    Sinusoida& operator /=(double broj) {
        PostaviAmplitudu(DajAmplitudu() / broj); return *this;
    }
    friend Sinusoida operator /(const Sinusoida& sinusoida, double broj) {
        Sinusoida kolicnik(sinusoida); kolicnik /= broj; return kolicnik;
    }


    double& operator [](const std::string&); // vracanjem reference privremeno zaobilazimo normalizaciju !
    double operator [](const std::string& atribut) const {
        return Sinusoida(*this)[atribut];
    }

    double operator ()(double vrijeme) const {
        return DajAmplitudu() * std::sin(DajFrekvenciju() * vrijeme + DajFazu());
    }
};


Sinusoida& Sinusoida::PostaviAmplitudu(double amplituda) {
    this->amplituda = std::fabs(amplituda);
    if (amplituda < 0) PostaviFazu(this->faza + Sinusoida::PI);

    return *this;
}

Sinusoida& Sinusoida::PostaviFrekvenciju(double frekvencija) {
    this->frekvencija = std::fabs(frekvencija);
    if (frekvencija < 0) PostaviFazu(-this->faza + Sinusoida::PI);

    return *this;
}

Sinusoida& Sinusoida::PostaviFazu(double faza) { // [-pi,pi]
    this->faza = faza;

    while (this->faza < -Sinusoida::PI) this->faza += 2 * Sinusoida::PI;
    while (this->faza > Sinusoida::PI) this->faza -= 2 * Sinusoida::PI;

    return *this;
}

Sinusoida& Sinusoida::PostaviParametre(const std::tuple<double, double, double> &parametri) {
    PostaviFazu(std::get<2>(parametri));
    PostaviFrekvenciju(std::get<1>(parametri));
    PostaviAmplitudu(std::get<0>(parametri));

    return *this;
}


Sinusoida& Sinusoida::operator +=(const Sinusoida& dodaj) {
    if (!SlabaJednakost(this->DajFrekvenciju(), dodaj.DajFrekvenciju()))
        throw std::domain_error("Razlicite frekvencije");

    double amplituda = std::sqrt(this->DajAmplitudu() * this->DajAmplitudu() +
                                 dodaj.DajAmplitudu() * dodaj.DajAmplitudu() +
                                 2 * this->DajAmplitudu() * dodaj.DajAmplitudu() * std::cos(this->DajFazu() - dodaj.DajFazu()));

    double faza = std::atan2(this->DajAmplitudu() * std::sin(this->DajFazu()) + dodaj.DajAmplitudu() * std::sin(dodaj.DajFazu()),
                             this->DajAmplitudu() * std::cos(this->DajFazu()) + dodaj.DajAmplitudu() * std::cos(dodaj.DajFazu()));

    PostaviParametre({amplituda, this->DajFrekvenciju(), faza});
    return *this;
}


double& Sinusoida::operator[](const std::string& atribut) {
    PostaviParametre(this->DajParametre()); // !

    if (atribut == "A") return this->amplituda;
    else if (atribut == "omega" || atribut == "w") return this->frekvencija;
    else if (atribut == "phi" || atribut == "fi") return this->faza;

    throw std::domain_error("Neispravan naziv parametra");
}


int main() {
    void IspisiSinusoidu(const Sinusoida& sinusoida);

    Sinusoida sinusoida(1, 2 * std::atan(1), 4 * std::atan(1));
    IspisiSinusoidu(sinusoida);
    std::cout << " (1) = " << sinusoida(1) << std::endl;

    sinusoida["A"] = -2; // pokusaj zaobilazenja normalizacije
    sinusoida["omega"] = -5 * std::atan(1);
    sinusoida["fi"] = std::atan(1);

    IspisiSinusoidu(sinusoida);
    std::cout << " (1) = " << sinusoida(1) << std::endl;

    sinusoida -= (sinusoida += -3 * sinusoida) / 4;
    IspisiSinusoidu(-sinusoida);
    std::cout << " (1) = " << -sinusoida(1);

    return 0;
}

void IspisiSinusoidu(const Sinusoida& sinusoida) {
    std::cout << std::fixed
              << std::setprecision(0) << sinusoida.DajAmplitudu() << " sin("
              << std::setprecision(2) << sinusoida.DajFrekvenciju() << " t"
              << (sinusoida.DajFazu() > 0 ? " + " : " - ")
              << std::setprecision(2) << std::fabs(sinusoida.DajFazu()) << ")";

    std::cout.unsetf(std::ios::floatfield);
}