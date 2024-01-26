cmake_minimum_required(VERSION 3.27)
project(irsetup)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(irsetup
        irsetup.ino
        main.cpp)
