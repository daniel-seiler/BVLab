cmake_minimum_required(VERSION 3.21)
project(BVProject)

find_package( OpenCV REQUIRED)
set(CMAKE_CXX_STANDARD 17)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(BVProject main.cpp)
target_link_libraries( BVProject ${OpenCV_LIBS})
