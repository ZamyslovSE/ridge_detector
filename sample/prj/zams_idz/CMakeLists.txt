cmake_minimum_required(VERSION 3.0)

project(zams_idz)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(PUBLIC_HEADERS stdafx.h)

set(HEADERS ${PUBLIC_HEADERS})

set(SOURCES zams_idz.cpp)

add_executable(zams_idz ${HEADERS} ${SOURCES})

target_link_libraries(zams_idz ${OpenCV_LIBS})