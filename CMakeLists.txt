cmake_minimum_required(VERSION 3.2)

option(TARGET "Architecture cible" pc)

if (CMAKE_CXX_COMPILER_ID STREQUAL Clang)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics -std=c++1y")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fcolor-diagnostics")
else ()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
endif()

include_directories(commun)
add_subdirectory(commun)
add_subdirectory(robot)

# Juste lacer cmake avec les bons compilos dans env CC / CXX
if (${TARGET} STREQUAL "rasPi")
else()
  add_subdirectory(simulateur)
endif()


