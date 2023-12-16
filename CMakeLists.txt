cmake_minimum_required(VERSION 3.0.0)
project(open_gl_drone)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(open_gl_drone 
  src/main.cpp
  src/stb_image.h
)

target_link_libraries(open_gl_drone 
  OpenGL::GL
  GLUT::GLUT
  OpenGL::GLU
)
