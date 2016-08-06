set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O0 -std=c++14 -std=gnu++14" )
set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES_INTERFACE
        Can_Interface.cpp
        Application.cpp
        Start_Screen.cpp
        Header.h)

#Vu que c'est la manière "officielle" de récuperer gtkmm via Cmake, on est obligé de passer par PkgConfig
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})
add_definitions(${GTKMM_CFLAGS_OTHER})

add_executable(Can_Interface ${SOURCE_FILES_INTERFACE} )
target_link_libraries(Can_Interface  ${GTKMM_LIBRARIES} Robot)
