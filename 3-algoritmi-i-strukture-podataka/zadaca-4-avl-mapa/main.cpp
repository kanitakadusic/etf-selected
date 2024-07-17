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

// ==================================================================================================== BEGIN - AVL STABLO MAPA

template <typename Key, typename Value>
struct Node {
    Key key;
    Value value;

    int height;
    Node *parent, *left, *right;

    Node(const Key& key, const Value& value = Value(), int height = 1, Node* parent = nullptr, Node* left = nullptr, Node* right = nullptr):
            key(key), value(value), height(height), parent(parent), left(left), right(right) {}
};

template <typename Key, typename Value>
class AVLStabloMapa: public Mapa<Key, Value> {
    int size;
    Node<Key, Value>* root;

    // --------------------

    static int getHeight(Node<Key, Value>*);
    static int updateHeight(Node<Key, Value>*);
    static int getBalance(Node<Key, Value>*);

    enum Rotation {LEFT, RIGHT};
    void rotate(Node<Key, Value>*, Rotation);
    void balance(Node<Key, Value>*);

    // --------------------

    Node<Key, Value>* add(Node<Key, Value>*&, const Key&, Node<Key, Value>* parent = nullptr);
    bool erase(const Key&);

    static void clone(Node<Key, Value>*, Node<Key, Value>*&, Node<Key, Value>* parent = nullptr);
    static Node<Key, Value>* search(Node<Key, Value>*, const Key&);
    static void deallocate(Node<Key, Value>*);
    static void auxPreorder(Node<Key, Value>*);

    // --------------------

public:
    AVLStabloMapa(): size(0), root(nullptr) {}

    AVLStabloMapa(const AVLStabloMapa& mapa): size(mapa.size), root(nullptr) { clone(mapa.root, this->root); }
    AVLStabloMapa& operator =(const AVLStabloMapa& mapa) {
        if (&mapa != this) { this->obrisi(); clone(mapa.root, this->root); this->size = mapa.size; }
        return *this;
    }

    AVLStabloMapa(AVLStabloMapa&& mapa): size(mapa.size), root(mapa.root) { mapa.size = 0; mapa.root = nullptr; }
    AVLStabloMapa& operator =(AVLStabloMapa&& mapa) {
        std::swap(this->size, mapa.size); std::swap(this->root, mapa.root);
        return *this;
    }

    ~AVLStabloMapa() { deallocate(this->root); }

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

    void preorder() const { auxPreorder(this->root); }
};

// ------------------------------------------------------- BEGIN - POMOCNE AVL METODE

template <typename Key, typename Value>
inline int AVLStabloMapa<Key, Value>::getHeight(Node<Key, Value>* x) {
    return x == nullptr ? 0 : x->height;
}

template <typename Key, typename Value>
inline int AVLStabloMapa<Key, Value>::updateHeight(Node<Key, Value>* x) {
    if (x != nullptr) {
        int L = getHeight(x->left), R = getHeight(x->right);
        return (x->height = (L > R ? L : R) + 1);
    }

    return 0;
}

template <typename Key, typename Value>
inline int AVLStabloMapa<Key, Value>::getBalance(Node<Key, Value>* x) {
    return x == nullptr ? 0 : getHeight(x->left) - getHeight(x->right);
}

template <typename Key, typename Value>
void AVLStabloMapa<Key, Value>::rotate(Node<Key, Value>* x, Rotation side) {
    Node<Key, Value>* p = x->parent->parent;
    if (p == nullptr) this->root = x;
    else {
        if (p->left == x->parent) p->left = x;
        else p->right = x;
    }

    if (side == LEFT) {
        x->parent->right = x->left;
        if (x->left != nullptr) x->left->parent = x->parent;
        x->left = x->parent;
    } else {
        x->parent->left = x->right;
        if (x->right != nullptr) x->right->parent = x->parent;
        x->right = x->parent;
    }

    x->parent->parent = x;

    updateHeight(x->parent);
    updateHeight(x);

    x->parent = p;
}

template <typename Key, typename Value>
void AVLStabloMapa<Key, Value>::balance(Node<Key, Value>* x) {
    int balance = getBalance(x);

    if (balance == -2) {
        if (getBalance(x->right) < 1) this->rotate(x->right, LEFT);
        else { this->rotate(x->right->left, RIGHT); this->rotate(x->right, LEFT); }
    } else if (balance == 2) {
        if (getBalance(x->left) > -1) this->rotate(x->left, RIGHT);
        else { this->rotate(x->left->right, LEFT); this->rotate(x->left, RIGHT); }
    }
}

// ------------------------------------------------------- END - POMOCNE AVL METODE

// ------------------------------------------------------- BEGIN - POMOCNE BST METODE

template <typename Key, typename Value>
Node<Key, Value>* AVLStabloMapa<Key, Value>::add(Node<Key, Value>*& x, const Key& key, Node<Key, Value>* parent) {
    if (x == nullptr) {
        x = new Node<Key, Value>(key, Value(), 1, parent);
        this->size++;

        // -----

        Node<Key, Value> *aux = x, *fix = x->parent;

        while (fix != nullptr) {
            if (fix->height == updateHeight(fix)) break;
            this->balance(fix);
            fix = fix->parent;
        }

        // -----

        return aux;
    }
    else if (key < x->key) return this->add(x->left, key, x);
    else if (key > x->key) return this->add(x->right, key, x);
    else return x;
}

template <typename Key, typename Value>
bool AVLStabloMapa<Key, Value>::erase(const Key& key) {
    Node<Key, Value> *x = nullptr, *subtree = nullptr, *fix = nullptr;

    if ((x = search(this->root, key)) == nullptr) return false;
    fix = x->parent;

    if (x->left == nullptr) subtree = x->right;
    else if (x->right == nullptr) subtree = x->left;
    else {
        Node<Key, Value>* maxLeft = subtree = x->left;

        while (maxLeft->right != nullptr) maxLeft = maxLeft->right;
        fix = maxLeft->parent;

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

    // -----

    if (fix != nullptr) {
        updateHeight(fix);
        this->balance(fix);
        fix = fix->parent;
    }

    while (fix != nullptr) {
        if (fix->height == updateHeight(fix)) break;
        this->balance(fix);
        fix = fix->parent;
    }

    // -----

    delete x;
    this->size--;

    return true;
}

template <typename Key, typename Value>
void AVLStabloMapa<Key, Value>::clone(Node<Key, Value>* source, Node<Key, Value>*& destination, Node<Key, Value>* parent) {
    if (source != nullptr) {
        destination = new Node<Key, Value>(source->key, source->value, source->height, parent);
        clone(source->left, destination->left, destination);
        clone(source->right, destination->right, destination);
    }
}

template <typename Key, typename Value>
Node<Key, Value>* AVLStabloMapa<Key, Value>::search(Node<Key, Value>* x, const Key& key) {
    while (x != nullptr && key != x->key)
        if (key < x->key) x = x->left;
        else x = x->right;

    return x;
}

template <typename Key, typename Value>
void AVLStabloMapa<Key, Value>::deallocate(Node<Key, Value>* tree) {
    if (tree != nullptr) {
        deallocate(tree->left);
        deallocate(tree->right);

        delete tree;
    }
}

template <typename Key, typename Value>
void AVLStabloMapa<Key, Value>::auxPreorder(Node<Key, Value>* tree) {
    if (tree != nullptr) {
        std::cout << tree->key << " ";
        auxPreorder(tree->left);
        auxPreorder(tree->right);
    }
}

// ------------------------------------------------------- END - POMOCNE BST METODE

// ==================================================================================================== END - AVL STABLO MAPA

void testBrojElemenata();
void testObrisiSve();
void testObrisi();
void testOperator();
void testOperatorConst();

int main() {

    /*
    testBrojElemenata();
    testObrisiSve();
    testObrisi();
    testOperator();
    testOperatorConst();
    */

    return 0;
}

// ==================================================================================================== BEGIN - TEST FUNKCIJE

void dajInfo(const AVLStabloMapa<int, int>& mapa) {
    std::cout << "{ "; mapa.preorder();
    std::cout << "} => "; std::cout << mapa.brojElemenata() << std::endl;
}

void testBrojElemenata() {
    std::cout << std::string(60, '-') << " TEST brojElemenata()\n";

    AVLStabloMapa<int, int> mapa;
    for (int i = 1; i <= 10; i++) mapa[i];
    dajInfo(mapa);

    mapa.obrisi(3); dajInfo(mapa);
    mapa.obrisi(); dajInfo(mapa);
}

void testObrisiSve() {
    std::cout << std::string(60, '-') << " TEST obrisiSve()\n";

    AVLStabloMapa<int, int> mapa;
    for (int i = 10; i >= 1; i--) mapa[i];
    dajInfo(mapa);

    mapa.obrisi(); dajInfo(mapa);
    mapa.obrisi(); dajInfo(mapa);
}

void testObrisi() {
    std::cout << std::string(60, '-') << " TEST obrisi()\n";

    AVLStabloMapa<int, int> mapa;
    mapa[8]; mapa[5]; mapa[11]; mapa[2]; mapa[7]; mapa[9]; mapa[12]; mapa[10]; mapa[6]; mapa[1]; mapa[3]; mapa[4];
    dajInfo(mapa);

    mapa.obrisi(1); dajInfo(mapa);
    mapa.obrisi(7); mapa.obrisi(6); dajInfo(mapa);
    mapa.obrisi(8); dajInfo(mapa);

    for (int i = 1; i <= 12; i++)
        if (i != 1 && i != 6 && i != 7 && i != 8)
            mapa.obrisi(i);

    dajInfo(mapa);

    try { mapa.obrisi(1); }
    catch (std::logic_error& e) { std::cout << e.what() << std::endl; }
}

void testOperator() {
    std::cout << std::string(60, '-') << " TEST operator []()\n";

    AVLStabloMapa<int, int> mapa;
    mapa[5]; mapa[3]; mapa[7]; mapa[2]; mapa[4]; mapa[6]; mapa[9]; mapa[1]; mapa[8]; mapa[10];
    dajInfo(mapa);

    mapa[-1]; dajInfo(mapa);
    mapa[-2]; dajInfo(mapa);
    mapa[-3]; mapa[-4]; mapa[-5]; mapa[-6]; dajInfo(mapa);
    mapa[-2]; mapa[5]; mapa[10]; dajInfo(mapa);
}

void testOperatorConst() {
    std::cout << std::string(60, '-') << " TEST operatorConst []()\n";

    AVLStabloMapa<int, int> m;
    for (int i = 1; i <= 10; i++) m[i] = 1;

    const AVLStabloMapa<int, int> mapa(m);
    dajInfo(mapa);

    for (int i = 1; i <= 15; i++) std::cout << mapa[i] << " ";
    std::cout << std::endl; dajInfo(mapa);
}

// ==================================================================================================== END - TEST FUNKCIJE