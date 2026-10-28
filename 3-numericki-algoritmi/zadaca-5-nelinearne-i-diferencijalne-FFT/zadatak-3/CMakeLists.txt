cmake_minimum_required(VERSION 3.26)
project(Zadatak_3)

set(CMAKE_CXX_STANDARD 17)

add_executable(Zadatak_3 main.cpp
        main/DifferentialEquations.h
        main/Comparator.h
        test/DifferentialEquationsTest.h)
