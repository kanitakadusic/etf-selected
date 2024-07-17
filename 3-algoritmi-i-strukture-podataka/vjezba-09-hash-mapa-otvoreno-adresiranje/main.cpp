// ====================================================================================================
// NAPOMENA: Za pokretanje testnih funkcija ukloniti komentar u main() funkciji
// ====================================================================================================

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <functional>
#include <iomanip>
#include <vector>
#include <memory>

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

// ==================================================================================================== BEGIN - HASH MAPA

unsigned int hash(std::string input, unsigned int max) {
    unsigned int sum = 5381;

    for (int i = 0; i < input.length(); i++)
        sum = sum * 33 + input[i];

    return sum % max;
}

template <typename Key, typename Value>
class HashMapa: public Mapa<Key, Value> {
    std::vector<std::shared_ptr<std::pair<Key, Value>>> table;
    int usage;

    unsigned int initialCapacity;
    float loadFactor;

    unsigned int (* hashCode)(Key, unsigned int);

    // --------------------

    static std::shared_ptr<std::pair<Key, Value>> nil;

    void rehash();

    // --------------------

public:
    HashMapa(unsigned int initialCapacity = 11, float loadFactor = 0.75);

    HashMapa(const HashMapa&);
    HashMapa& operator =(const HashMapa&);

    // --------------------

    int brojElemenata() const override { return this->usage; }

    void obrisi() override;
    void obrisi(const Key&) override;

    Value& operator [](const Key&) override;
    Value operator [](const Key&) const override;

    void definisiHashFunkciju(unsigned int (* hashCode)(Key, unsigned int)) { this->hashCode = hashCode; }
};

template <typename Key, typename Value>
std::shared_ptr<std::pair<Key, Value>> HashMapa<Key, Value>::nil = std::make_shared<std::pair<Key, Value>>();

// ------------------------------------------------------- BEGIN - UPRAVLJACKI DIO KLASE

template <typename Key, typename Value>
HashMapa<Key, Value>::HashMapa(unsigned int initialCapacity, float loadFactor):
        table(initialCapacity), usage(0), initialCapacity(initialCapacity), loadFactor(loadFactor) {
    this->table.shrink_to_fit();
}

template <typename Key, typename Value>
HashMapa<Key, Value>::HashMapa(const HashMapa& map):
        table(map.table.size()), usage(map.usage), initialCapacity(map.initialCapacity), loadFactor(map.loadFactor), hashCode(map.hashCode) {
    this->table.shrink_to_fit();

    std::transform(map.table.begin(), map.table.end(), this->table.begin(), [](const std::shared_ptr<std::pair<Key, Value>>& pair) {
        return pair == nullptr ? nullptr : std::make_shared<std::pair<Key, Value>>(*pair);
    });
}

template <typename Key, typename Value>
HashMapa<Key, Value>& HashMapa<Key, Value>::operator =(const HashMapa& map) {
    if (&map != this) {
        this->table.resize(map.table.size());
        this->table.shrink_to_fit();
        this->usage = map.usage;
        this->initialCapacity = map.initialCapacity;
        this->loadFactor = map.loadFactor;
        this->hashCode = map.hashCode;

        std::transform(map.table.begin(), map.table.end(), this->table.begin(), [](const std::shared_ptr<std::pair<Key, Value>>& pair) {
            return pair == nullptr ? nullptr : std::make_shared<std::pair<Key, Value>>(*pair);
        });
    }

    return *this;
}

// ------------------------------------------------------- END - UPRAVLJACKI DIO KLASE

// ------------------------------------------------------- BEGIN - POMOCNE HASH METODE

template <typename Key, typename Value>
void HashMapa<Key, Value>::rehash() {
    std::vector<std::shared_ptr<std::pair<Key, Value>>> rehashed(this->table.size() * 2);
    rehashed.shrink_to_fit();

    for (int i = 0; i < this->table.size(); i++) {
        if (this->table[i] != nullptr && this->table[i] != nil) {
            unsigned int h = this->hashCode(this->table[i]->first, rehashed.size());

            for (int j = 0; j < rehashed.size(); j++) {
                unsigned int next = h % rehashed.size();

                if (rehashed[next] == nullptr) {
                    rehashed[next] = std::make_shared<std::pair<Key, Value>>(*this->table[i]);
                    break;
                }

                h++;
            }
        }
    }

    this->table = std::move(rehashed);
}

// ------------------------------------------------------- END - POMOCNE HASH METODE

// ------------------------------------------------------- BEGIN - IMPLEMENTACIJA INTERFEJSA

template <typename Key, typename Value>
void HashMapa<Key, Value>::obrisi() {
    this->table.clear();
    this->table.resize(this->initialCapacity);
    this->table.shrink_to_fit();
    this->usage = 0;
}

template <typename Key, typename Value>
void HashMapa<Key, Value>::obrisi(const Key& key) {
    if (this->hashCode == nullptr) throw std::logic_error("Nedefinisana hash funkcija");

    unsigned int h = this->hashCode(key, this->table.size());

    for (int i = 0; i < this->table.size(); i++) {
        unsigned int next = h % this->table.size();

        if (this->table[next] == nullptr) {
            break;
        } else if (this->table[next]->first == key) {
            this->table[next] = nil;
            this->usage--;
            return;
        }

        h++;
    }

    throw std::logic_error("Nepostojeci kljuc");
}

template <typename Key, typename Value>
Value& HashMapa<Key, Value>::operator [](const Key& key) {
    if (this->hashCode == nullptr) throw std::logic_error("Nedefinisana hash funkcija");
    if (float(this->usage) / this->table.size() >= loadFactor) this->rehash();

    unsigned int h = this->hashCode(key, this->table.size());
    int firstFreeBucketIndex = -1;

    for (int i = 0; i < this->table.size(); i++) {
        unsigned int next = h % this->table.size();

        if (this->table[next] == nullptr) {
            if (firstFreeBucketIndex == -1) firstFreeBucketIndex = next;
            break;
        } else if (firstFreeBucketIndex == -1 && this->table[next] == nil) {
            firstFreeBucketIndex = next;
        } else if (this->table[next]->first == key) {
            return this->table[next]->second;
        }

        h++;
    }

    if (firstFreeBucketIndex != -1) {
        this->table[firstFreeBucketIndex] = std::make_shared<std::pair<Key, Value>>(key, Value());
        this->usage++;
        return this->table[firstFreeBucketIndex]->second;
    }

    throw std::logic_error("Greska");
}

template <typename Key, typename Value>
Value HashMapa<Key, Value>::operator [](const Key& key) const {
    if (this->hashCode == nullptr) throw std::logic_error("Nedefinisana hash funkcija");

    unsigned int h = this->hashCode(key, this->table.size());

    for (int i = 0; i < this->table.size(); i++) {
        unsigned int next = h % this->table.size();

        if (this->table[next] == nullptr) {
            break;
        } else if (this->table[next]->first == key) {
            return this->table[next]->second;
        }

        h++;
    }

    return Value();
}

// ------------------------------------------------------- END - IMPLEMENTACIJA INTERFEJSA

// ==================================================================================================== END - HASH MAPA

void performanseDodavanja(std::vector<int>&, std::vector<int>&);
void performansePristupa(std::vector<int>&);
void performanseBrisanja(std::vector<int>&);

void generisi(std::vector<int>&, std::vector<int>&);

/*
    == OUTPUT ==

    Prosjecno vrijeme DODAVANJA novog elementa u mapu [mikrosekunde]
    - niz: 100.311
    - stablo: 0.4194
    - hes: 1.7347
    Prosjecno vrijeme PRISTUPA postojecem elementu u mapi [mikrosekunde]
    - niz: 32.6417
    - stablo: 0.2861
    - hes: 0.1761
    Prosjecno vrijeme BRISANJA postojeceg elementa u mapi [mikrosekunde]
    - niz: 16.762
    - stablo: 0.3143
    - hes: 0.3399

    == KOMENTAR ==

    Evidentno je da se najgori rezultati dobijaju za NizMapu,
    dok su oni za BinStabloMapu i HashMapu izrazito bliski.

    Iako bi se moglo ocekivati da su performanse HashMape u svim aspektima najbolje,
    kljucni momenat se ogleda u implementacijskim detaljima koji,
    u slucaju BinStabloMape i NizMape ovise iskljucivo od dizajnera tih struktura,
    dok u slucaju HashMape postoje korisnicki definisani parametri ukljuceni u njenu implementacijsku logiku.

    Radi se o vrlo bitnoj hash funkciji, te initialCapacity i loadFactor parametrima,
    koje korisnik postavlja u skladu sa zahtjevima,
    a koji diktiraju vremensku i memorijsku kompleksnost ove strukture.

*/

int main() {

    /*
    std::vector<int> rands12345digits, rands6digits;
    generisi(rands12345digits, rands6digits);

    performanseDodavanja(rands12345digits, rands6digits);
    performansePristupa(rands12345digits);
    performanseBrisanja(rands12345digits);
    */

    return 0;
}

unsigned int hashInt(int x, unsigned int max) {
    return x % max;
}

void performanseDodavanja(std::vector<int>& rands12345digits, std::vector<int>& rands6digits) {
    NizMapa<int, int> niz;
    BinStabloMapa<int, int> stablo;
    HashMapa<int, int> hes;
    hes.definisiHashFunkciju(hashInt);

    for (int i = 0; i < rands12345digits.size(); i++)
        niz[rands12345digits[i]] = stablo[rands12345digits[i]] = hes[rands12345digits[i]] = rands12345digits[i];

    clock_t start = 0, end = 0;
    double nizPerformanse, stabloPerformanse, hesPerformanse;

    start = std::clock();
    for (int i = 0; i < rands6digits.size(); i++) niz[rands6digits[i]] = rands6digits[i];
    end = std::clock();
    nizPerformanse = (double(end - start) / double(CLOCKS_PER_SEC / 1e6)) / rands6digits.size();

    start = std::clock();
    for (int i = 0; i < rands6digits.size(); i++) stablo[rands6digits[i]] = rands6digits[i];
    end = std::clock();
    stabloPerformanse = (double(end - start) / double(CLOCKS_PER_SEC / 1e6)) / rands6digits.size();

    start = std::clock();
    for (int i = 0; i < rands6digits.size(); i++) hes[rands6digits[i]] = rands6digits[i];
    end = std::clock();
    hesPerformanse = (double(end - start) / double(CLOCKS_PER_SEC / 1e6)) / rands6digits.size();

    std::cout << "Prosjecno vrijeme DODAVANJA novog elementa u mapu [mikrosekunde]\n";
    std::cout << "- niz: " << nizPerformanse << "\n- stablo: " << stabloPerformanse << "\n- hes: " << hesPerformanse << std::endl;
}

void performansePristupa(std::vector<int>& rands12345digits) {
    NizMapa<int, int> niz;
    BinStabloMapa<int, int> stablo;
    HashMapa<int, int> hes;
    hes.definisiHashFunkciju(hashInt);

    for (int i = 0; i < rands12345digits.size(); i++)
        niz[rands12345digits[i]] = stablo[rands12345digits[i]] = hes[rands12345digits[i]] = rands12345digits[i];

    clock_t start = 0, end = 0;
    double nizPerformanse, stabloPerformanse, hesPerformanse;
    std::random_shuffle(rands12345digits.begin(), rands12345digits.end());

    start = std::clock();
    for (int i = 0; i < rands12345digits.size(); i++) niz[rands12345digits[i]];
    end = std::clock();
    nizPerformanse = (double(end - start) / double(CLOCKS_PER_SEC / 1e6)) / rands12345digits.size();

    start = std::clock();
    for (int i = 0; i < rands12345digits.size(); i++) stablo[rands12345digits[i]];
    end = std::clock();
    stabloPerformanse = (double(end - start) / double(CLOCKS_PER_SEC / 1e6)) / rands12345digits.size();

    start = std::clock();
    for (int i = 0; i < rands12345digits.size(); i++) hes[rands12345digits[i]];
    end = std::clock();
    hesPerformanse = (double(end - start) / double(CLOCKS_PER_SEC / 1e6)) / rands12345digits.size();

    std::cout << "Prosjecno vrijeme PRISTUPA postojecem elementu u mapi [mikrosekunde]\n";
    std::cout << "- niz: " << nizPerformanse << "\n- stablo: " << stabloPerformanse << "\n- hes: " << hesPerformanse << std::endl;
}

void performanseBrisanja(std::vector<int>& rands12345digits) {
    NizMapa<int, int> niz;
    BinStabloMapa<int, int> stablo;
    HashMapa<int, int> hes;
    hes.definisiHashFunkciju(hashInt);

    for (int i = 0; i < rands12345digits.size(); i++)
        niz[rands12345digits[i]] = stablo[rands12345digits[i]] = hes[rands12345digits[i]] = rands12345digits[i];

    clock_t start = 0, end = 0;
    double nizPerformanse, stabloPerformanse, hesPerformanse;
    std::random_shuffle(rands12345digits.begin(), rands12345digits.end());

    start = std::clock();
    for (int i = 0; i < rands12345digits.size(); i++) niz.obrisi(rands12345digits[i]);
    end = std::clock();
    nizPerformanse = (double(end - start) / double(CLOCKS_PER_SEC / 1e6)) / rands12345digits.size();

    start = std::clock();
    for (int i = 0; i < rands12345digits.size(); i++) stablo.obrisi(rands12345digits[i]);
    end = std::clock();
    stabloPerformanse = (double(end - start) / double(CLOCKS_PER_SEC / 1e6)) / rands12345digits.size();

    start = std::clock();
    for (int i = 0; i < rands12345digits.size(); i++) hes.obrisi(rands12345digits[i]);
    end = std::clock();
    hesPerformanse = (double(end - start) / double(CLOCKS_PER_SEC / 1e6)) / rands12345digits.size();

    std::cout << "Prosjecno vrijeme BRISANJA postojeceg elementa u mapi [mikrosekunde]\n";
    std::cout << "- niz: " << nizPerformanse << "\n- stablo: " << stabloPerformanse << "\n- hes: " << hesPerformanse << std::endl;
}

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
