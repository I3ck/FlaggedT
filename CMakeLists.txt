project(FlaggedT)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -O3")
set(CMAKE_EXE_LINKER_FLAGS "-lpthread")

include_directories(
        flaggedT/
)

add_executable(
        test_1
        tests/test_1.cpp
)
