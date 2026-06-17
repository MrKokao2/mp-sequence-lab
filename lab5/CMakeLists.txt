cmake_minimum_required(VERSION 3.16)

project(lab5_setrel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(OpenMP REQUIRED)

include_directories(include)

add_library(setrel src/setrel.cpp)
target_link_libraries(setrel PUBLIC OpenMP::OpenMP_CXX)

add_executable(setrel_app src/main.cpp)
target_link_libraries(setrel_app PRIVATE setrel)

add_executable(setrel_tests tests/test_setrel.cpp)
target_link_libraries(setrel_tests PRIVATE setrel)
