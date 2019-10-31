set(MODULE_SOURCES
        Module.hpp
        ModuleManager.cpp
        Captors.cpp
        IO.cpp
        Motors.cpp
        Navigation.cpp
        NavigationParameters.cpp
        Pumps.cpp
        Servos.cpp
        LED.cpp)

add_library(Modules STATIC ${MODULE_SOURCES})
target_link_libraries(Modules Outils_Robot)
