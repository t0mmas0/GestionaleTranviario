cmake_minimum_required(VERSION 3.17)
project(GestionaleTranviario)

set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-O2")
set(CMAKE_CXX_FLAGS_DEBUG "-O2")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_executable(GestionaleTranviario main.cpp Treno.cpp Treno.h Stazione.cpp Stazione.h LineaFerroviaria.cpp LineaFerroviaria.h Semaforo.cpp Semaforo.h StazioneSecondaria.cpp StazioneSecondaria.h LeggiLinea.h LeggiLinea.cpp LeggiOrari.h LeggiOrari.cpp StazionePrincipale.cpp StazionePrincipale.h main.cpp Grafica.cpp Grafica.h)