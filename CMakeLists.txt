cmake_minimum_required(VERSION 3.10)
project(T3_CG)

set(OpenGlLinkers -lglut -lGLU -lGL)
set(CMAKE_CXX_STANDARD 11)

include_directories(T3)
include_directories(T3/include)

add_executable(T3_CG
        T3/include/Aux.h
        T3/include/Bloco.h
        T3/include/Desenha.h
        T3/include/EnemySpawner.h
        T3/include/Esfera.h
        T3/include/GameController.h
        T3/include/glcWavefrontObject.h
        T3/include/IO.h
        T3/include/Pad.h
        T3/include/Plano.h
        T3/include/Tabuleiro.h
        T3/include/Triangulo.h
        T3/include/Vertice.h
        T3/src/Aux.cpp
        T3/src/Bloco.cpp
        T3/src/Desenha.cpp
        T3/src/EnemySpawner.cpp
        T3/src/Esfera.cpp
        T3/src/GameController.cpp
        T3/src/glcWavefrontObject.cpp
        T3/src/IO.cpp
        T3/src/Pad.cpp
        T3/src/Plano.cpp
        T3/src/Tabuleiro.cpp
        T3/src/Triangulo.cpp
        T3/src/Vertice.cpp
        T3/Extras.h
        T3/main.cpp)

target_link_libraries(T3_CG ${OpenGlLinkers})
