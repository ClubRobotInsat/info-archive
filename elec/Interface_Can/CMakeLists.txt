set(SOURCE_FILES_INTERFACE
        Can_Interface.cpp
        Application.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")

#Vu que c'est la manière "officielle" de récuperer gtk3 via Cmake, on est obligé de passer par PkgConfig
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})


add_executable(Can_Interface ${SOURCE_FILES_INTERFACE})
target_link_libraries(Can_Interface ${GTK3_LIBRARIES})
