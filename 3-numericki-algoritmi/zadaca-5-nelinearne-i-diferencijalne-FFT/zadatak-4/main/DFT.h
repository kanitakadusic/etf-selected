#ifndef ZADATAK_4_DFT_H
#define ZADATAK_4_DFT_H

#include <stdexcept>
#include <cmath>
#include <vector>
#include <complex>

std::complex<double> getW(int N) {
    return std::exp(2 * M_PI * std::complex<double>(0, 1) / std::complex<double>(N));
}

void FFT(std::vector<std::complex<double>>& x, std::vector<std::complex<double>>& y, int N, int s = 0, int d = 0, int t = 1) {
    if (N == 1) y[d] = x[s];
    else {
        FFT(x, y, N / 2, s, d, 2 * t);
        FFT(x, y, N / 2, s + t, d + N / 2, 2 * t);

        std::complex<double> m(1), w(std::pow(getW(N), -1)), u, v;

        for (int k = d; k < d + N / 2; k++) {
            u = y[k];
            v = m * y[k + N / 2];
            y[k] = u + v;
            y[k + N / 2] = u - v;
            m *= w;
        }
    }
}

void InverseFFT(std::vector<std::complex<double>>& y, std::vector<std::complex<double>>& x, int N, int s = 0, int d = 0, int t = 1) {
    if (N == 1) x[d] = y[s];
    else {
        InverseFFT(y, x, N / 2, s, d, 2 * t);
        InverseFFT(y, x, N / 2, s + t, d + N / 2, 2 * t);

        std::complex<double> m(1), w(getW(N)), u, v;

        for (int k = d; k < d + N / 2; k++) {
            u = x[k];
            v = m * x[k + N / 2];
            x[k] = (u + v) / 2.;
            x[k + N / 2] = (u - v) / 2.;
            m *= w;
        }
    }
}

bool isPowerOf2(unsigned int x) {
    return x != 0 && (x & (x - 1)) == 0;
}

bool isNatural(double x) {
    return x != 0 && x == static_cast<unsigned int>(x);
}

std::vector<double> LossyCompress(const std::vector<double>& data, int compressedSize) {
    unsigned int size = data.size();

    if (compressedSize < 2 || compressedSize > data.size()) throw std::range_error("Bad new size");
    if (!isPowerOf2(data.size())) throw std::range_error("Data size must be a power of two");

    std::vector<std::complex<double>> complexData(size), complexCompressed(size);

    for (int i = 0; i < size / 2; i++)
        complexData[i] = std::complex<double>(data[2 * i], 0.);

    for (int i = size / 2; i < size; i++)
        complexData[i] = std::complex<double>(data[2 * (size - i) - 1], 0.);

    FFT(complexData, complexCompressed, size);

    std::vector<double> compressed(compressedSize);
    std::complex<double> w(getW(2 * size));

    for (int i = 0; i < compressedSize - 1; i++)
        compressed[i] = (std::pow(w, -i / 2.) * complexCompressed[i]).real();

    compressed.back() = size;

    return compressed;
}

std::vector<double> LossyDecompress(const std::vector<double>& compressed) {
    unsigned int size = compressed.back();

    if (!isNatural(compressed.back()) || !isPowerOf2(compressed.back()) || compressed.back() < compressed.size() - 1)
        throw std::logic_error("Bad compressed sequence");

    std::vector<double> data(compressed);
    data.back() = 0;
    data.resize(size);

    std::vector<std::complex<double>> complexCompressed(size), complexData(size);
    std::complex<double> w(getW(2 * size));

    complexCompressed.front() = data.front();
    for (int i = 1; i < size; i++)
        complexCompressed[i] = 2. * std::pow(w, i / 2.) * data[i];

    InverseFFT(complexCompressed, complexData, size);

    for (int i = 0; i < size; i += 2)
        data[i] = complexData[i / 2].real();

    for (int i = 1; i < size; i += 2)
        data[i] = complexData[size - (i + 1) / 2].real();

    return data;
}

#endif //ZADATAK_4_DFT_H
