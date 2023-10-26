cmake_minimum_required(VERSION 3.26)
project(scanner)

set(CMAKE_CXX_STANDARD 23)

add_executable(scanner scanner.cpp
        scanner.h)
