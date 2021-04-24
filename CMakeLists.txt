cmake_minimum_required(VERSION 3.19)
project(Efficient_algorithms_and_data_structures)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(Efficient_algorithms_and_data_structures
        avltree.h
        functions.cpp
        hash.h
        list.h
        mm.h
        sort.h)
