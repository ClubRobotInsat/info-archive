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

        physique/IPhysicalContext.h
        physique/IPhysicalInstance.h
        physique/PhysicalObjectDefinition.cpp
        physique/PhysicalObject.cpp
        physique/Box2DPhysicalContext.cpp
        graphique/IGraphicalContext.h
        graphique/IGraphicalInstance.h
        graphique/WebGraphicalContext.cpp
        graphique/WebObjectHandler.cpp
        core/Object3D.cpp

        communication/cartes/CarteAsservissement2009.cpp
        communication/cartes/CarteCAN_USB.cpp
        communication/cartes/CarteDeplacement2009.cpp
        communication/cartes/CarteDetectAdv2009.cpp
        communication/cartes/CarteIO2014.cpp
        communication/cartes/deplacement/Asservissement.cpp
        communication/cartes/deplacement/PID.cpp

        ../commun/communication/Communicateur.cpp
        ../commun/communication/CommunicateurSerie.cpp
        ../commun/communication/CommunicateurTCPIP.cpp

        main.cpp

        communication/cartes/Carte.cpp
        communication/Robot2017.cpp
        communication/modules/ModuleContactor.cpp
        communication/modules/ModuleMove.cpp
        communication/modules/ModuleServos.cpp

        core/Simulateur.cpp

        core/World.cpp
        core/World2017.cpp

        ${root_SOURCE_DIR}/third_parties/petri/Runtime/Cpp/detail/jsoncpp/src/lib_json/json_reader.cpp
        ${root_SOURCE_DIR}/third_parties/petri/Runtime/Cpp/detail/jsoncpp/src/lib_json/json_value.cpp
        ${root_SOURCE_DIR}/third_parties/petri/Runtime/Cpp/detail/jsoncpp/src/lib_json/json_writer.cpp)

target_link_libraries(Simu2017 Box2D Outils_Simu Server)

