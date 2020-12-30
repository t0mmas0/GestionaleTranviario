cmake_minimum_required(VERSION 3.17)
project(GestionaleTranviario)

set(CMAKE_CXX_STANDARD 14)

add_executable(GestionaleTranviario main.cpp Treno.cpp Treno.h Stazione.cpp Stazione.h LineaFerroviaria.cpp LineaFerroviaria.h GestoreOrari.cpp GestoreOrari.h Semaforo.cpp Semaforo.h StazioneSecondaria.cpp StazioneSecondaria.h)