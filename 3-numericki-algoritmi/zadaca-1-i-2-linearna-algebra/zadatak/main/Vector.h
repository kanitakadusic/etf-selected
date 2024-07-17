#ifndef ZADATAK_VECTOR_H
#define ZADATAK_VECTOR_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <limits>
#include <algorithm>

#include "Comparator.h"

class Vector {
    std::vector<double> my_vector;

    void IndexCheck(int i) const {
        if (i < 1 || i > this->NElems()) throw std::range_error("Invalid index");
    }

public:
    explicit Vector(int size): my_vector(size < 0 ? 0 : size) { if (size <= 0) throw std::range_error("Bad dimension"); }
    Vector(std::initializer_list<double> vector): my_vector(vector) { if (vector.size() == 0) throw std::range_error("Bad dimension"); }

    // --------------------

    int NElems() const { return this->my_vector.size(); }

    double& operator[](int i) { return this->my_vector[i]; }
    double operator[](int i) const { return this->my_vector[i]; }

    double& operator()(int i) { this->IndexCheck(i); return this->my_vector[i - 1]; }
    double operator()(int i) const { this->IndexCheck(i); return this->my_vector[i - 1]; }

    double Norm() const;
    friend double VectorNorm(const Vector& vector) { return vector.Norm(); }

    double GetEpsilon() const { return 10 * this->Norm() * std::numeric_limits<double>::epsilon(); }

    void Print(char separator = '\n', double epsilon = -1) const;
    friend void PrintVector(const Vector& vector, char separator = '\n', double epsilon = -1) { vector.Print(separator, epsilon); }

    Vector& operator +=(const Vector&);
    friend Vector operator +(Vector first, const Vector& second) { return first += second; }

    Vector& operator -=(const Vector&);
    friend Vector operator -(Vector first, const Vector& second) { return first -= second; }

    Vector& operator *=(double);
    friend Vector operator *(double scalar, Vector vector) { return vector *= scalar; }
    friend Vector operator *(Vector vector, double scalar) { return vector *= scalar; }

    friend double operator *(const Vector&, const Vector&);

    Vector& operator /=(double);
    friend Vector operator /(Vector vector, double scalar) { return vector /= scalar; }

    void Chop(double eps = -1);
    bool EqualTo(const Vector&, double eps = -1) const;
};

// --------------------

double Vector::Norm() const {
    double sum = 0;

    for (int i = 0; i < this->NElems(); i++)
        sum += (*this)[i] * (*this)[i];

    return std::sqrt(sum);
}

void Vector::Print(char separator, double epsilon) const {
    if (epsilon < 0) epsilon = this->GetEpsilon();

    for (int i = 0; i < this->NElems() - 1; i++)
        std::cout << (std::fabs((*this)[i]) < epsilon ? 0 : (*this)[i]) << separator;

    std::cout << (*this)[this->NElems() - 1];
    if (separator == '\n') std::cout << separator;
}

Vector& Vector::operator +=(const Vector& vector) {
    if (vector.NElems() != this->NElems())
        throw std::domain_error("Incompatible formats");

    for (int i = 0; i < this->NElems(); i++)
        (*this)[i] += vector[i];

    return *this;
}

Vector& Vector::operator -=(const Vector& vector) {
    if (vector.NElems() != this->NElems())
        throw std::domain_error("Incompatible formats");

    for (int i = 0; i < this->NElems(); i++)
        (*this)[i] -= vector[i];

    return *this;
}

Vector& Vector::operator *=(double scalar) {
    for (int i = 0; i < this->NElems(); i++)
        (*this)[i] *= scalar;

    return *this;
}

double operator *(const Vector& first, const Vector& second) {
    if (first.NElems() != second.NElems())
        throw std::domain_error("Incompatible formats");

    double scalar = 0;
    for (int i = 0; i < first.NElems(); i++)
        scalar += first[i] * second[i];

    return scalar;
}

Vector& Vector::operator /=(double scalar) {
    if (Comparator::isEqual(scalar, 0))
        throw std::domain_error("Division by zero");

    for (int i = 0; i < this->NElems(); i++)
        (*this)[i] /= scalar;

    return *this;
}

void Vector::Chop(double eps) {
    if (eps < 0) eps = this->GetEpsilon();

    std::replace_if(this->my_vector.begin(), this->my_vector.end(), [eps](double element) {
        return std::fabs(element) < eps; }, 0.);
}

bool Vector::EqualTo(const Vector& vector, double eps) const {
    if (vector.NElems() != this->NElems()) return false;
    if (eps < 0) eps = this->GetEpsilon();

    for (int i = 0; i < this->NElems(); i++)
        if (std::fabs(this->my_vector[i] - vector[i]) > eps)
            return false;

    return true;
}

#endif //ZADATAK_VECTOR_H
