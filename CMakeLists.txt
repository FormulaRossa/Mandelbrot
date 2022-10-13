cmake_minimum_required(VERSION 3.18)
project(Mandelbrot)

set(CMAKE_CXX_STANDARD 14)


add_executable(Mandelbrot main.cpp)
add_library(Lodepng lodepng.cpp lodepng.h)
target_link_libraries(Mandelbrot PRIVATE Lodepng)