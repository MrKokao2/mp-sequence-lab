cmake_minimum_required(VERSION 3.16)

project(lab1_sorting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(sort_app
        src/main.cpp
)

add_executable(sort_tests
        tests/test_sort.cpp
)