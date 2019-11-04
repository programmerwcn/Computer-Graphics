#cmake_minimum_required(VERSION 3.15)
project(Project2)


find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_CXX_STANDARD 14)

add_executable(Project2 main.cpp Polygon.cpp Polygon.h)

target_link_libraries(Project2 ${GLUT_LIBRARY} ${OPENGL_LIBRARY} m)