cmake_minimum_required(VERSION 3.12)
project(geometry)

set(CMAKE_CXX_STANDARD 11)

add_executable(geometry main.cpp CPoint2D.h CPoint2D.h CVector2D.h CPoint3D.h CVector3D.h CSegment3D.h)