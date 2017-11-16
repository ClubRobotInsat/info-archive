SET(SIMU_OU_ROBOT Simu)

add_subdirectory(../commun commun)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
cmake_minimum_required(VERSION 3.2)

link_libraries(pthread)
add_subdirectory(graphique/server)
include_directories(SYSTEM ${root_SOURCE_DIR}/third_parties/websocketpp)
include_directories(SYSTEM ${root_SOURCE_DIR}/third_parties/asio/asio/include)
include_directories(${root_SOURCE_DIR}/third_parties/petri/Runtime/Cpp/detail/jsoncpp/include)

add_executable(Simu2017

        physique/PhysicalObjectDefinition.cpp
        physique/PhysicalObjectDefinition.h
        physique/PhysicalObject.cpp
        physique/PhysicalObject.h
        graphique/Cube.cpp
        graphique/Cube.h
        graphique/Cylinder.cpp
        graphique/Cylinder.h
        core/Object3D.cpp
        core/Object3D.h
        graphique/Sphere.cpp
        graphique/Sphere.h

        communication/cartes/CarteAsservissement2009.cpp
        communication/cartes/CarteAsservissement2009.h
        communication/cartes/CarteCAN_USB.cpp
        communication/cartes/CarteCAN_USB.h
        communication/cartes/CarteDeplacement2009.cpp
        communication/cartes/CarteDeplacement2009.h
        communication/cartes/CarteDetectAdv2009.cpp
        communication/cartes/CarteDetectAdv2009.h
        communication/cartes/CarteIO2014.cpp
        communication/cartes/CarteIO2014.h
        communication/cartes/deplacement/Asservissement.cpp
        communication/cartes/deplacement/Asservissement.h
        communication/cartes/deplacement/PID.cpp
        communication/cartes/deplacement/PID.h

        ../commun/communication/Communicateur.cpp
        ../commun/communication/Communicateur.h
        ../commun/communication/CommunicateurSerie.cpp
        ../commun/communication/CommunicateurSerie.h
        ../commun/communication/CommunicateurTCPIP.cpp
        ../commun/communication/CommunicateurTCPIP.h

        main.cpp

        core/PhysicalToSimulationUnits.h

        communication/cartes/Carte.h
        communication/cartes/Carte.cpp
        graphique/robot/FormeRobot.cpp
        graphique/robot/FormeRobot.h
        communication/Robot2017.cpp
        communication/Robot2017.h
        communication/RobotLogic.h
        communication/RobotLogic.hpp
        communication/modules/ModuleContactor.cpp
        communication/modules/ModuleContactor.h
        communication/modules/ModuleMove.cpp
        communication/modules/ModuleMove.h
        communication/modules/ModuleServos.cpp
        communication/modules/ModuleServos.h

        core/Simulateur.cpp
        core/Simulateur.h
        core/SimulateurConstantes.h

        core/World.cpp
        core/World.h
        core/World2017.cpp
        core/World2017.h

        ${root_SOURCE_DIR}/third_parties/petri/Runtime/Cpp/detail/jsoncpp/src/lib_json/json_reader.cpp
        ${root_SOURCE_DIR}/third_parties/petri/Runtime/Cpp/detail/jsoncpp/src/lib_json/json_value.cpp
        ${root_SOURCE_DIR}/third_parties/petri/Runtime/Cpp/detail/jsoncpp/src/lib_json/json_writer.cpp)

target_link_libraries(Simu2017 Box2D Outils_Simu Server)

