cmake_minimum_required(VERSION 3.12)
project(3 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Werror -m32 -msse4.1 -Wall")

add_executable(3 main.c)