set(INTERFACERS_SOURCES
        Interfacer.hpp
        Avoidance.cpp
        Elevator.cpp
        ServosManager.cpp
        GlobalManager.cpp)

add_library(ModuleInterfacers STATIC ${INTERFACERS_SOURCES})
target_link_libraries(ModuleInterfacers Modules)
