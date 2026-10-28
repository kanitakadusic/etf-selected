cmake_minimum_required(VERSION 3.26)
project(Zadaca_1_i_2)

set(CMAKE_CXX_STANDARD 17)

add_executable(Zadaca_1_i_2 main.cpp
        main/Vector.h
        main/Matrix.h
        main/LUDecomposer.h
        main/QRDecomposer.h
        test/VectorMatrixTest.h
        test/DecomposersTest.h
        main/Comparator.h)
