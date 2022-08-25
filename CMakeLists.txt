cmake_minimum_required(VERSION 3.22)
project(ex6_noamt)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ex6_noamt
        Dictionary.hpp
        HashMap.hpp
        tests.cpp
        )
