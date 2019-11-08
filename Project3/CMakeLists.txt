#cmake_minimum_required(VERSION 3.15)
project(Project3)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 14)

add_executable(Project3 main.cpp Polygon.h Polygon.cpp View.h View.cpp Polygon2D.h Polygon2D.cpp)
target_link_libraries(Project3 ${GLUT_LIBRARY} ${OPENGL_LIBRARY} m)