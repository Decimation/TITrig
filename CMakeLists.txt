cmake_minimum_required(VERSION 3.9.6)
project(TITrig C)

set(CMAKE_C_STANDARD 11)
set(HEADER_FILES C:/CEdev/include/)
include_directories(C:/CEdev/include/)
add_executable(TITrig src/Triangle.c src/Triangle.h src/GraphicsExt.c src/GraphicsExt.h src/Library.c src/Library.h src/Trigonometry.h src/Trigonometry.c src/Geometry.c src/Geometry.h src/Main.c src/Settings.c src/Settings.h src/RightTriangle.c src/RightTriangle.h src/Simplifiers.c src/Simplifiers.h src/Main.h src/Types.h)