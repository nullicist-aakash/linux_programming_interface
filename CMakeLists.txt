cmake_minimum_required(VERSION 3.30)
project(linux_programming_interface)

set(CMAKE_CXX_STANDARD 20)

add_executable(
    linux_programming_interface
    main.cpp
        chap4/4.1.cpp
        chap4/4.2.cpp
)
