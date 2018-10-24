cmake_minimum_required(VERSION 3.12)
project(task_b)

set(CMAKE_CXX_STANDARD 14)

add_executable(task_b main.cpp prefix_func.cpp prefix_func.h z_func.cpp z_func.h cross_transform.cpp cross_transform.h)