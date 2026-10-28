cmake_minimum_required(VERSION 3.26)
project(Zadatak_1)

set(CMAKE_CXX_STANDARD 17)

add_executable(Zadatak_1 main.cpp
        main/Comparator.h
        main/ChebyshevApproximation.h
        test/ChebyshevApproximationTest.h)
