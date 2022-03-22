cmake_minimum_required(VERSION 3.21)
project(DSCD)

MATH(EXPR stack_size "500*1024*1024")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")

set(CMAKE_CXX_STANDARD 14)

add_executable(DSCD main.cpp)
