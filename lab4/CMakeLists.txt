cmake_minimum_required(VERSION 3.16)

project(lab4_jacobi LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(OpenMP REQUIRED)

include_directories(include)

add_library(jacobi src/jacobi.cpp)
target_link_libraries(jacobi PUBLIC OpenMP::OpenMP_CXX)

add_executable(jacobi_app src/main.cpp)
target_link_libraries(jacobi_app PRIVATE jacobi)

add_executable(jacobi_tests tests/test_jacobi.cpp)
target_link_libraries(jacobi_tests PRIVATE jacobi)
