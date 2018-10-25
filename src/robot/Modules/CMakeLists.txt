set(MODULE_SOURCES
        ../../elec/librobot/c_src/SharedWithRust.c

        Module.hpp
        ModuleManager.cpp

        Avoidance2019.cpp
        IO2019.cpp
        Motors2019.cpp
        #Moving2019.cpp
        Servos2019.cpp
        LED2019.cpp)

add_library(Modules STATIC ${MODULE_SOURCES})
target_link_libraries(Modules Outils_Robot)
