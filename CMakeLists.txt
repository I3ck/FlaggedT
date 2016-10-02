project(FlaggedT)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra")
set(CMAKE_EXE_LINKER_FLAGS "-lpthread")

include_directories(
        inc/
)

add_executable(
        test_1
        tests/test_1.cpp
)

#aux_source_directory(
#        src/
#        SRC_LIST
#)
