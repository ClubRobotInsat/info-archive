set(MODULE_SOURCES
        SharedWithRust.c

        Module.hpp
        ModuleManager.cpp

        Avoidance2019.cpp
        IO2019.cpp
        Motors2019.cpp
        Moving2019.cpp
        Servos2019.cpp)

add_library(ModulesElec STATIC ${MODULE_SOURCES})
target_link_libraries(ModulesElec Outils_Robot)