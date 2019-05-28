set(INTERFACERS_SOURCES
        Interfacer.hpp
        AtomType.h
        AvoidanceInterfacer.cpp
        ElevatorInterfacer.cpp
        IOInterfacer.cpp
        NavigationInterfacer.cpp
        PumpsInterfacerPrimary.cpp
        PumpsInterfacerSecondary.cpp
        ServosInterfacer.cpp
        RobotManager.cpp)

add_library(ModuleInterfacers STATIC ${INTERFACERS_SOURCES})
target_link_libraries(ModuleInterfacers Robot Environment)
