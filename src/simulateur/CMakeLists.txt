if (RASPI)
    message(STATUS "No need to compile simulator for raspi")
    return()
endif(RASPI)


link_libraries(pthread)
#add_subdirectory(graphique/server)
#include_directories(SYSTEM ${root_SOURCE_DIR}/third_parties/websocketpp)
include_directories(SYSTEM ${root_SOURCE_DIR}/third_parties/asio/asio/include)
include_directories(${root_SOURCE_DIR}/third_parties/petri/Runtime/Cpp/detail/jsoncpp/include)

# Obtention d'Irrlicht
FIND_PACKAGE(Irrlicht)
if (NOT ${IRRLICHT_FOUND})
    message(FATAL_ERROR "Irrlicht was not found. Simulator cannot be built.")
endif()

include_directories(${IRRLICHT_INCLUDE_DIRS})
message(STATUS "Irrlicht trouvé ici : ${IRRLICHT_INCLUDE_DIRS}")

add_library(LibSimu
        core/Simulateur.cpp
        core/SimuGuiClient.cpp
        core/World.cpp
        core/WorldIO.cpp
        core/Object3D.cpp

        physique/IPhysicalContext.h
        physique/IPhysicalInstance.h
        physique/box2d/PhysicalObjectDefinition.cpp
        physique/box2d/PhysicalObject.cpp
        physique/box2d/Box2DPhysicalContext.cpp
        physique/box2d/RobotController.cpp
        physique/box2d/pid/Asservissement.cpp
        physique/box2d/pid/PID.cpp

        graphique/IGraphicalContext.h
        graphique/IGraphicalInstance.h
        graphique/IGraphicalUserListener.h
        graphique/irrlicht/SimulationToIrrlicht.h
        graphique/irrlicht/Object.cpp
        graphique/irrlicht/Scene.cpp
        #graphique/server/WebGraphicalContext.cpp
        #graphique/server/WebObjectHandler.cpp

        # TODO Enlever ModuleManager quand il sera dans librairies incluses
        ../robot/Modules/ModuleManager.cpp
        core/SimuRobot.cpp

        communication/SimuCommunicator.cpp
        communication/SimuServos.cpp
        communication/SimuLed.cpp
        communication/SimuNavigation.cpp)

add_subdirectory(gui/gtk)

target_link_libraries(LibSimu GtkSimu Communication Outils_Simu)
target_link_libraries(LibSimu Box2D ${IRRLICHT_LIBRARIES})

add_executable(Simulator main.cpp)
target_link_libraries(Simulator LibSimu)
