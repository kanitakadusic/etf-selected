#include <iostream>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <vector>
#include <list>
#include <queue>

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

// ==================================================================================================== BEGIN - HASH MAPA

template <typename Key, typename Value>
class HashMapaLan: public Mapa<Key, Value> {
    std::vector<std::list<std::pair<Key, Value>>> table;
    int usage;

    unsigned int (* hashCode)(Key, unsigned int);

public:
    HashMapaLan(): table(773), usage(0) { this->table.shrink_to_fit(); }

    // --------------------

    int brojElemenata() const override { return this->usage; }

    void obrisi() override;
    void obrisi(const Key&) override;

    Value& operator [](const Key&) override;
    Value operator [](const Key&) const override;

    void definisiHashFunkciju(unsigned int (* hashCode)(Key, unsigned int)) { this->hashCode = hashCode; }
};

// ------------------------------------------------------- BEGIN - IMPLEMENTACIJA INTERFEJSA

template <typename Key, typename Value>
void HashMapaLan<Key, Value>::obrisi() {
    this->table.clear();
    this->table.resize(773);
    this->table.shrink_to_fit();
    this->usage = 0;
}

template <typename Key, typename Value>
void HashMapaLan<Key, Value>::obrisi(const Key& key) {
    if (this->hashCode == nullptr) throw std::logic_error("Nedefinisana hash funkcija");
    unsigned int h = this->hashCode(key, this->table.size());

    if (!this->table[h].empty()) {
        bool found = false;

        auto it = std::find_if(this->table[h].begin(), this->table[h].end(), [&key, &found](const std::pair<Key, Value>& pair) {
            if (key == pair.first) { found = true; return true; }
            if (key < pair.first) return true;

            return false;
        });

        if (found) {
            this->usage--;
            this->table[h].erase(it);
            return;
        }
    }

    throw std::logic_error("Nepostojeci kljuc");
}

template <typename Key, typename Value>
Value& HashMapaLan<Key, Value>::operator [](const Key& key) {
    if (this->hashCode == nullptr) throw std::logic_error("Nedefinisana hash funkcija");
    unsigned int h = this->hashCode(key, this->table.size());

    auto it = this->table[h].begin();

    if (!this->table[h].empty()) {
        bool found = false;

        it = std::find_if(this->table[h].begin(), this->table[h].end(), [&key, &found](const std::pair<Key, Value>& pair) {
            if (key == pair.first) { found = true; return true; }
            if (key < pair.first) return true;

            return false;
        });

        if (found) return it->second;
    }

    this->usage++;
    return this->table[h].emplace(it, key, Value())->second;
}

template <typename Key, typename Value>
Value HashMapaLan<Key, Value>::operator [](const Key& key) const {
    if (this->hashCode == nullptr) throw std::logic_error("Nedefinisana hash funkcija");
    unsigned int h = this->hashCode(key, this->table.size());

    if (!this->table[h].empty()) {
        bool found = false;

        auto it = std::find_if(this->table[h].begin(), this->table[h].end(), [&key, &found](const std::pair<Key, Value>& pair) {
            if (key == pair.first) { found = true; return true; }
            if (key < pair.first) return true;

            return false;
        });

        if (found) return it->second;
    }

    return Value();
}

// ------------------------------------------------------- END - IMPLEMENTACIJA INTERFEJSA

// ==================================================================================================== END - HASH MAPA

template <typename Label>
class Cvor;

template <typename Label>
class Grana;

template <typename Label>
class GranaIterator;

template <typename Label>
class UsmjereniGraf;

template <typename Label>
class ListaGraf;

// ==================================================================================================== BEGIN - CVOR

template <typename Label>
class Cvor {
    /*const*/ int num;
    std::shared_ptr<Label> label;

public:
    Cvor(int num): num(num), label(std::make_shared<Label>(Label())) {}

    Cvor(UsmjereniGraf<Label>* graph, int num): num(num), label(graph->dajCvor(num).label) {}

    // --------------------

    int dajRedniBroj() const { return this->num; }

    Label dajOznaku() const { return *this->label; }
    void postaviOznaku(Label label) { *this->label = label; }
};

// ==================================================================================================== END - CVOR

// ==================================================================================================== BEGIN - GRANA

template <typename Label>
class Grana {
    /*const*/ std::shared_ptr<Cvor<Label>> startNode;
    /*const*/ std::shared_ptr<Cvor<Label>> endNode;
    std::shared_ptr<double> weight;
    std::shared_ptr<Label> label;

public:
    Grana(std::shared_ptr<Cvor<Label>> startNode, std::shared_ptr<Cvor<Label>> endNode, double weight = 0):
            startNode(startNode), endNode(endNode), weight(std::make_shared<double>(weight)), label(std::make_shared<Label>(Label())) {}

    Grana(UsmjereniGraf<Label>* graph, int startNum, int endNum) {
        Grana<Label> edge = graph->dajGranu(startNum, endNum);

        this->startNode = edge.startNode;
        this->endNode = edge.endNode;

        this->weight = edge.weight;
        this->label = edge.label;
    }

    // --------------------

    Cvor<Label> dajPolazniCvor() const { return *this->startNode; }
    Cvor<Label> dajDolazniCvor() const { return *this->endNode; }

    double dajTezinu() const { return *this->weight; }
    void postaviTezinu(double weight) { *this->weight = weight; }

    Label dajOznaku() const { return *this->label; }
    void postaviOznaku(Label label) { *this->label = label; }
};

// ==================================================================================================== END - GRANA

// ==================================================================================================== BEGIN - GRANA ITERATOR

template <typename Label>
class GranaIterator {
    /*const*/ UsmjereniGraf<Label>* graph;
    int startNum, endNum;

public:
    GranaIterator(UsmjereniGraf<Label>* graph, int startNum, int endNum): graph(graph), startNum(startNum), endNum(endNum) {}

    // --------------------

    Grana<Label> operator *() { return graph->dajGranu(startNum, endNum); }

    bool operator ==(const GranaIterator& it) const { return graph == it.graph && startNum == it.startNum && endNum == it.endNum; }
    bool operator !=(const GranaIterator& it) const { return graph != it.graph || startNum != it.startNum || endNum != it.endNum; }

    GranaIterator& operator ++() { graph->goToNextEdge(startNum, endNum); return *this; }
    GranaIterator operator ++(int) { auto temp = *this; graph->goToNextEdge(startNum, endNum); return temp; }
};

// ==================================================================================================== END - GRANA ITERATOR

// ==================================================================================================== BEGIN - USMJERENI GRAF

template <typename Label>
class UsmjereniGraf {
    std::pair<int, int> firstEdge;

    virtual bool goToNextEdge(int&, int&) = 0;

    virtual void bfsComponent(std::vector<Cvor<Label>>&, int, std::vector<bool>&) const = 0;
    virtual void dfsComponent(std::vector<Cvor<Label>>&, int, std::vector<bool>&) const = 0;

protected:
    void checkNode(int num) const { if (num < 0 || num >= dajBrojCvorova()) throw std::domain_error("UsmjereniGraf: checkNode"); }

    void updateFirstEdgeAfterInsertion(std::pair<int, int> inserted) { if (inserted < firstEdge) firstEdge = inserted; }
    void updateFirstEdgeAfterDeletion(std::pair<int, int> deleted) { if (deleted == firstEdge) goToNextEdge(firstEdge.first, firstEdge.second); }

public:
    UsmjereniGraf(unsigned int size): firstEdge(size, -1) {}

    UsmjereniGraf(const UsmjereniGraf&) = default;
    UsmjereniGraf& operator =(const UsmjereniGraf&) = default;

    UsmjereniGraf(UsmjereniGraf&&) = default;
    UsmjereniGraf& operator =(UsmjereniGraf&&) = default;

    virtual ~UsmjereniGraf() = default;

    // --------------------

    friend class GranaIterator<Label>;

    GranaIterator<Label> dajGranePocetak() /*const*/ { return GranaIterator<Label>(this, firstEdge.first, firstEdge.second); }
    GranaIterator<Label> dajGraneKraj() /*const*/ { return GranaIterator<Label>(this, dajBrojCvorova(), -1); }

    // --------------------

    virtual Cvor<Label> dajCvor(int) const = 0;
    virtual Grana<Label> dajGranu(int, int) const = 0;

    virtual int dajBrojCvorova() const = 0;
    virtual void postaviBrojCvorova(unsigned int) = 0;

    virtual bool postojiGrana(int, int) const = 0;
    virtual void dodajGranu(int, int, double) = 0;
    virtual void obrisiGranu(int, int) = 0;

    // --------------------

    Label dajOznakuCvora(int num) const { return dajCvor(num).dajOznaku(); }
    void postaviOznakuCvora(int num, Label label) { dajCvor(num).postaviOznaku(label); }

    double dajTezinuGrane(int startNum, int endNum) const { return dajGranu(startNum, endNum).dajTezinu(); }
    void postaviTezinuGrane(int startNum, int endNum, double weight) { dajGranu(startNum, endNum).postaviTezinu(weight); }

    Label dajOznakuGrane(int startNum, int endNum) const { return dajGranu(startNum, endNum).dajOznaku(); }
    void postaviOznakuGrane(int startNum, int endNum, Label label) { dajGranu(startNum, endNum).postaviOznaku(label); }

    // --------------------

    std::vector<Cvor<Label>> bfs(const Cvor<Label>&) const;
    std::vector<Cvor<Label>> dfs(const Cvor<Label>&) const;
};

template <typename Label>
std::vector<Cvor<Label>> UsmjereniGraf<Label>::bfs(const Cvor<Label>& node) const {
    std::vector<Cvor<Label>> nodes;
    std::vector<bool> visited(dajBrojCvorova(), false);

    int begin = node.dajRedniBroj(), end = begin + dajBrojCvorova(), x;

    for (int i = begin; i < end; i++) {
        x = i % dajBrojCvorova();
        if (!visited[x]) bfsComponent(nodes, x, visited);
    }

    return nodes;
}

template <typename Label>
std::vector<Cvor<Label>> UsmjereniGraf<Label>::dfs(const Cvor<Label>& node) const {
    std::vector<Cvor<Label>> nodes;
    std::vector<bool> visited(dajBrojCvorova(), false);

    int begin = node.dajRedniBroj(), end = begin + dajBrojCvorova(), x;

    for (int i = begin; i < end; i++) {
        x = i % dajBrojCvorova();
        if (!visited[x]) dfsComponent(nodes, x, visited);
    }

    return nodes;
}

// ==================================================================================================== END - USMJERENI GRAF

// ==================================================================================================== BEGIN - LISTA GRAF

template <typename Label>
class ListaGraf: public UsmjereniGraf<Label> {
    std::vector<std::pair<std::shared_ptr<Cvor<Label>>, std::list<Grana<Label>>>> graph;

    std::pair<typename std::list<Grana<Label>>::const_iterator, bool> findEdge(int, int) const;

    bool goToNextEdge(int&, int&) override;

    void bfsComponent(std::vector<Cvor<Label>>&, int, std::vector<bool>&) const override;
    void dfsComponent(std::vector<Cvor<Label>>&, int, std::vector<bool>&) const override;

    using UsmjereniGraf<Label>::checkNode;
    using UsmjereniGraf<Label>::updateFirstEdgeAfterInsertion;
    using UsmjereniGraf<Label>::updateFirstEdgeAfterDeletion;

public:
    ListaGraf(unsigned int size): UsmjereniGraf<Label>(size), graph(size) {
        for (int i = 0; i < size; i++) graph[i].first = std::make_shared<Cvor<Label>>(i);
    }

    ListaGraf(const ListaGraf&) = delete;
    ListaGraf& operator =(const ListaGraf&) = delete;

    ListaGraf(ListaGraf&&) = delete;
    ListaGraf& operator =(ListaGraf&&) = delete;

    ~ListaGraf() = default;

    // --------------------

    Cvor<Label> dajCvor(int num) const override { checkNode(num); return *graph[num].first; }
    Grana<Label> dajGranu(int, int) const override;

    int dajBrojCvorova() const override { return graph.size(); }
    void postaviBrojCvorova(unsigned int) override;

    bool postojiGrana(int startNum, int endNum) const override { return findEdge(startNum, endNum).second; }
    void dodajGranu(int, int, double weight = 0) override;
    void obrisiGranu(int, int) override;
};

template <typename Label>
Grana<Label> ListaGraf<Label>::dajGranu(int startNum, int endNum) const {
    auto itBool = findEdge(startNum, endNum);
    if (itBool.second) return *itBool.first;
    throw std::domain_error("ListaGraf: dajGranu");
}

template <typename Label>
void ListaGraf<Label>::postaviBrojCvorova(unsigned int size) {
    if (size <= dajBrojCvorova()) throw std::domain_error("ListaGraf: postaviBrojCvorova");
    int oldSize = dajBrojCvorova();

    graph.resize(size);
    for (int i = oldSize; i < size; i++)
        graph[i].first = std::make_shared<Cvor<Label>>(i);
}

template <typename Label>
inline void ListaGraf<Label>::dodajGranu(int startNum, int endNum, double weight) {
    auto itBool = findEdge(startNum, endNum);

    if (!itBool.second) {
        graph[startNum].second.emplace(itBool.first, graph[startNum].first, graph[endNum].first, weight);
        updateFirstEdgeAfterInsertion({startNum, endNum});
    }
}

template <typename Label>
inline void ListaGraf<Label>::obrisiGranu(int startNum, int endNum) {
    auto itBool = findEdge(startNum, endNum);

    if (itBool.second) {
        graph[startNum].second.erase(itBool.first);
        updateFirstEdgeAfterDeletion({startNum, endNum});
    }
}

template <typename Label>
std::pair<typename std::list<Grana<Label>>::const_iterator, bool> ListaGraf<Label>::findEdge(int startNum, int endNum) const {
    checkNode(startNum); checkNode(endNum);
    bool found = false;

    auto it = std::find_if(graph[startNum].second.begin(), graph[startNum].second.end(), [endNum, &found](const Grana<Label>& edge) {
        int num = edge.dajDolazniCvor().dajRedniBroj();

        if (endNum == num) { found = true; return true; }
        if (endNum < num) return true;

        return false;
    });

    return {it, found ? true : false};
}

template <typename Label> // prelazak na sljedecu granu bi bio efikasniji ako bi se slao iterator na trenutnu granu
bool ListaGraf<Label>::goToNextEdge(int& startNum, int& endNum) {
    auto itBool = findEdge(startNum, endNum);

    if (!itBool.second || ++itBool.first == graph[startNum].second.end()) {
        while (++startNum != dajBrojCvorova())
            if (!graph[startNum].second.empty()) {
                endNum = graph[startNum].second.front().dajDolazniCvor().dajRedniBroj();
                return true;
            }

        endNum = -1;
        return false;
    }

    endNum = itBool.first->dajDolazniCvor().dajRedniBroj();
    return true;
}

template <typename Label>
void ListaGraf<Label>::bfsComponent(std::vector<Cvor<Label>>& nodes, int x, std::vector<bool>& visited) const {
    std::queue<int> nums;

    nums.push(x);
    visited[x] = true;

    while (!nums.empty()) {
        x = nums.front();
        nums.pop();

        nodes.push_back(dajCvor(x));

        for (auto it = graph[x].second.begin(); it != graph[x].second.end(); it++) {
            int i = it->dajDolazniCvor().dajRedniBroj();
            if (!visited[i]) { visited[i] = true; nums.push(i); }
        }
    }
}

template <typename Label>
void ListaGraf<Label>::dfsComponent(std::vector<Cvor<Label>>& nodes, int x, std::vector<bool>& visited) const {
    nodes.push_back(dajCvor(x));
    visited[x] = true;

    for (auto it = graph[x].second.begin(); it != graph[x].second.end(); it++) {
        int i = it->dajDolazniCvor().dajRedniBroj();
        if (!visited[i]) dfsComponent(nodes, i, visited);
    }
}

// ==================================================================================================== END - LISTA GRAF

template <typename Label>
void bfs(UsmjereniGraf<Label>* graph, std::vector<Cvor<Label>>& nodes, const Cvor<Label>& node) {
    nodes = graph->bfs(node.dajRedniBroj());
}

template <typename Label>
void dfs(UsmjereniGraf<Label>* graph, std::vector<Cvor<Label>>& nodes, const Cvor<Label>& node) {
    nodes = graph->dfs(node.dajRedniBroj());
}

int main() {

    return 0;
}
