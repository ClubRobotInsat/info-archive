SET(SIMU_OU_ROBOT Simu)

add_subdirectory(../commun commun)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
cmake_minimum_required(VERSION 3.2)

link_libraries(pthread)
add_subdirectory(graphique/server)
include_directories(SYSTEM ${root_SOURCE_DIR}/third_parties/websocketpp)
include_directories(SYSTEM ${root_SOURCE_DIR}/third_parties/asio/asio/include)
include_directories(${root_SOURCE_DIR}/third_parties/petri/Runtime/Cpp/detail/jsoncpp/include)

# Obtention d'Irrlicht
FIND_PACKAGE(Irrlicht)
# A faire : messages d'erreurs si irrlicht pas trouvé
include_directories(${IRRLICHT_INCLUDE_DIRS})

add_executable(Simu2017
        main.cpp

        core/Simulateur.cpp
        core/World.cpp
        core/WorldIO.cpp
        core/World2017.cpp
        core/Object3D.cpp

        physique/IPhysicalContext.h
        physique/IPhysicalInstance.h
        physique/box2d/PhysicalObjectDefinition.cpp
        physique/box2d/PhysicalObject.cpp
        physique/box2d/Box2DPhysicalContext.cpp

        graphique/IGraphicalContext.h
        graphique/IGraphicalInstance.h
        graphique/IGraphicalUserListener.h
        graphique/server/WebGraphicalContext.cpp
        graphique/server/WebObjectHandler.cpp
        graphique/irrlicht/SimulationToIrrlicht.h
        graphique/irrlicht/Object.cpp
        graphique/irrlicht/Scene.cpp

        communication/cartes/CarteAsservissement2009.cpp
        communication/cartes/CarteCAN_USB.cpp
        communication/cartes/CarteDeplacement2009.cpp
        communication/cartes/CarteDetectAdv2009.cpp
        communication/cartes/CarteIO2014.cpp
        communication/cartes/deplacement/Asservissement.cpp
        communication/cartes/deplacement/PID.cpp

        communication/cartes/Carte.cpp
        communication/Robot2017.cpp
        communication/modules/ModuleContactor.cpp
        communication/modules/ModuleMove.cpp
        communication/modules/ModuleServos.cpp

        ../commun/communication/Communicateur.cpp
        ../commun/communication/CommunicateurSerie.cpp
        ../commun/communication/CommunicateurTCPIP.cpp

        ${root_SOURCE_DIR}/third_parties/petri/Runtime/Cpp/detail/jsoncpp/src/lib_json/json_reader.cpp
        ${root_SOURCE_DIR}/third_parties/petri/Runtime/Cpp/detail/jsoncpp/src/lib_json/json_value.cpp
        ${root_SOURCE_DIR}/third_parties/petri/Runtime/Cpp/detail/jsoncpp/src/lib_json/json_writer.cpp)

target_link_libraries(Simu2017 Box2D Outils_Simu Server ${IRRLICHT_LIBRARIES})

file(COPY core/table_simu.json DESTINATION ${CMAKE_BINARY_DIR}/src/simulateur)
