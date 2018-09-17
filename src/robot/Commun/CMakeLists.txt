include_directories(Environnement)
set(STRAT_SOURCE
        Environnement/environment.cpp
        Environnement/node.cpp
        Environnement/shapes.h
        Environnement/spline.cpp
        Environnement/TGAWriter.cpp

        Strategie.cpp)

add_library(Strategie STATIC ${STRAT_SOURCE})
# target_link_libraries(Strategie)

set(COMMUNICATION_SOURCE
        Communication/CAN.cpp
        Communication/RS232.cpp
        Communication/Serie.cpp
        Communication/NullCommunicator.cpp
        Communication/TCPIP.cpp
        Communication/NamedPipe.cpp)
add_library(Communication STATIC ${COMMUNICATION_SOURCE})

add_subdirectory(Modules)

add_library(Modules STATIC "ElecCommunicator.cpp")
target_link_libraries(Modules ModulesElec Communication)


# add_executable(debug_TestCAN Commun/Test/TestCan.cpp Cartes/Carte.cpp Commun/Test/CarteTestCAN.cpp)
# target_link_libraries(debug_TestCAN Robot)
# add_executable(debug_TestServo Commun/Test/TestServo.cpp Cartes/Carte.cpp Cartes/CarteServosNova2017.cpp)
# target_link_libraries(debug_TestServo Robot)
# add_executable(debug_TestContacteur Commun/Test/TestContacteur.cpp Cartes/Carte.cpp Cartes/CarteIO2018.cpp)
# target_link_libraries(debug_TestContacteur Robot)
# add_executable(debug_TestAscenseur Commun/Test/TestAscenseur.cpp Cartes/Carte.cpp Cartes/CarteAsservissement2009.cpp)
# target_link_libraries(debug_TestAscenseur Robot)
# add_executable(debug_TesteurRS232 Commun/Test/Testeur_RS232.cpp)
# target_link_libraries(debug_TesteurRS232 Robot)
# add_executable(debug_TesteurCAN Commun/Test/Testeur_CAN.cpp)
# target_link_libraries(debug_TesteurCAN Robot)
# add_executable(debug_TestRS23 Commun/Test/TestAscenseur.cpp)
# target_link_libraries(debug_TestRS23 robotPrincipal)