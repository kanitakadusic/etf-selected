cmake_minimum_required(VERSION 3.26)
project(Nelder_Mead_algoritam)

set(CMAKE_CXX_STANDARD 17)

add_executable(Nelder_Mead_algoritam main.cpp
        main/Comparator.h
        main/Optimization.h
        test/OptimizationTest.h)
