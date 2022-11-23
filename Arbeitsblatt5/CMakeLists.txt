cmake_minimum_required(VERSION 3.17)
project(A5)

set(CMAKE_CXX_STANDARD 11)

file(GLOB a5_src
        "*.h"
        "*.cpp"
        )

add_executable(A5 ${a5_src})
