cmake_minimum_required(VERSION 3.14)
project(S0008E)

set(CMAKE_CXX_STANDARD 14)
file(GLOB SOURCE *.h)
file(GLOB SOURCE *.cpp)
add_executable(S0008E ${SOURCE})