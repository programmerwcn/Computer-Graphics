cmake_minimum_required(VERSION 3.15)
project(Project1)


find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_CXX_STANDARD 14)

add_executable(Project1 main.cpp ModelGL.cpp ModelGL.h ViewGL.cpp ViewGL.h ControllerGL.cpp ControllerGL.h  Polygon.cpp Polygon.h Util.cpp Util.h Clipping.cpp Clipping.h ClippingPolygon.cpp ClippingPolygon.h Transform.cpp Transform.h)

target_link_libraries(Project1 ${GLUT_LIBRARY} ${OPENGL_LIBRARY} m)
