#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <string>
#include <array>
#include <stdexcept>

using namespace std;

void radixSort(std::vector<int>& array) {
    if (array.empty()) return;

    std::array<std::queue<int>, 10> radix;
    int maxDigits = std::to_string(*std::max_element(array.begin(), array.end())).size();

    for (int i = 0, d = 1; i < maxDigits; i++, d *= 10) {
        for (int x : array) radix[(x / d) % 10].push(x);

        for (int a = 0, r = 0; r < radix.size(); r++)
            while (!radix[r].empty()) {
                array[a++] = radix[r].front();
                radix[r].pop();
            }
    }
}

void fixDownwards(std::vector<int>& heap, int size, int i) {
    if (size < 0 || size > heap.size() || i < 0 || i >= size)
        throw std::domain_error("Invalid argument");

    while (i < size / 2) {
        int R = 2 * i + 2, L = 2 * i + 1;
        int x = R < size && heap[R] > heap[L] ? R : L;

        if (heap[i] > heap[x]) return;

        std::swap(heap[i], heap[x]);
        i = x;
    }
}

void fixUpwards(std::vector<int>& heap, int i) {
    if (i < 0 || i >= heap.size())
        throw std::domain_error("Invalid argument");

    while (i != 0 && heap[i] > heap[(i - 1) / 2]) {
        std::swap(heap[i], heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void stvoriGomilu(std::vector<int>& array) {
    for (int i = array.size() / 2 - 1; i >= 0; i--)
        fixDownwards(array, array.size(), i);
}

void umetniUGomilu(std::vector<int>& heap, int x, int& size) {
    if (size < 0 || size > heap.size())
        throw std::domain_error("Invalid argument");

    if (size == heap.size()) heap.push_back(x);
    else heap[size] = x;

    fixUpwards(heap, size++);
}

int izbaciPrvi(std::vector<int>& heap, int& size) {
    if (size < 0 || size > heap.size())
        throw std::domain_error("Invalid argument");

    std::swap(heap[0], heap[--size]);
    if (size != 0) fixDownwards(heap, size, 0);

    return heap[size];
}

void gomilaSort(std::vector<int>& array) {
    stvoriGomilu(array);
    int size = array.size();

    for (int i = 0; i < array.size() - 1; i++)
        izbaciPrvi(array, size);
}

int main() {

    return 0;
}