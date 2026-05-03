cmake_minimum_required(VERSION 3.16)

project(lab5_matmul LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(OpenMP REQUIRED)

include_directories(include)

add_library(matmul src/matmul.cpp)
target_link_libraries(matmul PUBLIC OpenMP::OpenMP_CXX)

add_executable(matmul_app src/main.cpp)
target_link_libraries(matmul_app PRIVATE matmul)

add_executable(matmul_tests tests/test_matmul.cpp)
target_link_libraries(matmul_tests PRIVATE matmul)
