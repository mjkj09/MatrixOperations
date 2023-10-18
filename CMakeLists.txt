cmake_minimum_required(VERSION 3.23)
project(5_6 C)

set(CMAKE_C_STANDARD 99)

add_executable(5_6 MatrixOperationsFromFiles.c matrix_utils.c matrix_utils.h
)
