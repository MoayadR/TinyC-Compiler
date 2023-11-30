cmake_minimum_required(VERSION 3.26)
project(TinyC_Compiler)

set(CMAKE_CXX_STANDARD 23)

add_executable(TinyC_Compiler main.cpp
        scanner.cpp
        parser.cpp
        parser.h
)
