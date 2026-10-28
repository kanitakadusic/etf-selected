cmake_minimum_required(VERSION 3.26)
project(Zadatak_1)

set(CMAKE_CXX_STANDARD 17)

add_executable(Zadatak_1 main.cpp
        main/LinearInterpolator.h
        main/PolynomialInterpolator.h
        main/PiecewisePolynomialInterpolator.h
        main/SplineInterpolator.h
        main/BarycentricInterpolator.h
        main/TrigonometricInterpolator.h
        main/AbstractInterpolator.h
        main/Comparator.h
        test/AbstractInterpolatorTest.h
        test/LinearInterpolatorTest.h
        test/PolynomialInterpolatorTest.h
        test/PiecewisePolynomialInterpolatorTest.h
        test/SplineInterpolatorTest.h
        test/BarycentricInterpolatorTest.h
        test/TrigonometricInterpolatorTest.h
)
