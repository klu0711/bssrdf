cmake_minimum_required(VERSION 3.10)
project(S0008E)

set(CMAKE_CXX_STANDARD 14)
file(GLOB SOURCE *.h)
file(GLOB SOURCE *.cpp)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
add_executable(S0008E ${SOURCE})