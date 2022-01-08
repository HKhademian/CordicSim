cmake_minimum_required(VERSION 3.19)
project(project
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(cordic-sim
        fixed/fixed.hpp cordic.hpp main.cpp)

add_executable(test-fixed
        fixed/fixed.hpp fixed/test-fixed.cpp)

add_executable(test-mask
        fixed/fixed.hpp fixed/test-mask.cpp)
