add_library(GtkSimu
        GtkSimuContext.cpp
        GtkSimuApplication.cpp
        PanelConnect.cpp
        PanelRobotState.cpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

target_include_directories(GtkSimu PUBLIC ${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})
add_definitions(${GTKMM_CFLAGS_OTHER})

set_property(TARGET GtkSimu PROPERTY CXX_STANDARD 14)

target_link_libraries(GtkSimu ${GTKMM_LIBRARIES})

# glade files
# TODO dependance entre GtkSimu et move des fichiers glade
#add_custom_command(TARGET GtkSimu PRE_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_file
#        ${CMAKE_SOURCE_DIR}/glade $<TARGET_FILE_DIR:GtkSimu>/..)

function(include_glade_file glade_file)
    file(READ ${glade_file} content)
    set(delim "for_c++_include")
    file(WRITE
            "${glade_file}.include"
            "R\"${delim}(\n${content})${delim}\"")
endfunction(include_glade_file)

file(GLOB glade_files ${CMAKE_SOURCE_DIR}/src/simulateur/gui/gtk/glade/*.glade)
foreach(glade_file ${glade_files})
    include_glade_file(${glade_file})
endforeach()

#
#
#foreach(glade_file ${glade_files})
#    include_glade_file(${glade_file})
#endforeach()