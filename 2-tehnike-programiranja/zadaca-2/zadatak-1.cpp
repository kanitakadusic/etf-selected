#include <iostream>
#include <vector>
#include <string>

constexpr int X_POLUOSA = 30, Y_POLUOSA = 10;
std::vector<std::vector<char>> g_teren(Y_POLUOSA * 2 + 1, std::vector<char>(X_POLUOSA * 2 + 1, ' '));
bool g_aktivan = true;

enum class Komande {Idi, Rotiraj, Sakrij, Otkrij, PrikaziTeren, Kraj};
enum class KodoviGresaka {PogresnaKomanda, NedostajeParametar, SuvisanParametar, NeispravanParametar};
enum class Pravci {Sjever, Sjeveroistok, Istok, Jugoistok, Jug, Jugozapad, Zapad, Sjeverozapad};

bool UnosKomande(Komande &, int &, KodoviGresaka &);

void Rotiraj(Pravci &orijentacija, int ugao) {
    ugao = (int(orijentacija) - ugao) % 8;
    orijentacija = Pravci((ugao < 0) ? ugao + 8 : ugao);
}

void Idi(int &x, int &y, Pravci orijentacija, int korak) {
    std::vector<std::vector<short int>> kretanje{{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};
    if (korak < 0) Rotiraj(orijentacija, 4), korak = -korak;

    g_teren.at(y + Y_POLUOSA).at(x + X_POLUOSA) = (g_teren.at(y + Y_POLUOSA).at(x + X_POLUOSA) == 'T') ? ' ' : '*';

    while (korak--) {
        if (g_aktivan) g_teren.at(y + Y_POLUOSA).at(x + X_POLUOSA) = '*';
        x += kretanje.at(int(orijentacija)).at(0); y += kretanje.at(int(orijentacija)).at(1);

        if (x < -X_POLUOSA) x = X_POLUOSA; else if (x > X_POLUOSA) x = -X_POLUOSA;
        if (y < -Y_POLUOSA) y = Y_POLUOSA; else if (y > Y_POLUOSA) y = -Y_POLUOSA;
    }

    g_teren.at(y + Y_POLUOSA).at(x + X_POLUOSA) = (!g_aktivan && g_teren.at(y + Y_POLUOSA).at(x + X_POLUOSA) == ' ') ? 'T' : 'O';
}

void PostaviVidljivost(bool vidljiv) {
    g_aktivan = vidljiv;
}

void IspisiPoziciju(int x, int y, Pravci orijentacija) {
    std::vector<const char *> strane{"sjever", "sjeveroistok", "istok", "jugoistok", "jug", "jugozapad", "zapad", "sjeverozapad"};
    std::cout << "Robot je " << ((g_aktivan) ? "vidljiv" : "nevidljiv") << ", nalazi se na poziciji (" << x << "," << y << ") i gleda na " << strane.at(int(orijentacija)) << "." << std::endl;
}

void PrikaziTeren() {
    std::cout << std::string(X_POLUOSA * 2 + 3, '#') << std::endl;

    for (int i = Y_POLUOSA * 2; i >= 0; i--) {
        std::cout << "#";

        for (int j = 0; j <= X_POLUOSA * 2; j++)
            std::cout << ((g_teren.at(i).at(j) == 'T') ? 'O' : g_teren.at(i).at(j));

        std::cout << "#" << std::endl;
    }

    std::cout << std::string(X_POLUOSA * 2 + 3, '#') << std::endl;
}

void IzvrsiKomandu(Komande komanda, int parametar, int &x, int &y, Pravci &orijentacija) {
    switch (komanda) {
        case Komande::Idi: Idi(x, y, orijentacija, parametar); break;
        case Komande::Rotiraj: Rotiraj(orijentacija, parametar); break;
        case Komande::Sakrij: PostaviVidljivost(false); break;
        case Komande::Otkrij: g_teren.at(y + Y_POLUOSA).at(x + X_POLUOSA) = '*'; PostaviVidljivost(true); break;
        case Komande::PrikaziTeren: PrikaziTeren(); break;
        case Komande::Kraj: break;
    }
}

void PrijaviGresku(KodoviGresaka kod_greske) {
    switch (kod_greske) {
        case KodoviGresaka::PogresnaKomanda: std::cout << "Nerazumljiva komanda!\n"; break;
        case KodoviGresaka::NedostajeParametar: std::cout << "Komanda trazi parametar koji nije naveden!\n"; break;
        case KodoviGresaka::NeispravanParametar: std::cout << "Parametar komande nije ispravan!\n"; break;
        case KodoviGresaka::SuvisanParametar: std::cout << "Zadan je suvisan parametar nakon komande!\n"; break;
    }
}

int main() {
    Komande komanda;
    KodoviGresaka kod_greske;
    Pravci orijentacija = Pravci::Sjever;
    int parametar, x = 0, y = 0;

    g_teren.at(y + Y_POLUOSA).at(x + X_POLUOSA) = 'O';
    IspisiPoziciju(x, y, orijentacija);

    for (;;) {
        std::cout << "Unesi komandu: ";
        if (UnosKomande(komanda, parametar, kod_greske)) IzvrsiKomandu(komanda, parametar, x, y, orijentacija);
        else { PrijaviGresku(kod_greske); continue; }

        if (komanda == Komande::Kraj) { std::cout << "Dovidjenja!"; break; }
        if (komanda != Komande::PrikaziTeren) IspisiPoziciju(x, y, orijentacija);
    }

    return 0;
}

void SrediSpremnik() {
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}

bool UnosKomande(Komande &komanda, int &parametar, KodoviGresaka &kod_greske) {
    char slovo; std::cin >> slovo;

    if (slovo == 'I' || slovo == 'R') {
        while (std::cin.peek() == ' ') std::cin.get();
        if (std::cin.peek() == '\n') { kod_greske = KodoviGresaka::NedostajeParametar; SrediSpremnik(); return false; }

        int koraci_ugao = 0;
        std::cin >> koraci_ugao;
        if (!std::cin || (std::cin.peek() != '\n' && std::cin.peek() != ' ')) { kod_greske = KodoviGresaka::NeispravanParametar; SrediSpremnik(); return false; }
        else if (std::cin.peek() != '\n') { kod_greske = KodoviGresaka::SuvisanParametar; SrediSpremnik(); return false; }

        parametar = koraci_ugao;
        komanda = (slovo == 'I') ? Komande::Idi : Komande::Rotiraj;
    } else if (slovo == 'S') {
        if (slovo = char(std::cin.get()), slovo != '+' && slovo != '-') { kod_greske = KodoviGresaka::PogresnaKomanda; SrediSpremnik(); return false; }
        if (std::cin.peek() != '\n') { kod_greske = KodoviGresaka::SuvisanParametar; SrediSpremnik(); return false; }

        komanda = (slovo == '+') ? Komande::Sakrij : Komande::Otkrij;
    } else if (slovo == 'T' || slovo == 'K') {
        while (std::cin.peek() == ' ') std::cin.get();
        if (std::cin.peek() != '\n') { kod_greske = KodoviGresaka::SuvisanParametar; SrediSpremnik(); return false; }

        komanda = (slovo == 'T') ? Komande::PrikaziTeren : Komande::Kraj;
    } else {
        kod_greske = KodoviGresaka::PogresnaKomanda; SrediSpremnik(); return false;
    }

    SrediSpremnik(); return true;
}