#ifndef ZADATAK_QRDECOMPOSER_H
#define ZADATAK_QRDECOMPOSER_H

#include "Matrix.h"

class QRDecomposer {
    Matrix QR;
    Vector D;

public:
    QRDecomposer(Matrix);

    // --------------------

    Vector Solve(Vector) const;
    void Solve(const Vector& b, Vector& x) const {
        if (x.NElems() != this->QR.NRows())
            throw std::domain_error("Incompatible formats");

        x = Solve(b);
    }

    Matrix Solve(Matrix) const;
    void Solve(const Matrix& B, Matrix& X) const {
        if (X.NRows() != this->QR.NRows() || X.NCols() != B.NCols())
            throw std::domain_error("Incompatible formats");

        X = Solve(B);
    }

    Vector MulQWith(Vector) const;
    Matrix MulQWith(Matrix) const;

    Vector MulQTWith(Vector) const;
    Matrix MulQTWith(Matrix) const;

    Matrix GetQ() const;
    Matrix GetR() const;
};

// --------------------

QRDecomposer::QRDecomposer(Matrix matrix): QR(1, 1), D(matrix.NCols()) {
    if (matrix.NRows() < matrix.NCols()) throw std::domain_error("Invalid matrix format");

    double eps = matrix.GetEpsilon();

    for (int k = 1; k <= matrix.NCols(); k++) {
        double s = 0.;

        for (int i = k; i <= matrix.NRows(); i++)
            s += matrix[i - 1][k - 1] * matrix[i - 1][k - 1];

        s = std::sqrt(s);
        double u = std::sqrt(s * (s + std::fabs(matrix[k - 1][k - 1])));

        if (std::fabs(u) < eps)
            throw std::domain_error("Matrix is singular");

        if (Comparator::isLess(matrix[k - 1][k - 1], 0)) s = -s;
        matrix[k - 1][k - 1] = (matrix[k - 1][k - 1] + s) / u;

        for (int i = k + 1; i <= matrix.NRows(); i++)
            matrix[i - 1][k - 1] = matrix[i - 1][k - 1] / u;

        D[k - 1] = -s;

        for (int j = k + 1; j <= matrix.NCols(); j++) {
            s = 0.;

            for (int i = k; i <= matrix.NRows(); i++)
                s += matrix[i - 1][k - 1] * matrix[i - 1][j - 1];

            for (int i = k; i <= matrix.NRows(); i++)
                matrix[i - 1][j - 1] -= s * matrix[i - 1][k - 1];
        }
    }

    this->QR = std::move(matrix);
}

Vector QRDecomposer::Solve(Vector b) const {
    if (this->QR.NRows() != this->QR.NCols()) throw std::domain_error("Matrix is not square");
    if (this->QR.NRows() != b.NElems()) throw std::domain_error("Incompatible formats");

    Matrix R(this->GetR());
    b = this->MulQTWith(b);

    Vector x(R.NRows());

    for (int i = x.NElems(); i >= 1; i--) {
        double s = b[i - 1];

        for (int j = i + 1; j <= x.NElems(); j++)
            s -= R[i - 1][j - 1] * x[j - 1];

        x[i - 1] = s / R[i - 1][i - 1];
    }

    return x;
}

Matrix QRDecomposer::Solve(Matrix B) const {
    if (this->QR.NRows() != this->QR.NCols()) throw std::domain_error("Matrix is not square");
    if (this->QR.NRows() != B.NRows()) throw std::domain_error("Incompatible formats");

    Matrix R(this->GetR());
    B = this->MulQTWith(B);

    Matrix X(R.NRows(), B.NCols());

    for (int k = 1; k <= X.NCols(); k++) {
        for (int i = X.NRows(); i >= 1; i--) {
            double s = B[i - 1][k - 1];

            for (int j = i + 1; j <= X.NRows(); j++)
                s -= R[i - 1][j - 1] * X[j - 1][k - 1];

            X[i - 1][k - 1] = s / R[i - 1][i - 1];
        }
    }

    return X;
}

Vector QRDecomposer::MulQWith(Vector v) const {
    if (this->QR.NCols() != v.NElems()) throw std::domain_error("Incompatible formats");

    for (int k = this->QR.NCols(); k >= 1; k--) {
        double s = 0.;

        for (int i = k; i <= this->QR.NRows(); i++)
            s += this->QR[i - 1][k - 1] * v[i - 1];

        for (int i = k; i <= this->QR.NRows(); i++)
            v[i - 1] -= s * this->QR[i - 1][k - 1];
    }

    return v;
}

Matrix QRDecomposer::MulQWith(Matrix M) const {
    if (this->QR.NCols() != M.NRows()) throw std::domain_error("Incompatible formats");

    for (int j = 0; j < M.NCols(); j++) {
        Vector v(M.NRows());

        for (int i = 0; i < M.NRows(); i++)
            v[i] = M[i][j];

        v = MulQWith(v);

        for (int i = 0; i < M.NRows(); i++)
            M[i][j] = v[i];
    }

    return M;
}

Vector QRDecomposer::MulQTWith(Vector v) const {
    if (this->QR.NRows() != v.NElems()) throw std::domain_error("Incompatible formats");

    for (int k = 1; k <= this->QR.NRows(); k++) {
        double s = 0.;

        for (int i = k; i <= this->QR.NRows(); i++)
            s += this->QR[i - 1][k - 1] * v[i - 1];

        for (int i = k; i <= this->QR.NRows(); i++)
            v[i - 1] -= s * this->QR[i - 1][k - 1];
    }

    return v;
}

Matrix QRDecomposer::MulQTWith(Matrix M) const {
    if (this->QR.NRows() != M.NRows()) throw std::domain_error("Incompatible formats");

    for (int j = 0; j < M.NCols(); j++) {
        Vector v(M.NRows());

        for (int i = 0; i < M.NRows(); i++)
            v[i] = M[i][j];

        v = MulQTWith(v);

        for (int i = 0; i < M.NRows(); i++)
            M[i][j] = v[i];
    }

    return M;
}

Matrix QRDecomposer::GetQ() const {
    Matrix Q(this->QR.NRows(), this->QR.NRows());

    for (int j = 1; j <= this->QR.NRows(); j++) {
        for (int i = 1; i <= this->QR.NRows(); i++)
            Q[i - 1][j - 1] = 0.;

        Q[j - 1][j - 1] = 1.;

        for (int k = this->QR.NCols(); k >= 1; k--) {
            double s = 0.;

            for (int i = k; i <= this->QR.NRows(); i++)
                s += this->QR[i - 1][k - 1] * Q[i - 1][j - 1];

            for (int i = k; i <= this->QR.NRows(); i++)
                Q[i - 1][j - 1] -= s * this->QR[i - 1][k - 1];
        }
    }

    return Q;
}

Matrix QRDecomposer::GetR() const {
    Matrix R(this->QR.NRows(), this->QR.NCols());

    for (int i = 0; i < R.NRows(); i++)
        for (int j = i; j < R.NCols(); j++)
            R[i][j] = (i == j ? this->D[i] : this->QR[i][j]);

    return R;
}

#endif //ZADATAK_QRDECOMPOSER_H
