add_subdirectory(Environment)
add_subdirectory(ModuleInterfacers)
add_subdirectory(PetriLab)

set(STRAT_SOURCE
        AbstractStrategy.cpp)
add_library(Strategy STATIC ${STRAT_SOURCE})
target_link_libraries(Strategy ModuleInterfacers HighLevelFunctions)

include_directories(/usr/local/include/wiic/)

set(WIIMOTE_SOURCES IA/IAWiiMote.cpp wiimote/Wiimote.cpp)
add_executable(IAWiimote ${WIIMOTE_SOURCES})

if (RASPI)
    # WiiMote #
    message(STATUS "Compiling IAWiimote for the raspi")

    add_library(libwiimotecpp SHARED IMPORTED)
    add_library(libwiimotec SHARED IMPORTED)
    set_target_properties(libwiimotecpp PROPERTIES IMPORTED_LOCATION "../../../precompiled-libraries/libwiicpp.so")
    set_target_properties(libwiimotec PROPERTIES IMPORTED_LOCATION "../../../precompiled-libraries/libwiic.so")

    #link_directories(${PROJECT_SOURCE_DIR}/precompiled-libraries/)
    #add_subdirectory(precompiled-libraries)
    target_link_libraries(IAWiimote ModuleInterfacers HighLevelFunctions libwiimotecpp libwiimotec)
else()
    target_link_libraries(IAWiimote ModuleInterfacers HighLevelFunctions wiicpp)
endif()
