cmake_minimum_required(VERSION 3.10)
project(T3_CG)


set(OpenGlLinkers -lglut -lGLU -lGL)
set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

include_directories(T3)
include_directories(T3/include)

add_executable(T3_CG
        T3/include/aux.h
        T3/include/bloco.h
        T3/include/desenha.h
        T3/include/esfera.h
        T3/include/gameController.h
        T3/include/glcWavefrontObject.h
        T3/include/io.h
        T3/include/pad.h
        T3/include/plano.h
        T3/include/tabuleiro.h
        T3/include/triangulo.h
        T3/include/vertice.h
        T3/include/camera.h
        T3/src/aux.cpp
        T3/src/bloco.cpp
        T3/src/desenha.cpp
        T3/src/esfera.cpp
        T3/src/gameController.cpp
        T3/src/glcWavefrontObject.cpp
        T3/src/io.cpp
        T3/src/pad.cpp
        T3/src/plano.cpp
        T3/src/tabuleiro.cpp
        T3/src/triangulo.cpp
        T3/src/vertice.cpp
        T3/src/camera.cpp
        T3/main.cpp)

target_link_libraries(T3_CG ${OpenGlLinkers})
