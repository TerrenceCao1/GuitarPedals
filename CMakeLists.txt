cmake_minimum_required(VERSION 3.10)
project(GuitarPedal)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

include_directories(inc)

add_executable(GuitarPedal
	src/AudioFrame.cpp
	src/main.cpp
)
