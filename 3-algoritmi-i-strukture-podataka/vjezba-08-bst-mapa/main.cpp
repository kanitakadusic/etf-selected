// ====================================================================================================
// NAPOMENA: Za pokretanje testnih funkcija ukloniti komentare u main() funkciji
// ====================================================================================================

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <functional>
#include <iomanip>
#include <vector>

using std::cout;
using std::endl;

// ==================================================================================================== BEGIN - MAPA

template <typename Key, typename Value>
class Mapa {
public:
    Mapa() = default;

    Mapa(const Mapa&) = default;
    Mapa& operator =(const Mapa&) = default;

    Mapa(Mapa&&) = default;
    Mapa& operator =(Mapa&&) = default;

    virtual ~Mapa() = default;

    // --------------------

    virtual int brojElemenata() const = 0;

    virtual void obrisi() = 0;
    virtual void obrisi(const Key&) = 0;

    virtual Value& operator [](const Key&) = 0;
    virtual Value operator [](const Key&) const = 0;
};

// ==================================================================================================== END - MAPA

// ==================================================================================================== BEGIN - NIZ MAPA

template <typename Key, typename Value>
class NizMapa: public Mapa<Key, Value> {
    int capacity, size;
    std::pair<Key, Value>** array;

    // --------------------

    static void deallocate(std::pair<Key, Value>**, int);
    std::pair<Key, Value>* add(const Key&);

    // --------------------

public:
    NizMapa(int capacity = 2);

    NizMapa(const NizMapa&);
    NizMapa& operator =(const NizMapa&);

    NizMapa(NizMapa&&);
    NizMapa& operator =(NizMapa&&);

    ~NizMapa() { deallocate(this->array, this->size); }

    // --------------------

    int brojElemenata() const override { return this->size; }

    void obrisi() override;
    void obrisi(const Key&) override;

    Value& operator [](const Key&) override;
    Value operator [](const Key&) const override;
};

// ------------------------------------------------------- BEGIN - UPRAVLJACKI DIO KLASE

template <typename Key, typename Value>
NizMapa<Key, Value>::NizMapa(int capacity): size(0) {
    if (capacity < 0) throw std::logic_error("Neispravan kapacitet");

    this->capacity = capacity < 2 ? 2 : capacity;
    this->array = new std::pair<Key, Value>*[this->capacity]{};
}

template <typename Key, typename Value>
NizMapa<Key, Value>::NizMapa(const NizMapa& map): capacity(map.capacity), size(map.size), array(new std::pair<Key, Value>*[map.capacity]{}) {
    for (int i = 0; i < map.size; i++)
        try {
            this->array[i] = new std::pair<Key, Value>{*map.array[i]};
        } catch (...) {
            deallocate(this->array, i); throw;
        }
}

template <typename Key, typename Value>
NizMapa<Key, Value>& NizMapa<Key, Value>::operator =(const NizMapa& map) {
    if (map.capacity > this->capacity) {
        auto newArray = new std::pair<Key, Value>*[map.capacity]{};

        for (int i = 0; i < map.size; i++)
            try {
                newArray[i] = new std::pair<Key, Value>{*map.array[i]};
            } catch (...) {
                deallocate(newArray, i); throw;
            }

        deallocate(this->array, this->size);

        this->capacity = map.capacity;
        this->array = newArray;
    } else if (&map != this) {
        this->obrisi();

        for (int i = 0; i < map.size; i++)
            try {
                this->array[i] = new std::pair<Key, Value>{*map.array[i]};
            } catch (...) {
                deallocate(this->array, i); throw;
            }
    }

    this->size = map.size;
    return *this;
}

template <typename Key, typename Value>
NizMapa<Key, Value>::NizMapa(NizMapa&& map): capacity(map.capacity), size(map.size), array(map.array) {
    map.size = 0; map.array = nullptr;
}

template <typename Key, typename Value>
NizMapa<Key, Value>& NizMapa<Key, Value>::operator =(NizMapa&& map) {
    std::swap(this->capacity, map.capacity);
    std::swap(this->size, map.size);
    std::swap(this->array, map.array);

    return *this;
}

// ------------------------------------------------------- END - UPRAVLJACKI DIO KLASE

// ------------------------------------------------------- BEGIN - POMOCNE NIZ METODE

template <typename Key, typename Value>
void NizMapa<Key, Value>::deallocate(std::pair<Key, Value>** array, int size) {
    for (int i = 0; i < size; i++) delete array[i];
    delete[] array;
}

template <typename Key, typename Value>
std::pair<Key, Value>* NizMapa<Key, Value>::add(const Key& key) {
    if (this->size == this->capacity) {
        auto newArray = new std::pair<Key, Value>*[this->capacity *= 2]{};

        for (int i = 0; i < this->size; i++) {
            newArray[i] = this->array[i];
            this->array[i] = nullptr;
        }

        delete[] this->array;
        this->array = newArray;
    }

    this->array[this->size] = new std::pair<Key, Value>{key, Value()};
    return this->array[this->size++];
}

// ------------------------------------------------------- END - POMOCNE NIZ METODE

// ------------------------------------------------------- BEGIN - IMPLEMENTACIJA INTERFEJSA

template <typename Key, typename Value>
void NizMapa<Key, Value>::obrisi() {
    std::for_each(this->array, this->array + this->size, [](std::pair<Key, Value>*& element) { delete element; element = nullptr; });
    this->size = 0;
}

template <typename Key, typename Value>
void NizMapa<Key, Value>::obrisi(const Key& key) {
    auto p = std::find_if(this->array, this->array + this->size, [&key](std::pair<Key, Value>* pair) { return pair->first == key; });

    if (p == this->array + this->size) throw std::logic_error("Nepostojeci kljuc");
    int index = p - this->array;

    delete this->array[index];
    std::copy(this->array + index + 1, this->array + this->size, this->array + index);

    this->size--;
}

template <typename Key, typename Value>
Value& NizMapa<Key, Value>::operator [](const Key& key) {
    auto p = std::find_if(this->array, this->array + this->size, [&key](std::pair<Key, Value>* pair) { return pair->first == key; });

    if (p == this->array + this->size) return this->add(key)->second;
    return (*p)->second;
}

template <typename Key, typename Value>
Value NizMapa<Key, Value>::operator [](const Key& key) const {
    auto p = std::find_if(this->array, this->array + this->size, [&key](std::pair<Key, Value>* pair) { return pair->first == key; });

    if (p == this->array + this->size) return Value();
    return (*p)->second;
}

// ------------------------------------------------------- END - IMPLEMENTACIJA INTERFEJSA

// ==================================================================================================== END - NIZ MAPA

// ==================================================================================================== BEGIN - BINARNO STABLO MAPA

template <typename Key, typename Value>
struct Node {
    Key key;
    Value value;

    Node *parent, *left, *right;

    Node(const Key& key, const Value& value = Value(), Node* parent = nullptr, Node* left = nullptr, Node* right = nullptr):
            key(key), value(value), parent(parent), left(left), right(right) {}
};

template <typename Key, typename Value>
class BinStabloMapa: public Mapa<Key, Value> {
    int size;
    Node<Key, Value>* root;

    // --------------------

    Node<Key, Value>* add(Node<Key, Value>*&, const Key&, Node<Key, Value>* parent = nullptr);
    bool erase(const Key&);

    static void clone(Node<Key, Value>*, Node<Key, Value>*&, Node<Key, Value>* parent = nullptr);
    static Node<Key, Value>* search(Node<Key, Value>*, const Key&);
    static void deallocate(Node<Key, Value>*);

    // --------------------

public:
    BinStabloMapa(): size(0), root(nullptr) {}

    BinStabloMapa(const BinStabloMapa& map): size(map.size), root(nullptr) { clone(map.root, this->root); }
    BinStabloMapa& operator =(const BinStabloMapa& map) {
        if (&map != this) { this->obrisi(); clone(map.root, this->root); this->size = map.size; }
        return *this;
    }

    BinStabloMapa(BinStabloMapa&& map): size(map.size), root(map.root) { map.size = 0; map.root = nullptr; }
    BinStabloMapa& operator =(BinStabloMapa&& map) {
        std::swap(this->size, map.size); std::swap(this->root, map.root);
        return *this;
    }

    ~BinStabloMapa() { deallocate(this->root); }

    // --------------------

    int brojElemenata() const override { return this->size; }

    void obrisi() override {
        deallocate(this->root);

        this->size = 0;
        this->root = nullptr;
    }

    void obrisi(const Key& key) override {
        if (!this->erase(key)) throw std::logic_error("Nepostojeci kljuc");
    }

    Value& operator [](const Key& key) override {
        return this->add(this->root, key)->value;
    }

    Value operator [](const Key& key) const override {
        Node<Key, Value>* x;
        return (x = search(this->root, key)) == nullptr ? Value() : x->value;
    }
};

// ------------------------------------------------------- BEGIN - POMOCNE BST METODE

template <typename Key, typename Value>
Node<Key, Value>* BinStabloMapa<Key, Value>::add(Node<Key, Value>*& x, const Key& key, Node<Key, Value>* parent) {
    if (x == nullptr) { this->size++; return (x = new Node<Key, Value>(key, Value(), parent)); }
    else if (key < x->key) return this->add(x->left, key, x);
    else if (key > x->key) return this->add(x->right, key, x);
    else return x;
}

template <typename Key, typename Value>
bool BinStabloMapa<Key, Value>::erase(const Key& key) {
    Node<Key, Value> *x = nullptr, *subtree = nullptr;

    if ((x = search(this->root, key)) == nullptr) return false;

    if (x->left == nullptr) subtree = x->right;
    else if (x->right == nullptr) subtree = x->left;
    else {
        Node<Key, Value>* maxLeft = subtree = x->left;

        while (maxLeft->right != nullptr) maxLeft = maxLeft->right;

        if (maxLeft != subtree) {
            maxLeft->parent->right = maxLeft->left;
            if (maxLeft->left != nullptr) maxLeft->left->parent = maxLeft->parent;
            maxLeft->left = subtree; subtree->parent = maxLeft;
        }

        maxLeft->right = x->right; maxLeft->right->parent = maxLeft;
        subtree = maxLeft;
    }

    if (x->parent == nullptr) this->root = subtree;
    else if (x == x->parent->left) x->parent->left = subtree;
    else x->parent->right = subtree;

    if (subtree != nullptr) subtree->parent = x->parent;

    delete x;
    this->size--;

    return true;
}

template <typename Key, typename Value>
void BinStabloMapa<Key, Value>::clone(Node<Key, Value>* source, Node<Key, Value>*& destination, Node<Key, Value>* parent) {
    if (source != nullptr) {
        destination = new Node<Key, Value>(source->key, source->value, parent);
        clone(source->left, destination->left, destination);
        clone(source->right, destination->right, destination);
    }
}

template <typename Key, typename Value>
Node<Key, Value>* BinStabloMapa<Key, Value>::search(Node<Key, Value>* x, const Key& key) {
    while (x != nullptr && key != x->key)
        if (key < x->key) x = x->left;
        else x = x->right;

    return x;
}

template <typename Key, typename Value>
void BinStabloMapa<Key, Value>::deallocate(Node<Key, Value>* tree) {
    if (tree != nullptr) {
        deallocate(tree->left);
        deallocate(tree->right);

        delete tree;
    }
}

// ------------------------------------------------------- END - POMOCNE BST METODE

// ==================================================================================================== END - BINARNO STABLO MAPA

void testBrojElemenata(BinStabloMapa<int, std::string>);
void testObrisiSve(BinStabloMapa<int, std::string>);
void testObrisi(BinStabloMapa<int, std::string>);
void testOperator(BinStabloMapa<int, std::string>);
void testOperatorConst(const BinStabloMapa<int, std::string>&);
void testMapa(const BinStabloMapa<int, std::string>&);

// --------------------

void testDodajPerformanse(const std::vector<int>& rands12345digits, const std::vector<int>& rands6digits) {
    NizMapa<int, int> niz;
    BinStabloMapa<int, int> stablo;

    for (int i = 0; i < rands12345digits.size(); i++)
        niz[rands12345digits[i]] = stablo[rands12345digits[i]] = rands12345digits[i];

    clock_t start = 0, end = 0;
    double nizPerformanse, stabloPerformanse;

    start = std::clock();
    for (int i = 0; i < rands6digits.size(); i++) niz[rands6digits[i]] = rands6digits[i];
    end = std::clock();
    nizPerformanse = (double(end - start) / double(CLOCKS_PER_SEC / 1e6)) / rands6digits.size();

    start = std::clock();
    for (int i = 0; i < rands6digits.size(); i++) stablo[rands6digits[i]] = rands6digits[i];
    end = std::clock();
    stabloPerformanse = (double(end - start) / double(CLOCKS_PER_SEC / 1e6)) / rands6digits.size();

    std::cout << "Prosjecno vrijeme dodavanja novog elementa u mapu [mikrosekunde]\n";
    std::cout << "- niz: " << nizPerformanse << "\n- stablo: " << stabloPerformanse << std::endl;
}

void testPristupPerformanse(const std::vector<int>& rands12345digits) {
    NizMapa<int, int> niz;
    BinStabloMapa<int, int> stablo;

    for (int i = 0; i < rands12345digits.size(); i++)
        niz[rands12345digits[i]] = stablo[rands12345digits[i]] = rands12345digits[i];

    clock_t start = 0, end = 0;
    double nizPerformanse, stabloPerformanse;

    start = std::clock();
    for (int i = 0; i < rands12345digits.size(); i++) niz[rands12345digits[i]];
    end = std::clock();
    nizPerformanse = (double(end - start) / double(CLOCKS_PER_SEC / 1e6)) / rands12345digits.size();

    start = std::clock();
    for (int i = 0; i < rands12345digits.size(); i++) stablo[rands12345digits[i]];
    end = std::clock();
    stabloPerformanse = (double(end - start) / double(CLOCKS_PER_SEC / 1e6)) / rands12345digits.size();

    std::cout << "Prosjecno vrijeme pristupa postojecem elementu u mapi [mikrosekunde]\n";
    std::cout << "- niz: " << nizPerformanse << "\n- stablo: " << stabloPerformanse << std::endl;
}

/*
    == OUTPUT ==

    Prosjecno vrijeme dodavanja novog elementa u mapu [mikrosekunde]
    - niz: 102.451
    - stablo: 0.4278
    Prosjecno vrijeme pristupa postojecem elementu u mapi [mikrosekunde]
    - niz: 33.2927
    - stablo: 0.2427

    == KOMENTAR ==

    S obzirom da je pri svakom dodavanju novog elementa u mapu
    potrebno prethodno provjeriti da li je element s istim kljucem vec prisutan u mapi,
    kljucna razlika u performansama se odlikuje upravo u nacinu te provjere.

    Niz pretragu izvrsava linearno s vremenskom kompleksnoscu O(n),
    a stablo, zahvaljujuci posebnoj organizaciji elemenata, s vremenskom kompleksnoscu O(log(n)).

    Cinjenica da je niz "s vremena na vrijeme" potrebno realocirati ne utice na prethodna razmatranja,
    jer je prosjecno vrijeme dodavanja novog elementa u niz konstantno.

    Mozemo reci da je pretraga mape vremenski najdominantniji dio postupka dodavanja u istu,
    stoga vrijede prethodna razmatranja.

*/

void generisi(std::vector<int>& rands12345digits, std::vector<int>& rands6digits) {
    while (rands12345digits.size() != 10000) {
        int rand = std::abs(std::rand() % 100000);

        if (std::find(rands12345digits.begin(), rands12345digits.end(), rand) == rands12345digits.end())
            rands12345digits.push_back(rand);
    }

    while (rands6digits.size() != 10000) {
        int rand = std::abs(std::rand());

        if (rand >= 100000 && std::find(rands6digits.begin(), rands6digits.end(), rand) == rands6digits.end())
            rands6digits.push_back(rand);
    }
}

int main() {
    /*
    std::vector<int> rands12345digits, rands6digits;
    generisi(rands12345digits, rands6digits);

    testDodajPerformanse(rands12345digits, rands6digits);
    testPristupPerformanse(rands12345digits);
    */

    /*
    BinStabloMapa<int, std::string> mapa;

    mapa[1] = "jedan";
    mapa[9] = "devet";
    mapa[3] = "tri";
    mapa[2] = "dva";
    mapa[7] = "sedam";

    testMapa(mapa);
    */

    return 0;
}

// ==================================================================================================== BEGIN - TEST FUNKCIJE

void testBrojElemenata(BinStabloMapa<int, std::string> mapa) {
    std::cout << std::string(75, '-') << " TEST brojElemenata()\n";

    std::cout << mapa.brojElemenata() << std::endl;
    mapa.obrisi(3);
    std::cout << mapa.brojElemenata() << std::endl;
    mapa.obrisi();
    std::cout << mapa.brojElemenata() << std::endl;
}

void testObrisiSve(BinStabloMapa<int, std::string> mapa) {
    std::cout << std::string(75, '-') << " TEST obrisiSve()\n";

    for (int i = 1; i <= 9; i++) std::cout << "'" << mapa[i] << "' ";
    std::cout << std::endl;

    mapa.obrisi();
    for (int i = 1; i <= 9; i++) std::cout << "'" << mapa[i] << "' ";
    std::cout << std::endl;

    mapa.obrisi();
    for (int i = 1; i <= 9; i++) std::cout << "'" << mapa[i] << "' ";
    std::cout << std::endl;
}

void testObrisi(BinStabloMapa<int, std::string> mapa) {
    std::cout << std::string(75, '-') << " TEST obrisi()\n";

    for (int i = 1; i <= 9; i++) std::cout << "'" << mapa[i] << "' ";
    std::cout << std::endl;

    mapa.obrisi(2);
    for (int i = 1; i <= 9; i++) std::cout << "'" << mapa[i] << "' ";
    std::cout << std::endl;

    try {
        mapa.obrisi(11);
    } catch (std::logic_error& e) {
        std::cout << "IZUZETAK: " << e.what() << std::endl;
    }
}

void testOperator(BinStabloMapa<int, std::string> mapa) {
    std::cout << std::string(75, '-') << " TEST operator []()\n";

    for (int i = 1; i <= 9; i++) std::cout << "'" << mapa[i] << "' ";
    std::cout << std::endl;

    mapa[1] = "kec";
    mapa[5] = "pet";
    mapa[11] = "jedanaest";

    for (int i = 1; i <= 11; i++) std::cout << "'" << mapa[i] << "' ";
    std::cout << std::endl;
}

void testOperatorConst(const BinStabloMapa<int, std::string>& mapa) {
    std::cout << std::string(75, '-') << " TEST operatorConst []()\n";

    for (int i = 1; i <= 15; i++) std::cout << "'" << mapa[i] << "' ";
    std::cout << std::endl << mapa.brojElemenata() << std::endl;
}

void testMapa(const BinStabloMapa<int, std::string>& mapa) {
    testBrojElemenata(mapa);
    testObrisiSve(mapa);
    testObrisi(mapa);
    testOperator(mapa);
    testOperatorConst(mapa);
}

// ==================================================================================================== END - TEST FUNKCIJE