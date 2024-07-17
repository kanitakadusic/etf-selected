#ifndef ZADATAK_4_DFTTEST_H
#define ZADATAK_4_DFTTEST_H

#include <iostream>
#include <iomanip>
#include <string>

#include "../main/DFT.h"

template <typename F>
void Compare(const std::vector<double>& samples, F f, int compressedSize) {
    std::vector<double> x(samples.size());
    for (int i = 0; i < samples.size(); i++)
        x[i] = f(samples[i]);

    std::vector<double> y(LossyDecompress(LossyCompress(x, compressedSize)));

    std::cout << '\n' << std::setw(15) << "ACCURATE" << std::setw(15) << "TESTED";
    std::cout << '\n' << std::string(40, '-') << std::endl;

    for (int i = 0; i < samples.size(); i++)
        std::cout << std::setw(15) << x[i] << std::setw(15) << y[i] << '\n';
}

void LossyComparison() {
    std::vector<double> samples;
    for (double i = 0; i < 2 * M_PI; i += M_PI / 8)
        samples.push_back(i);

    Compare(samples, (double(*)(double))(std::sin), 4);
    Compare(samples, (double(*)(double))(std::cos), 4);
}

void LossyCompressTest() {
    try { LossyCompress({1, 1}, 1); }
    catch (...) {
        try { LossyCompress({1, 1}, 3); }
        catch (...) {
            try { LossyCompress({1, 1, 1}, 2); }
            catch (...) {
                return;
            }
        }
    }

    throw std::logic_error("LossyCompressTest: Validation failed");
}

void LossyDecompressTest() {
    try { LossyDecompress({1, 1, 1, 4.1}); }
    catch (...) {
        try { LossyDecompress({1, 1, 1, 5}); }
        catch (...) {
            try { LossyDecompress({1, 1, 1, 2}); }
            catch (...) {
                return;
            }
        }
    }

    throw std::logic_error("LossyDecompressTest: Validation failed");
}

#endif //ZADATAK_4_DFTTEST_H
