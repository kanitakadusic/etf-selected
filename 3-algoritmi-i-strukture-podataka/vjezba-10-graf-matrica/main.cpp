#include <iostream>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <vector>
#include <queue>

using std::cout;
using std::endl;

template <typename Label>
class Cvor;

template <typename Label>
class Grana;

template <typename Label>
class GranaIterator;

template <typename Label>
class UsmjereniGraf;

template <typename Label>
class MatricaGraf;

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

// ==================================================================================================== BEGIN - MATRICA GRAF

template <typename Label>
class MatricaGraf: public UsmjereniGraf<Label> {
    std::vector<std::shared_ptr<Cvor<Label>>> nodes;
    std::vector<std::vector<std::shared_ptr<Grana<Label>>>> edges;

    int getFirstNeighbor(int x) const { return getNextNeighbor(x, -1); }
    int getNextNeighbor(int, int) const;

    bool goToNextEdge(int&, int&) override;

    void bfsComponent(std::vector<Cvor<Label>>&, int, std::vector<bool>&) const override;
    void dfsComponent(std::vector<Cvor<Label>>&, int, std::vector<bool>&) const override;

    using UsmjereniGraf<Label>::checkNode;
    using UsmjereniGraf<Label>::updateFirstEdgeAfterInsertion;
    using UsmjereniGraf<Label>::updateFirstEdgeAfterDeletion;

public:
    MatricaGraf(unsigned int size): UsmjereniGraf<Label>(size), nodes(size), edges(size, std::vector<std::shared_ptr<Grana<Label>>>(size)) {
        for (int i = 0; i < size; i++) nodes[i] = std::make_shared<Cvor<Label>>(i);
    }

    MatricaGraf(const MatricaGraf&) = delete;
    MatricaGraf& operator =(const MatricaGraf&) = delete;

    MatricaGraf(MatricaGraf&&) = delete;
    MatricaGraf& operator =(MatricaGraf&&) = delete;

    ~MatricaGraf() = default;

    // --------------------

    Cvor<Label> dajCvor(int num) const override { checkNode(num); return *nodes[num]; }
    Grana<Label> dajGranu(int, int) const override;

    int dajBrojCvorova() const override { return nodes.size(); }
    void postaviBrojCvorova(unsigned int) override;

    bool postojiGrana(int, int) const override;
    void dodajGranu(int, int, double weight = 0) override;
    void obrisiGranu(int, int) override;
};

template <typename Label>
Grana<Label> MatricaGraf<Label>::dajGranu(int startNum, int endNum) const {
    if (!postojiGrana(startNum, endNum)) throw std::domain_error("MatricaGraf: dajGranu");
    return *edges[startNum][endNum];
}

template <typename Label>
void MatricaGraf<Label>::postaviBrojCvorova(unsigned int size) {
    if (size <= dajBrojCvorova()) throw std::domain_error("MatricaGraf: postaviBrojCvorova");
    int oldSize = dajBrojCvorova();

    nodes.resize(size);
    for (int i = oldSize; i < size; i++)
        nodes[i] = std::make_shared<Cvor<Label>>(i);

    edges.resize(size);
    std::for_each(edges.begin(), edges.end(), [size](std::vector<std::shared_ptr<Grana<Label>>>& row) {
        row.resize(size);
    });
}

template <typename Label>
inline bool MatricaGraf<Label>::postojiGrana(int startNum, int endNum) const {
    checkNode(startNum); checkNode(endNum);
    return edges[startNum][endNum] != nullptr;
}

template <typename Label>
inline void MatricaGraf<Label>::dodajGranu(int startNum, int endNum, double weight) {
    if (!postojiGrana(startNum, endNum)) {
        edges[startNum][endNum] = std::make_shared<Grana<Label>>(nodes[startNum], nodes[endNum], weight);
        updateFirstEdgeAfterInsertion({startNum, endNum});
    }
}

template <typename Label>
inline void MatricaGraf<Label>::obrisiGranu(int startNum, int endNum) {
    if (postojiGrana(startNum, endNum)) {
        edges[startNum][endNum] = nullptr;
        updateFirstEdgeAfterDeletion({startNum, endNum});
    }
}

template <typename Label>
int MatricaGraf<Label>::getNextNeighbor(int x, int a) const {
    for (int i = a + 1; i < dajBrojCvorova(); i++)
        if (edges[x][i] != nullptr) return i;

    return dajBrojCvorova();
}

template <typename Label>
bool MatricaGraf<Label>::goToNextEdge(int& startNum, int& endNum) {
    checkNode(startNum);
    checkNode(endNum);

    do {
        if (++endNum == dajBrojCvorova()) {
            endNum = 0;

            if (++startNum == dajBrojCvorova()) {
                endNum = -1;
                return false;
            }
        }
    } while (edges[startNum][endNum] == nullptr);

    return true;
}

template <typename Label>
void MatricaGraf<Label>::bfsComponent(std::vector<Cvor<Label>>& nodes, int x, std::vector<bool>& visited) const {
    std::queue<int> nums;

    nums.push(x);
    visited[x] = true;

    while (!nums.empty()) {
        x = nums.front();
        nums.pop();

        nodes.push_back(dajCvor(x));

        for (int i = getFirstNeighbor(x); i < dajBrojCvorova(); i = getNextNeighbor(x, i))
            if (!visited[i]) { visited[i] = true; nums.push(i); }
    }
}

template <typename Label>
void MatricaGraf<Label>::dfsComponent(std::vector<Cvor<Label>>& nodes, int x, std::vector<bool>& visited) const {
    nodes.push_back(dajCvor(x));
    visited[x] = true;

    for (int i = getFirstNeighbor(x); i < dajBrojCvorova(); i = getNextNeighbor(x, i))
        if (!visited[i]) dfsComponent(nodes, i, visited);
}

// ==================================================================================================== END - MATRICA GRAF

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
