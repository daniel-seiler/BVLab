cmake_minimum_required(VERSION 3.17)
project(A4)

set(CMAKE_CXX_STANDARD 11)

file(GLOB a4_src
        "*.h"
        "*.cpp"
        )

add_executable(A4 ${a4_src})
