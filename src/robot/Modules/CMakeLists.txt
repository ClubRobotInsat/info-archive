set(MODULE_SOURCES
        Module.hpp
        ModuleManager.cpp

        IO.cpp
        Motors.cpp
        Navigation.cpp
        Pumps.cpp
        Servos.cpp)

add_library(Modules STATIC ${MODULE_SOURCES})
target_link_libraries(Modules Outils_Robot)
