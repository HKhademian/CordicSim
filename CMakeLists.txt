cmake_minimum_required(VERSION 3.19)
project(project
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(cordic-sim
        ../fixedsim/fixed.hpp cordic.hpp cordic-sim.cpp)
