#ifndef ZADATAK_VECTORMATRIXTEST_H
#define ZADATAK_VECTORMATRIXTEST_H

#include <iomanip>

#include "../main/Matrix.h"

void GetVectorInfo(const Vector& vector) {
    std::cout << std::string(60, '-') << " [" << vector.NElems() << "]\n";

    static int x = 3; x++;

    if (x % 4 == 0) vector.Print('|', 1e-10);
    else if (x % 4 == 1) PrintVector(vector, '_');
    else if (x % 4 == 2)
        for (int i = 0; i < vector.NElems(); i++)
            std::cout << vector[i] << " ";
    else
        for (int i = 1; i <= vector.NElems(); i++)
            std::cout << vector(i) << " ";

    std::cout << std::endl;
}

void VectorTest() {
    try { Vector a(0); }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    try { Vector a(-8); }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    try { Vector a({}); }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    Vector A(5);
    Vector B{1, -2.3, 3, -4.1, 5, 6.89, -7.554, 8.2, 9};
    Vector C{-9, 8, 0, -6, 5};

    A[2] = 1;
    A(2) = 1;
    A[0] = 1;

    try { A(0) = 1; }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    try { A(20); }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    GetVectorInfo(A);
    GetVectorInfo(B);
    GetVectorInfo(C);

    std::cout << std::string(64, '-') << std::endl;
    std::cout << A.Norm() << " " << B.Norm() << " " << VectorNorm(C) << " " << VectorNorm(Vector{1, 1, 1, 1}) << std::endl;
    std::cout << A.GetEpsilon() << " " << B.GetEpsilon() << " " << C.GetEpsilon() << " " << Vector{0.1}.GetEpsilon() << std::endl;

    A += C; GetVectorInfo(A);
    Vector D = A + Vector{-1, 0, 0, 1, -1}; GetVectorInfo(D);

    try { A += B; }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    try { B + Vector{5}; }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    A -= C; GetVectorInfo(A);
    Vector E = Vector{-1, 0, 0, 1, -1} - A; GetVectorInfo(E);

    try { A -= B; }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    try { Vector{15} - B; }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    std::swap(E(1), E[0]);
    std::swap(E(3), E[2]);

    E *= 2; E = E * 1.5; E = -0.5 * E; GetVectorInfo(E);

    std::cout << D * E << " " << Vector{1, -2, 3, 4.1, 5} * D << std::endl;

    try { A * B; }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    try { A * Vector{1, 2, 3, 4, 5, 6, 7, 8, 9}; }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    E /= 0.5; E = E / 7; GetVectorInfo(E);
}

void GetMatrixInfo(const Matrix& matrix) {
    std::cout << std::string(60, '-') << " [" << matrix.NRows() << "x" << matrix.NCols() << "]\n";

    static int x = 3; x++;

    if (x % 4 == 0) matrix.Print(8, 1e-10);
    else if (x % 4 == 1) PrintMatrix(matrix, (x % 10) + 5);
    else if (x % 4 == 2)
        for (int i = 0; i < matrix.NRows(); i++, std::cout << std::endl)
            for (int j = 0; j < matrix.NCols(); j++)
                std::cout << std::setw(10) << matrix[i][j] << " ";
    else
        for (int i = 1; i <= matrix.NRows(); i++, std::cout << std::endl)
            for (int j = 1; j <= matrix.NCols(); j++)
                std::cout << std::setw(10) << matrix(i, j) << " ";
}

void MatrixTest() {
    try { Matrix a(0, 0); }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    try { Matrix a(-8, 0); }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    try { Matrix a(0, -3); }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    try { Matrix a(Vector{}); }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    try { Matrix a({}); }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    try { Matrix a({{1, 1}, {}, {1, 1}}); }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    try { Matrix a({{1, 1}, {1}, {1, 1}}); }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    Matrix A(3, 3);
    Matrix B{Vector{-9, 8, 0, -6, 5}};
    Matrix C({{1, -2.3, 3}, {-4.1, 5, 6.89}, {-7.554, 8.2, 9}});

    A[2][2] = 1;
    A(2, 2) = 1;
    *A[0] = 1;

    try { A(0, 0) = 1; }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    try { Matrix({{1, 2}, {2, 1}})(1, 5) = 1; }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    GetMatrixInfo(A);
    GetMatrixInfo(B);
    GetMatrixInfo(C);

    std::cout << std::string(66, '-') << std::endl;
    std::cout << A.Norm() << " " << B.Norm() << " " << MatrixNorm(C) << " " << MatrixNorm(Matrix({1, 1, 1, 1})) << std::endl;
    std::cout << A.GetEpsilon() << " " << B.GetEpsilon() << " " << C.GetEpsilon() << " " << Matrix({0.1}).GetEpsilon() << std::endl;

    A += C; GetMatrixInfo(A);
    Matrix D = A + Matrix({{-1, 0, 0}, {0, -1, 0}, {0, 0, -1}}); GetMatrixInfo(D);

    try { A += B; }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    try { B + Matrix({{5}}); }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    A -= C; GetMatrixInfo(A);
    Matrix E = A - Matrix({{-1, 0, 0}, {0, -1, 0}, {0, 0, -1}}); GetMatrixInfo(E);

    try { A -= B; }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    try { Matrix({{10}}) - C; }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    std::swap(E(1, 1), E[0][2]);
    std::swap(E(3, 1), E[2][2]);

    E *= 2; E = E * 0.5; E = 0.5 * E; GetMatrixInfo(E);

    D *= A; GetMatrixInfo(D);
    D = D * E; GetMatrixInfo(D);
    D = E * D; GetMatrixInfo(D);

    Matrix F = C * D; GetMatrixInfo(F);
    Vector G = F * Vector{2, 2, 2}; GetMatrixInfo(Matrix(G));

    try { A *= B; }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    try { A * B; }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    try { A * Vector{1, 2, 3, 4, 5}; }
    catch (std::exception& e) { std::cout << e.what() << std::endl; }

    A.Transpose(); GetMatrixInfo(A);
    B.Transpose(); GetMatrixInfo(B);
    A = Transpose(A); GetMatrixInfo(A);
    B = Transpose(B); GetMatrixInfo(B);

    Matrix H{{1, 2, 3, 4}, {2, 3, 4, 5}, {3, 4, 5, 6}, {4, 5, 6, 7}};
    H.Transpose(); GetMatrixInfo(H);
    H = Transpose(H); GetMatrixInfo(H);

    Matrix I{{1, 2, 3, 4}, {2, 3, 4, 5}};
    I.Transpose(); GetMatrixInfo(I);
    I = Transpose(I); GetMatrixInfo(I);
}

#endif //ZADATAK_VECTORMATRIXTEST_H
