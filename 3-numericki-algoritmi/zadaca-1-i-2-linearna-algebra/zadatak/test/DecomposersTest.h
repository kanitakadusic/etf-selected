#ifndef ZADATAK_DECOMPOSERSTEST_H
#define ZADATAK_DECOMPOSERSTEST_H

#include "../main/LUDecomposer.h"
#include "../main/QRDecomposer.h"

void DecomposersTest() {
    void print(const Vector&);
    void print(const Matrix&);
    std::cout << std::boolalpha;

    Vector v{1e-10, 12, 35.55, 1e-4};
    v.Chop(1e-7);
    print(v);

    std::cout << (v + v).EqualTo(v * 2) << std::endl;
    std::cout << v.EqualTo(v * 2) << std::endl;

    Matrix m{{1e-10, 12, 1e-4}, {1e-8, 2, 0}, {1e-6, 0, 35.55}};
    m.Chop(1e-7);
    print(m);

    std::cout << (m + m).EqualTo(m * 2) << std::endl;
    std::cout << m.EqualTo(m * 2) << std::endl;

    Vector b(1);
    Matrix A(1, 1), B(1, 1), C(1, 1);

    try { // Divisor matrix is not square
        A = {{1, 2, 3}, {7, 3, 5}};
        B = {{2, 3}};

        print(LeftDiv(A, B));
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    try { // Incompatible formats
        A = {{1, 2, 3}, {7, 3, 5}, {4, 4, 3}};
        B = {{2, 3}};

        print(LeftDiv(A, B));
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    try { // Divisor matrix is singular
        A = {{1, 2, 3}, {7, 3, 5}, {1, 2, 3}};
        B = {{2, 3}, {1, 5}, {5, 6}};

        print(LeftDiv(A, B));
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    try { // Divisor matrix is singular
        A = {{0, 2, 3}, {0, 3, 5}, {0, 3, 3}};
        B = {{2, 3}, {1, 5}, {5, 6}};

        print(LeftDiv(A, B));
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    A = {{1, 2, 3}, {0, 3, 5}, {2, 3, 3}};
    B = {{2, 3}, {1, 5}, {5, 6}};
    print(LeftDiv(A, B));
    print(LeftDiv(A, A * A));

    C = {{1}};
    print(LeftDiv(C, C * C));

    try { // Divisor matrix is not square
        A = {{1, 2, 3}, {7, 3, 5}};
        b = {2, 3};

        print(LeftDiv(A, b));
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    try { // Incompatible formats
        A = {{1, 2, 3}, {7, 3, 5}, {4, 4, 3}};
        b = {2, 3};

        print(LeftDiv(A, b));
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    try { // Divisor matrix is singular
        A = {{1, 2, 3}, {7, 3, 5}, {1, 2, 3}};
        b = {2, 3, 4};

        print(LeftDiv(A, b));
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    try { // Divisor matrix is singular
        A = {{0, 2, 3}, {0, 3, 5}, {0, 3, 3}};
        b = {2, 3, 4};

        print(LeftDiv(A, b));
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    A = {{1, 2, 3}, {0, 3, 5}, {2, 3, 3}};
    b = {2, 3, 4};
    print(LeftDiv(A, b));

    try { // Divisor matrix is not square
        A = {{1, 2, 3}, {7, 3, 5}};
        B = {{2, 3}};

        B /= A;
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    try { // Incompatible formats
        A = {{1, 2, 3}, {7, 3, 5}, {4, 4, 3}};
        B = {{2, 3}};

        B /= A;
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    try { // Divisor matrix is singular
        A = {{1, 2, 3}, {7, 3, 5}, {1, 2, 3}};
        B = {{2, 3, 1}};

        B /= A;
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    try { // Divisor matrix is singular
        A = {{0, 2, 3}, {0, 3, 5}, {0, 3, 3}};
        B = {{2, 3, 1}};

        B /= A;
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    A = {{1, 2, 3}, {0, 3, 5}, {2, 3, 3}};
    B = {{2, 3, 1}};
    print(B /= A);
    print((A * A) /= A);

    C = {{1}};
    print((C * C) /= C);

    print(B / A); // implementirana koristeci vec testiranu metodu

    try { // Division by zero
        A = {{0, 2, 3}, {0, 3, 5}, {0, 3, 3}};
        print(A /= 0);
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    print(A / 5);

    try { // Matrix is not square
        A = {{0, 2, 3}, {0, 3, 5}};
        std::cout << A.Det() << std::endl;
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    A = {{1, 2, 3}, {0, 3, 5}, {1, 2, 3}};
    std::cout << A.Det() << std::endl;

    A = {{0, 2, 3}, {0, 3, 5}, {0, 2, 3}};
    std::cout << A.Det() << std::endl;

    A = {{1, 2, 3}, {0, 3, 5}, {4, 4, 3}};
    std::cout << A.Det() << std::endl;
    print(A);

    std::cout << Det(A) << std::endl; // implementirana koristeci vec testiranu metodu

    try { // Matrix is not square
        A = {{0, 2, 3}, {0, 3, 5}};
        A.Invert();
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    try { // Matrix is singular
        A = {{1, 2, 3}, {0, 3, 5}, {1, 2, 3}};
        A.Invert();
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    try { // Matrix is singular
        A = {{0, 2, 3}, {0, 3, 5}, {0, 2, 3}};
        A.Invert();
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    A = {{1, 2, 3}, {0, 3, 5}, {4, 4, 3}};
    A.Invert();
    print(A);

    A = {{5, 0, 0}, {0, 3, 0}, {0, 0, 2}};
    print(Inverse(A)); // implementirana koristeci vec testiranu metodu

    A = {{1, 2, 3, 4, 5}, {2, 8, 7, 5, 2}, {1, 6, 5, 4, 2}};
    std::cout << A.Rank() << std::endl;
    A.ReduceToRREF();
    print(A);

    A = {{1, 2, 3, 4, 5}, {5, 0, 7.1, 0, 2}, {2, 4, 6, 8, 10}};
    std::cout << Rank(A) << std::endl;
    print(RREF(A));

    try { // Matrix is not square
        A = {{0, 2, 3}, {0, 3, 5}};
        LUDecomposer lu(A);
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    try { // Matrix is singular
        A = {{1, 2, 3}, {0, 3, 5}, {2, 4, 6}};
        LUDecomposer lu(A);
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    try { // Matrix is singular
        A = {{0, 2, 3}, {0, 3, 5}, {0, 4, 6}};
        LUDecomposer lu(A);
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    {
        A = {{0, 2, 3}, {1, 3, 5}, {0, 2, 5}};
        LUDecomposer lu(A);

        print(lu.GetL());
        print(lu.GetU());
        print(lu.GetCompactLU());
        print(lu.GetPermuation());

        print(lu.GetL() * lu.GetU());

        try { // Incompatible formats
            b = {1, 2};
            lu.Solve(b);
        } catch (std::domain_error& e) {
            std::cout << e.what() << std::endl;
        }

        b = {1, 2, 3};
        print(lu.Solve(b));

        try { // Incompatible formats
            B = {{1, 2}, {2, 4}};
            lu.Solve(B);
        } catch (std::domain_error& e) {
            std::cout << e.what() << std::endl;
        }

        B = {{1, 2}, {2, 4}, {5, 2}};
        print(lu.Solve(B));
    }

    {
        A = {{1, 2}, {5, 6}};
        LUDecomposer lu(A);

        print(lu.GetL());
        print(lu.GetU());
        print(lu.GetCompactLU());
        print(lu.GetPermuation());

        print(lu.GetL() * lu.GetU());

        try { // Incompatible formats
            b = {1};
            lu.Solve(b);
        } catch (std::domain_error& e) {
            std::cout << e.what() << std::endl;
        }

        b = {1, 2};
        print(lu.Solve(b));

        try { // Incompatible formats
            B = {{1, 2}};
            lu.Solve(B);
        } catch (std::domain_error& e) {
            std::cout << e.what() << std::endl;
        }

        B = {{1, 2, 4, 5, 6}, {2, 4, 2, 3, 1}};
        print(lu.Solve(B));
    }

    try { // Invalid matrix format
        A = {{0, 2, 3}, {0, 3, 5}};
        QRDecomposer qr(A);
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    try { // Matrix is singular
        A = {{1, 2, 3}, {0, 3, 5}, {2, 4, 6}};
        QRDecomposer qr(A);
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    try { // Matrix is singular
        A = {{0, 2, 3}, {0, 3, 5}, {0, 4, 6}};
        QRDecomposer qr(A);
    } catch (std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }

    {
        A = {{0, 2, 3}, {1, 3, 7}, {0, 2, 6}};
        QRDecomposer qr(A);

        print(qr.GetQ());
        print(qr.GetR());

        print(qr.GetQ() * qr.GetR());

        try { // Incompatible formats
            b = {1, 2, 3, 4};
            qr.MulQWith(b);
        } catch (std::domain_error& e) {
            std::cout << e.what() << std::endl;
        }

        b = {1, 2, 3};
        print(qr.MulQWith(b));
        print(qr.GetQ() * b);

        try { // Incompatible formats
            B = {{1, 2}, {2, 3}};
            qr.MulQWith(B);
        } catch (std::domain_error& e) {
            std::cout << e.what() << std::endl;
        }

        B = {{4, 5}, {3, 2}, {1, 2}};
        print(qr.MulQWith(B));
        print(qr.GetQ() * B);

        try { // Incompatible formats
            b = {1, 2, 3, 4};
            qr.MulQTWith(b);
        } catch (std::domain_error& e) {
            std::cout << e.what() << std::endl;
        }

        b = {1, 2, 3};
        print(qr.MulQTWith(b));
        print(Transpose(qr.GetQ()) * b);

        try { // Incompatible formats
            B = {{1, 2}, {2, 3}};
            qr.MulQTWith(B);
        } catch (std::domain_error& e) {
            std::cout << e.what() << std::endl;
        }

        B = {{4, 5}, {3, 2}, {1, 2}};
        print(qr.MulQTWith(B));
        print(Transpose(qr.GetQ()) * B);

        try { // Incompatible formats
            b = {1, 2};
            qr.Solve(b);
        } catch (std::domain_error& e) {
            std::cout << e.what() << std::endl;
        }

        b = {1, 2, 3};
        print(qr.Solve(b));

        try { // Incompatible formats
            B = {{1, 2}, {2, 4}};
            qr.Solve(B);
        } catch (std::domain_error& e) {
            std::cout << e.what() << std::endl;
        }

        B = {{1, 2}, {2, 4}, {5, 2}};
        print(qr.Solve(B));
    }

    {
        A = {{0, 2}, {1, 7}};
        QRDecomposer qr(A);

        print(qr.GetQ());
        print(qr.GetR());

        print(qr.GetQ() * qr.GetR());

        try { // Incompatible formats
            b = {1, 2, 3};
            qr.MulQWith(b);
        } catch (std::domain_error& e) {
            std::cout << e.what() << std::endl;
        }

        b = {5, 9};
        print(qr.MulQWith(b));
        print(qr.GetQ() * b);

        try { // Incompatible formats
            B = {{1, 2, 3}, {2, 3, 1}, {5, 6, 6}};
            qr.MulQWith(B);
        } catch (std::domain_error& e) {
            std::cout << e.what() << std::endl;
        }

        B = {{4}, {3}};
        print(qr.MulQWith(B));
        print(qr.GetQ() * B);

        try { // Incompatible formats
            b = {1, 2, 3, 4};
            qr.MulQTWith(b);
        } catch (std::domain_error& e) {
            std::cout << e.what() << std::endl;
        }

        b = {1, 7};
        print(qr.MulQTWith(b));
        print(Transpose(qr.GetQ()) * b);

        try { // Incompatible formats
            B = {{1, 2, 4}, {2, 3, 2}, {9, 2, 0}};
            qr.MulQTWith(B);
        } catch (std::domain_error& e) {
            std::cout << e.what() << std::endl;
        }

        B = {{4, 5}, {1, 2}};
        print(qr.MulQTWith(B));
        print(Transpose(qr.GetQ()) * B);

        try { // Incompatible formats
            b = {1, 2, 1};
            qr.Solve(b);
        } catch (std::domain_error& e) {
            std::cout << e.what() << std::endl;
        }

        b = {2, 3};
        print(qr.Solve(b));

        try { // Incompatible formats
            B = {{1, 2, 9}, {2, 4, 5}, {4, 5, 1}};
            qr.Solve(B);
        } catch (std::domain_error& e) {
            std::cout << e.what() << std::endl;
        }

        B = {{2, 4}, {5, 2}};
        print(qr.Solve(B));
    }
}

void print(const Vector& vector) {
    vector.Print();
    std::cout << std::endl;
}

void print(const Matrix& matrix) {
    matrix.Print();
    std::cout << std::endl;
}

#endif //ZADATAK_DECOMPOSERSTEST_H
