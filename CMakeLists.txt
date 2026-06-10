cmake_minimum_required(VERSION 3.16)

project(Semga67Lab3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(semga67_cli
    src/main.cpp
    src/verify.cpp
    src/benchmark.cpp
)

target_include_directories(semga67_cli PRIVATE include)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(semga67_cli PRIVATE -Wall -Wextra -Wpedantic)
endif()