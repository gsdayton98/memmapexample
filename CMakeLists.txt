cmake_minimum_required(VERSION 3.22)
project(memmapexample)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(memmapexample
        file.hpp
        filemap.cpp
        fileopen.cpp
        memmapexample.cpp
        oscpp.hpp
        stopwatch.cpp
        stopwatch.hpp
        sysexception.cpp
        sysexception.hpp)
