#ifndef ZADATAK_LUDECOMPOSER_H
#define ZADATAK_LUDECOMPOSER_H

#include "Matrix.h"

class LUDecomposer {
    Matrix LU;
    Vector P;

public:
    LUDecomposer(Matrix);

    // --------------------

    Vector Solve(Vector) const;
    void Solve(const Vector& b, Vector& x) const {
        if (x.NElems() != this->LU.NRows())
            throw std::domain_error("Incompatible formats");

        x = Solve(b);
    }

    Matrix Solve(Matrix) const;
    void Solve(const Matrix& B, Matrix& X) const {
        if (B.NRows() != X.NRows() || X.NRows() != this->LU.NRows() || X.NCols() != B.NCols())
            throw std::domain_error("Incompatible formats");

        X = Solve(B);
    }

    Matrix GetCompactLU() const { return this->LU; }

    Matrix GetL() const;
    Matrix GetU() const;

    Vector GetPermuation() const { return this->P; }
};

// --------------------

LUDecomposer::LUDecomposer(Matrix matrix): LU(1, 1), P(matrix.NRows()) {
    if (matrix.NRows() != matrix.NCols()) throw std::domain_error("Matrix is not square");

    double eps = matrix.GetEpsilon();

    for (int j = 1; j <= matrix.NRows(); j++) {
        for (int i = 1; i <= j; i++) {
            double s = matrix[i - 1][j - 1];

            for (int k = 1; k <= i - 1; k++)
                s -= matrix[i - 1][k - 1] * matrix[k - 1][j - 1];

            matrix[i - 1][j - 1] = s;
        }

        int p = j;

        for (int i = j + 1; i <= matrix.NRows(); i++) {
            double s = matrix[i - 1][j - 1];

            for (int k = 1; k <= j - 1; k++)
                s -= matrix[i - 1][k - 1] * matrix[k - 1][j - 1];

            matrix[i - 1][j - 1] = s;

            if (Comparator::isGreater(std::fabs(s), std::fabs(matrix[p - 1][j - 1]))) p = i;
        }

        if (std::fabs(matrix[p - 1][j - 1]) < eps)
            throw std::domain_error("Matrix is singular");

        if (p != j)
            std::swap_ranges(matrix[j - 1], matrix[j - 1] + matrix.NCols(), matrix[p - 1]);

        P[j - 1] = p;
        double u = matrix[j - 1][j - 1];

        for (int i = j + 1; i <= matrix.NRows(); i++)
            matrix[i - 1][j - 1] /= u;
    }

    this->LU = std::move(matrix);
}

Vector LUDecomposer::Solve(Vector b) const {
    if (b.NElems() != this->LU.NRows()) throw std::domain_error("Incompatible formats");

    Vector x(this->LU.NRows()), y(this->LU.NRows());

    for (int i = 1; i <= y.NElems(); i++) {
        double p = this->P[i - 1], s = b[p - 1];
        b[p - 1] = b[i - 1];

        for (int j = 1; j <= i - 1; j++)
            s -= this->LU[i - 1][j - 1] * y[j - 1];

        y[i - 1] = s;
    }

    for (int i = x.NElems(); i >= 1; i--) {
        double s = y[i - 1];

        for (int j = i + 1; j <= x.NElems(); j++)
            s -= this->LU[i - 1][j - 1] * x[j - 1];

        x[i - 1] = s / this->LU[i - 1][i - 1];
    }

    return x;
}

Matrix LUDecomposer::Solve(Matrix B) const {
    if (this->LU.NCols() != B.NRows()) throw std::domain_error("Incompatible formats");

    Matrix X(this->LU.NRows(), B.NCols()), Y(this->LU.NRows(), B.NCols());

    for (int k = 1; k <= B.NCols(); k++)
        for (int i = 1; i <= this->LU.NRows(); i++) {
            double p = this->P[i - 1], s = B[p - 1][k - 1];
            B[p - 1][k - 1] = B[i - 1][k - 1];

            for (int j = 1; j <= i - 1; j++)
                s -= this->LU[i - 1][j - 1] * Y[j - 1][k - 1];

            Y[i - 1][k - 1] = s;
        }

    for (int k = 1; k <= B.NCols(); k++)
        for (int i = this->LU.NRows(); i >= 1; i--) {
            double s = Y[i - 1][k - 1];

            for (int j = i + 1; j <= this->LU.NRows(); j++)
                s -= this->LU[i - 1][j - 1] * X[j - 1][k - 1];

            X[i - 1][k - 1] = s / this->LU[i - 1][i - 1];
        }

    return X;
}

Matrix LUDecomposer::GetL() const {
    Matrix L(this->LU.NRows(), this->LU.NCols());

    for (int i = 0; i < L.NRows(); i++) {
        L[i][i] = 1.;

        for (int j = 0; j < i; j++)
            L[i][j] = this->LU[i][j];
    }

    return L;
}

Matrix LUDecomposer::GetU() const {
    Matrix U(this->LU.NRows(), this->LU.NCols());

    for (int i = 0; i < U.NRows(); i++)
        for (int j = i; j < U.NCols(); j++)
            U[i][j] = this->LU[i][j];

    return U;
}

#endif //ZADATAK_LUDECOMPOSER_H
