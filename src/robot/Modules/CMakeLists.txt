set(MODULE_SOURCES
        ../../elec/librobot/c_src/SharedWithRust.c

        Module.hpp
        ModuleManager.cpp

        Avoidance.cpp
        IO.cpp
        Motors.cpp
        #Moving.cpp
        Servos.cpp)

add_library(Modules STATIC ${MODULE_SOURCES})
target_link_libraries(Modules Outils_Robot)
