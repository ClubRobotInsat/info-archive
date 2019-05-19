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

add_executable(Calibration calibration/Calibration.cpp)
target_link_libraries(Calibration ModuleInterfacers HighLevelFunctions)

if(EXISTS "/usr/bin/petrilab")
    set(USE_PETRILAB ON)
    message(STATUS "PetriLab found at '/usr/bin/petrilab'")

    add_executable(IAPrimary IA/IAPrimary.cpp)
    target_link_libraries(IAPrimary Strategy PetriRuntime)
else()
    set(USE_PETRILAB OFF)
endif()

add_executable(IATest IA/IATest.cpp)
target_link_libraries(IATest Strategy)

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

    if(USE_PETRILAB)
        # IAPrimary
        message(STATUS "Compiling IAPrimary for the raspi")
        link_directories(/usr/arm-linux-gnueabihf/lib)
        target_link_libraries(IAPrimary ModuleInterfacers HighLevelFunctions dl)
    else()
        message(WARNING "PetriLab is not installed, please run 'info/scripts/install.sh petri'")
    endif()

    # IATest
    message(STATUS "Compiling IATest for the raspi")
    link_directories(/usr/arm-linux-gnueabihf/lib)
else()
    target_link_libraries(IAWiimote ModuleInterfacers HighLevelFunctions wiicpp)

    if(USE_PETRILAB)
    else()
        message(WARNING "PetriLab is not installed, please run 'info/scripts/install.sh petri'")
    endif()
endif()
