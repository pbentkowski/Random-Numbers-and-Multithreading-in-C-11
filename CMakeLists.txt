cmake_minimum_required(VERSION 3.10)
project(cpp11random)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

include_directories(.)

add_executable(cpp11random
        getRandomMulti.cpp
        Random.cpp
        Random.h)
