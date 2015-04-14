cmake_minimum_required(VERSION 3.2)

option(TARGET "Architecture cible" pc)
option(COLOR "Sortie en couleur de la compilation" ON)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
if (CMAKE_CXX_COMPILER_ID STREQUAL Clang)
  if (COLOR)
    set(CMAKE_COLOR_MAKEFILE ON)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fcolor-diagnostics")
  else ()
    set(CMAKE_COLOR_MAKEFILE OFF)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-color-diagnostics")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-color-diagnostics")
  endif()
endif()
message(STATUS "${CMAKE_CXX_FLAGS}")
message(STATUS "${CMAKE_C_FLAGS}")

include_directories(commun)
add_subdirectory(commun)
add_subdirectory(robot)

# Juste lancer cmake avec les bons compilos dans env CC / CXX
if (${TARGET} STREQUAL "rasPi")
else()
  add_subdirectory(simulateur)
endif()


