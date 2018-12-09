add_library(GtkSimu
        GtkSimuContext.cpp
        GtkSimuApplication.cpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

target_include_directories(GtkSimu PUBLIC ${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})
add_definitions(${GTKMM_CFLAGS_OTHER})

set_property(TARGET GtkSimu PROPERTY CXX_STANDARD 14)

target_link_libraries(GtkSimu ${GTKMM_LIBRARIES})