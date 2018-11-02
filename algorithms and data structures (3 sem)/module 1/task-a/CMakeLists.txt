cmake_minimum_required(VERSION 3.12)
project(task_a)

set(CMAKE_CXX_STANDARD 14)


add_executable(task_a main.cpp IStringFunctor.cpp IStringFunctor.h CZFunctor.cpp CZFunctor.h)

