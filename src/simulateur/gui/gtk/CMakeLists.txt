add_library(GtkSimu
        GtkSimuContext.cpp
        GtkSimuApplication.cpp
        PanelConnect.cpp
        PanelCentral.cpp
        PanelRobotState.cpp
        PanelTestNavigation.cpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

target_include_directories(GtkSimu PUBLIC ${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})
add_definitions(${GTKMM_CFLAGS_OTHER})

set_property(TARGET GtkSimu PROPERTY CXX_STANDARD 14)

target_link_libraries(GtkSimu Resources ${GTKMM_LIBRARIES})

# glade files
file(GLOB glade_files ${CMAKE_SOURCE_DIR}/src/simulateur/gui/gtk/glade/*.glade)
foreach(glade_file ${glade_files})
    get_filename_component(glade_filename ${glade_file} NAME)
    include_file(glade/${glade_filename})
endforeach()
