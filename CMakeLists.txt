cmake_minimum_required(VERSION 3.22)
project(memmapexample)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(memmapexample
        memmapexample.cpp
        stopwatch.cpp
        stopwatch.hpp
        systemexception.cpp
        systemexception.hpp filereadmethod.cpp memoryMapMethod.cpp)
