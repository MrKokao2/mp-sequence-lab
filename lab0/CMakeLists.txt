cmake_minimum_required(VERSION 3.16)

project(lab0_sequence LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(sequence SHARED
        src/sequence.cpp
)

add_executable(sequence_app
        src/main.cpp
)

target_link_libraries(sequence_app sequence)

add_executable(sequence_tests
        tests/test_sequence.cpp
)

target_link_libraries(sequence_tests sequence)