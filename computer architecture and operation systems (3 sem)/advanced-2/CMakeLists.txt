cmake_minimum_required(VERSION 3.12)
project(advanced_2 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-m32 -msse4.1 -Wall")
add_executable(advanced_2 main.c)