cmake_minimum_required(VERSION 3.10)
project(GuitarPedal)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

include_directories(inc)

# Main Exec
add_executable(GuitarPedal
	src/AudioFrame.cpp
	src/main.cpp
)

# ----- Testing -----
enable_testing()

add_executable(test_bypass
	src/AudioFrame.cpp
	src/Effects.cpp
	test/BypassTest.cpp
)

add_test(NAME BypassTest COMMAND test_bypass)
