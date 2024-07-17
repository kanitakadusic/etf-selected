#ifndef ZADATAK_MATRIX_H
#define ZADATAK_MATRIX_H

#include "Vector.h"

class Matrix {
    std::vector<std::vector<double>> my_matrix;

    void IndexCheck(int i, int j) const {
        if (i < 1 || i > this->NRows() || j < 1 || j > this->NCols()) throw std::range_error("Invalid index");
    }

    int RankRREF();

public:
    Matrix(int, int);
    Matrix(const Vector&);
    Matrix(std::initializer_list<std::vector<double>>);

    // --------------------

    int NRows() const { return this->my_matrix.size(); }
    int NCols() const { return this->my_matrix[0].size(); }

    double* operator [](int i) { return &this->my_matrix[i][0]; }
    const double* operator [](int i) const { return &this->my_matrix[i][0]; }

    double& operator ()(int i, int j) { this->IndexCheck(i, j); return this->my_matrix[i - 1][j - 1]; }
    double operator ()(int i, int j) const { this->IndexCheck(i, j); return this->my_matrix[i - 1][j - 1]; }

    double Norm() const;
    friend double MatrixNorm(const Matrix& matrix) { return matrix.Norm(); }

    double GetEpsilon() const { return 10 * this->Norm() * std::numeric_limits<double>::epsilon(); }

    void Print(int width = 10, double epsilon = -1) const;
    friend void PrintMatrix(const Matrix& matrix, int width = 10, double epsilon = -1) { matrix.Print(width, epsilon); }

    Matrix& operator +=(const Matrix&);
    friend Matrix operator +(Matrix first, const Matrix& second) { return first += second; }

    Matrix& operator -=(const Matrix&);
    friend Matrix operator -(Matrix first, const Matrix& second) { return first -= second; }

    Matrix& operator *=(double);
    friend Matrix operator *(double scalar, Matrix matrix) { return matrix *= scalar; }
    friend Matrix operator *(Matrix matrix, double scalar) { return matrix *= scalar; }

    Matrix& operator *=(const Matrix&);
    friend Matrix operator *(const Matrix&, const Matrix&);
    friend Vector operator *(const Matrix&, const Vector&);

    void Transpose();
    friend Matrix Transpose(const Matrix&);

    void Chop(double eps = -1);
    bool EqualTo(const Matrix&, double eps = -1) const;

    friend Matrix LeftDiv(Matrix, Matrix);
    friend Vector LeftDiv(Matrix, Vector);

    Matrix& operator /=(Matrix);
    friend Matrix operator /(Matrix A, Matrix B) { return A /= std::move(B); }

    Matrix& operator /=(double);
    friend Matrix operator /(Matrix matrix, double scalar) { return matrix /= scalar; }

    double Det() const;
    friend double Det(const Matrix& matrix) { return matrix.Det(); }

    void Invert();
    friend Matrix Inverse(Matrix matrix) { matrix.Invert(); return matrix; }

    void ReduceToRREF() { this->RankRREF(); }
    friend Matrix RREF(Matrix matrix) { matrix.ReduceToRREF(); return matrix; }

    int Rank() const;
    friend int Rank(const Matrix& matrix) { return matrix.Rank(); }
};

// --------------------

Matrix::Matrix(int rows, int columns): my_matrix(rows < 0 ? 0 : rows, std::vector<double>(columns < 0 ? 0 : columns)) {
    if (rows <= 0 || columns <= 0) throw std::range_error("Bad dimension");
}

Matrix::Matrix(const Vector& vector): my_matrix(vector.NElems(), std::vector<double>(1)) {
    for (int i = 0; i < vector.NElems(); i++)
        this->my_matrix[i][0] = vector[i];
}

Matrix::Matrix(std::initializer_list<std::vector<double>> matrix): my_matrix(matrix.size()) {
    if (matrix.size() == 0) throw std::range_error("Bad dimension");

    auto row = matrix.begin();
    unsigned int first_row_length = row->size();

    for (int i = 0; i < matrix.size(); i++) {
        if (row->empty()) throw std::range_error("Bad dimension");
        if (row->size() != first_row_length) throw std::logic_error("Bad matrix");

        this->my_matrix[i] = *row++;
    }
}

double Matrix::Norm() const {
    double sum = 0;

    for (int i = 0; i < this->NRows(); i++)
        for (int j = 0; j < this->NCols(); j++)
            sum += (*this)[i][j] * (*this)[i][j];

    return std::sqrt(sum);
}

void Matrix::Print(int width, double epsilon) const {
    if (epsilon < 0) epsilon = this->GetEpsilon();

    for (int i = 0; i < this->NRows(); i++) {
        for (int j = 0; j < this->NCols(); j++) {
            std::cout.width((*this)[i][j] < 0 ? width + 1 : width);
            std::cout << (std::fabs((*this)[i][j]) < epsilon ? 0 : (*this)[i][j]);
        }

        std::cout << std::endl;
    }
}

Matrix& Matrix::operator +=(const Matrix& matrix) {
    if (matrix.NRows() != this->NRows() || matrix.NCols() != this->NCols())
        throw std::domain_error("Incompatible formats");

    for (int i = 0; i < this->NRows(); i++)
        for (int j = 0; j < this->NCols(); j++)
            (*this)[i][j] += matrix[i][j];

    return *this;
}

Matrix& Matrix::operator -=(const Matrix& matrix) {
    if (matrix.NRows() != this->NRows() || matrix.NCols() != this->NCols())
        throw std::domain_error("Incompatible formats");

    for (int i = 0; i < this->NRows(); i++)
        for (int j = 0; j < this->NCols(); j++)
            (*this)[i][j] -= matrix[i][j];

    return *this;
}

Matrix& Matrix::operator *=(double scalar) {
    for (int i = 0; i < this->NRows(); i++)
        for (int j = 0; j < this->NCols(); j++)
            (*this)[i][j] *= scalar;

    return *this;
}

Matrix& Matrix::operator *=(const Matrix& matrix) {
    if (this->NCols() != matrix.NRows())
        throw std::domain_error("Incompatible formats");

    Matrix product(this->NRows(), matrix.NCols());
    for (int i = 0; i < this->NRows(); i++)
        for (int j = 0; j < matrix.NCols(); j++)
            for (int k = 0; k < this->NCols(); k++)
                product[i][j] += (*this)[i][k] * matrix[k][j];

    *this = std::move(product);
    return *this;
}

Matrix operator *(const Matrix& first, const Matrix& second) {
    if (first.NCols() != second.NRows())
        throw std::domain_error("Incompatible formats");

    Matrix product(first.NRows(), second.NCols());
    for (int i = 0; i < first.NRows(); i++)
        for (int j = 0; j < second.NCols(); j++)
            for (int k = 0; k < first.NCols(); k++)
                product[i][j] += first[i][k] * second[k][j];

    return product;
}

Vector operator *(const Matrix& matrix, const Vector& vector) {
    if (matrix.NCols() != vector.NElems())
        throw std::domain_error("Incompatible formats");

    Vector product(matrix.NRows());
    for (int i = 0; i < matrix.NRows(); i++)
        for (int j = 0; j < matrix.NCols(); j++)
            product[i] += matrix[i][j] * vector[j];

    return product;
}

void Matrix::Transpose() {
    if (this->NRows() == this->NCols()) {
        for (int i = 0; i < this->NRows(); i++)
            for (int j = i + 1; j < this->NRows(); j++)
                std::swap((*this)[i][j], (*this)[j][i]);
    } else {
        Matrix transpose(this->NCols(), this->NRows());

        for (int i = 0; i < this->NRows(); i++)
            for (int j = 0; j < this->NCols(); j++)
                transpose[j][i] = (*this)[i][j];

        *this = std::move(transpose);
    }
}

Matrix Transpose(const Matrix& matrix) {
    Matrix transpose(matrix.NCols(), matrix.NRows());

    for (int i = 0; i < matrix.NRows(); i++)
        for (int j = 0; j < matrix.NCols(); j++)
            transpose[j][i] = matrix[i][j];

    return transpose;
}

void Matrix::Chop(double eps) {
    if (eps < 0) eps = this->GetEpsilon();

    for (int i = 0; i < this->NRows(); i++)
        for (int j = 0; j < this->NCols(); j++)
            if (std::fabs(this->my_matrix[i][j]) < eps)
                this->my_matrix[i][j] = 0.;
}

bool Matrix::EqualTo(const Matrix& matrix, double eps) const {
    if (matrix.NRows() != this->NRows() || matrix.NCols() != this->NCols()) return false;
    if (eps < 0) eps = this->GetEpsilon();

    for (int i = 0; i < this->NRows(); i++)
        for (int j = 0; j < this->NCols(); j++)
            if (std::fabs(this->my_matrix[i][j] - matrix[i][j]) > eps)
                return false;

    return true;
}

Matrix LeftDiv(Matrix A, Matrix B) {
    if (A.NRows() != A.NCols()) throw std::domain_error("Divisor matrix is not square");
    if (B.NRows() != A.NRows()) throw std::domain_error("Incompatible formats");

    int n = A.NRows(), m = B.NCols(), p;
    double u, eps = A.GetEpsilon();

    for (int k = 1; k <= n; k++) {
        p = k;

        for (int i = k + 1; i <= n; i++)
            if (Comparator::isGreater(std::fabs(A[i - 1][k - 1]), std::fabs(A[p - 1][k - 1])))
                p = i;

        if (std::fabs(A[p - 1][k - 1]) < eps)
            throw std::domain_error("Divisor matrix is singular");

        if (p != k) {
            std::swap_ranges(A[k - 1], A[k - 1] + n, A[p - 1]);
            std::swap_ranges(B[k - 1], B[k - 1] + m, B[p - 1]);
        }

        for (int i = k + 1; i <= n; i++) {
            u = A[i - 1][k - 1] / A[k - 1][k - 1];

            for (int j = k + 1; j <= n; j++)
                A[i - 1][j - 1] -= u * A[k - 1][j - 1];

            for (int j = 1; j <= m; j++)
                B[i - 1][j - 1] -= u * B[k - 1][j - 1];
        }
    }

    Matrix X(n, m);

    for (int k = 1; k <= m; k++)
        for (int i = n; i >= 1; i--) {
            u = B[i - 1][k - 1];

            for (int j = i + 1; j <= n; j++)
                u -= A[i - 1][j - 1] * X[j - 1][k - 1];

            X[i - 1][k - 1] = u / A[i - 1][i - 1];
        }

    return X;
}

Vector LeftDiv(Matrix A, Vector B) {
    if (A.NRows() != A.NCols()) throw std::domain_error("Divisor matrix is not square");
    if (B.NElems() != A.NRows()) throw std::domain_error("Incompatible formats");

    int n = A.NRows(), p;
    double u, eps = A.GetEpsilon();

    for (int k = 1; k <= n; k++) {
        p = k;

        for (int i = k + 1; i <= n; i++)
            if (Comparator::isGreater(std::fabs(A[i - 1][k - 1]), std::fabs(A[p - 1][k - 1])))
                p = i;

        if (std::fabs(A[p - 1][k - 1]) < eps)
            throw std::domain_error("Divisor matrix is singular");

        if (p != k) {
            std::swap_ranges(A[k - 1], A[k - 1] + n, A[p - 1]);
            std::swap(B[k - 1], B[p - 1]);
        }

        for (int i = k + 1; i <= n; i++) {
            u = A[i - 1][k - 1] / A[k - 1][k - 1];

            for (int j = k + 1; j <= n; j++)
                A[i - 1][j - 1] -= u * A[k - 1][j - 1];

            B[i - 1] -= u * B[k - 1];
        }
    }

    Vector X(n);

    for (int i = n; i >= 1; i--) {
        u = B[i - 1];

        for (int j = i + 1; j <= n; j++)
            u -= A[i - 1][j - 1] * X[j - 1];

        X[i - 1] = u / A[i - 1][i - 1];
    }

    return X;
}

Matrix& Matrix::operator /=(Matrix A) {
    if (A.NRows() != A.NCols()) throw std::domain_error("Divisor matrix is not square");
    if (this->NCols() != A.NRows()) throw std::domain_error("Incompatible formats");

    int n = A.NRows(), m = this->NRows(), p;
    double u, eps = A.GetEpsilon();

    for (int k = 1; k <= n; k++) {
        p = k;

        for (int i = k + 1; i <= n; i++)
            if (Comparator::isGreater(std::fabs(A[k - 1][i - 1]), std::fabs(A[k - 1][p - 1])))
                p = i;

        if (std::fabs(A[k - 1][p - 1]) < eps)
            throw std::domain_error("Divisor matrix is singular");

        if (p != k) {
            for (int j = 1; j <= n; j++)
                std::swap(A[j - 1][k - 1], A[j - 1][p - 1]);

            for (int j = 1; j <= m; j++)
                std::swap(this->my_matrix[j - 1][k - 1], this->my_matrix[j - 1][p - 1]);
        }

        for (int i = k + 1; i <= n; i++) {
            u = A[k - 1][i - 1] / A[k - 1][k - 1];

            for (int j = k + 1; j <= n; j++)
                A[j - 1][i - 1] -= u * A[j - 1][k - 1];

            for (int j = 1; j <= m; j++)
                this->my_matrix[j - 1][i - 1] -= u * this->my_matrix[j - 1][k - 1];
        }
    }

    Matrix X(m, n);

    for (int k = 1; k <= m; k++)
        for (int i = n; i >= 1; i--) {
            u = this->my_matrix[k - 1][i - 1];

            for (int j = i + 1; j <= n; j++)
                u -= A[j - 1][i - 1] * X[k - 1][j - 1];

            X[k - 1][i - 1] = u / A[i - 1][i - 1];
        }

    return *this = std::move(X);
}

Matrix& Matrix::operator /=(double scalar) {
    if (Comparator::isEqual(scalar, 0)) throw std::domain_error("Division by zero");

    for (int i = 0; i < this->NRows(); i++)
        for (int j = 0; j < this->NCols(); j++)
            this->my_matrix[i][j] /= scalar;

    return *this;
}

double Matrix::Det() const {
    if (this->NRows() != this->NCols()) throw std::domain_error("Matrix is not square");

    double d = 1., eps = this->GetEpsilon(), u;
    int n = this->NRows(), p;

    auto A(this->my_matrix);

    for (int k = 1; k <= n; k++) {
        p = k;

        for (int i = k + 1; i <= n; i++)
            if (Comparator::isGreater(std::fabs(A[i - 1][k - 1]), std::fabs(A[p - 1][k - 1])))
                p = i;

        if (std::fabs(A[p - 1][k - 1]) < eps) return 0;

        if (p != k) {
            std::swap(A[k - 1], A[p - 1]);
            d = -d;
        }

        d *= A[k - 1][k - 1];

        for (int i = k + 1; i <= n; i++) {
            u = A[i - 1][k - 1] / A[k - 1][k - 1];

            for (int j = k + 1; j <= n; j++)
                A[i - 1][j - 1] -= u * A[k - 1][j - 1];
        }
    }

    return d;
}

void Matrix::Invert() {
    if (this->NRows() != this->NCols()) throw std::domain_error("Matrix is not square");

    double eps = this->GetEpsilon(), u;
    int n = this->NRows(), p;

    std::vector<int> W(n);

    for (int k = 1; k <= n; k++) {
        p = k;

        for (int i = k + 1; i <= n; i++)
            if (Comparator::isGreater(std::fabs(this->my_matrix[i - 1][k - 1]), std::fabs(this->my_matrix[p - 1][k - 1])))
                p = i;

        if (std::fabs(this->my_matrix[p - 1][k - 1]) < eps)
            throw std::domain_error("Matrix is singular");

        if (p != k) std::swap(this->my_matrix[k - 1], this->my_matrix[p - 1]);

        W[k - 1] = p;
        u = this->my_matrix[k - 1][k - 1];
        this->my_matrix[k - 1][k - 1] = 1;

        for (int j = 1; j <= n; j++)
            this->my_matrix[k - 1][j - 1] /= u;

        for (int i = 1; i <= n; i++)
            if (i != k) {
                u = this->my_matrix[i - 1][k - 1];
                this->my_matrix[i - 1][k - 1] = 0;

                for (int j = 1; j <= n; j++)
                    this->my_matrix[i - 1][j - 1] -= u * this->my_matrix[k - 1][j - 1];
            }
    }

    for (int j = n; j >= 1; j--) {
        p = W[j - 1];

        if (p != j)
            for (int i = 1; i <= n; i++)
                std::swap(this->my_matrix[i - 1][p - 1], this->my_matrix[i - 1][j - 1]);
    }
}

int Matrix::RankRREF() {
    int k = 0, l = 0, p = 0;
    int m = this->NRows(), n = this->NCols();
    double v, eps = this->GetEpsilon(), u;

    while (k <= m && l <= n) {
        l++;
        k++;

        v = 0.;

        while (v < eps && l <= n) {
            p = k;

            for (int i = k; i <= m; i++)
                if (Comparator::isGreater(std::fabs(this->my_matrix[i - 1][l - 1]), v)) {
                    v = std::fabs(this->my_matrix[i - 1][l - 1]);
                    p = i;
                }

            if (v < eps) l++;
        }

        if (l <= n) {
            if (p != k) std::swap(this->my_matrix[k - 1], this->my_matrix[p - 1]);

            u = this->my_matrix[k - 1][l - 1];

            for (int j = l; j <= n; j++)
                this->my_matrix[k - 1][j - 1] /= u;

            for (int i = 1; i <= m; i++)
                if (i != k) {
                    u = this->my_matrix[i - 1][l - 1];

                    for (int j = l; j <= n; j++)
                        this->my_matrix[i - 1][j - 1] -= u * this->my_matrix[k - 1][j - 1];
                }
        }
    }

    return k - 1;
}

int Matrix::Rank() const {
    Matrix matrix(this->NRows(), this->NCols());
    matrix.my_matrix = this->my_matrix;

    return matrix.RankRREF();
}

#endif //ZADATAK_MATRIX_H
