cmake_minimum_required(VERSION 3.9.6)
project(TITrig C)

set(CMAKE_C_STANDARD 11)
set(HEADER_FILES C:/CEdev/include/)
include_directories(C:/CEdev/include/)

add_executable(TITrig
        src/Triangle.c src/Triangle.h
        src/GraphicsExt.c src/GraphicsExt.h
        src/Library.c src/Library.h
        src/Trigonometry.h src/Trigonometry.c
        src/Main.c src/Main.h
        src/Settings.c src/Settings.h
        src/Right/RightTriangle.c src/Right/RightTriangle.h
        src/Simplifiers.c src/Simplifiers.h
        src/Types.h
        src/Right/RightOperability.c src/Right/RightOperability.h
        src/Right/RightDrawFunctions.c src/Right/RightDrawFunctions.h
        src/AutoTriangle/DynamicTriangle.c src/AutoTriangle/DynamicTriangle.h
        src/LabelPoint/LabelPoint.c src/LabelPoint/LabelPoint.h)