cmake_minimum_required(VERSION 3.16)

project(lab3_openmp_intro LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

include_directories(include)

set(EXAMPLE_SOURCES
        src/example1_hello.cpp
        src/example2_dotproduct_for.cpp
        src/example3_dotproduct_subroutine.cpp
        src/example4_dotproduct_sections.cpp
        src/example5_matrix_vector.cpp
)

add_executable(openmp_intro_app
        src/main.cpp
        ${EXAMPLE_SOURCES}
)

target_link_libraries(openmp_intro_app PUBLIC OpenMP::OpenMP_CXX)

add_executable(openmp_intro_tests
        tests/test_examples.cpp
        ${EXAMPLE_SOURCES}
)

target_link_libraries(openmp_intro_tests PUBLIC OpenMP::OpenMP_CXX)
