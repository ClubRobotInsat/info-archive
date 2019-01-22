set(INTERFACERS_SOURCES
        Interfacer.hpp
        Elevator.cpp
        ServosManager.cpp)

add_library(ModuleInterfacers STATIC ${INTERFACERS_SOURCES})
target_link_libraries(ModuleInterfacers Modules)
