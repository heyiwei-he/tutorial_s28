cmake_minimum_required(VERSION 3.10)
project(armor_hw)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(armor_hw main.cpp)
target_link_libraries(armor_hw ${OpenCV_LIBS})