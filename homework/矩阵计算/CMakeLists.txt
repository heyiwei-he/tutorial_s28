cmake_minimum_required(VERSION 3.15)
project(MatrixCalc CXX)

# C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# MSVC 下用 /utf-8，否则中文注释和 cout 输出会乱码
if(MSVC)
    add_compile_options(/utf-8)
endif()

add_executable(matrix 矩阵.cpp)
