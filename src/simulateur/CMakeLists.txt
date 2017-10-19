SET(SIMU_OU_ROBOT Simu)

add_subdirectory(../commun commun)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
cmake_minimum_required(VERSION 3.2)

link_libraries(pthread)
add_subdirectory(server)
include_directories(SYSTEM ${root_SOURCE_DIR}/third_parties/websocketpp)
include_directories(SYSTEM ${root_SOURCE_DIR}/third_parties/asio/asio/include)
include_directories(${root_SOURCE_DIR}/third_parties/petri/Runtime/Cpp/detail/jsoncpp/include)

add_executable(Simu2017

        3DObject/box2D/PhysicalObjectDefinition.cpp
        3DObject/box2D/PhysicalObjectDefinition.h
        3DObject/box2D/PhysicalObject.cpp
        3DObject/box2D/PhysicalObject.h
        3DObject/Cube.cpp
        3DObject/Cube.h
        3DObject/Cylinder.cpp
        3DObject/Cylinder.h
        3DObject/Object3D.cpp
        3DObject/Object3D.h
        3DObject/Sphere.cpp
        3DObject/Sphere.h

        cartes/CarteAsservissement2009.cpp
        cartes/CarteAsservissement2009.h
        cartes/CarteCAN_USB.cpp
        cartes/CarteCAN_USB.h
        cartes/CarteDeplacement2009.cpp
        cartes/CarteDeplacement2009.h
        cartes/CarteDetectAdv2009.cpp
        cartes/CarteDetectAdv2009.h
        cartes/CarteIO2014.cpp
        cartes/CarteIO2014.h
        cartes/deplacement/Asservissement.cpp
        cartes/deplacement/Asservissement.h
        cartes/deplacement/PID.cpp
        cartes/deplacement/PID.h

        ../commun/communication/Communicateur.cpp
        ../commun/communication/Communicateur.h
        ../commun/communication/CommunicateurSerie.cpp
        ../commun/communication/CommunicateurSerie.h
        ../commun/communication/CommunicateurTCPIP.cpp
        ../commun/communication/CommunicateurTCPIP.h

        main.cpp

        PhysicalToSimulationUnits.h

        robot/Carte.h
        robot/Carte.cpp
        robot/FormeRobot.cpp
        robot/FormeRobot.h
        robot/Robot2017.cpp
        robot/Robot2017.h
        robot/RobotLogic.h
        robot/RobotLogic.hpp
        robot/modules/ModuleContactor.cpp
        robot/modules/ModuleContactor.h
        robot/modules/ModuleMove.cpp
        robot/modules/ModuleMove.h
        robot/modules/ModuleServos.cpp
        robot/modules/ModuleServos.h

        Simulateur.cpp
        Simulateur.h
        SimulateurConstantes.h

        world/World.cpp
        world/World.h
        world/World2017.cpp
        world/World2017.h

        ${root_SOURCE_DIR}/third_parties/petri/Runtime/Cpp/detail/jsoncpp/src/lib_json/json_reader.cpp
        ${root_SOURCE_DIR}/third_parties/petri/Runtime/Cpp/detail/jsoncpp/src/lib_json/json_value.cpp
        ${root_SOURCE_DIR}/third_parties/petri/Runtime/Cpp/detail/jsoncpp/src/lib_json/json_writer.cpp)

target_link_libraries(Simu2017 Box2D Outils_Simu Server)

