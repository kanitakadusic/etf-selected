cmake_minimum_required(VERSION 3.26)
project(Zadatak_2)

set(CMAKE_CXX_STANDARD 17)

add_executable(Zadatak_2 main.cpp
        main/LocalExtremum.h
        main/Comparator.h
        test/LocalExtremumTest.h)
