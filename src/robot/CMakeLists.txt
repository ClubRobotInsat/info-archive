SET(SIMU_OU_ROBOT Robot)

# Dossier de génération de code de pétri.
execute_process(
	COMMAND mkdir -p ${root_SOURCE_DIR}/src/robot/Principal/petri/src
)

if(${BITS} EQUAL 32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
	set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -m32")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32")
endif()

add_subdirectory(../commun commun)

include_directories(Commun/Environnement)
set(STRAT_SOURCE
        Commun/Environnement/environment.cpp
        Commun/Environnement/environment.h
        Commun/Environnement/node.cpp
        Commun/Environnement/node.h
        Commun/Environnement/shapes.h
        Commun/Environnement/spline.cpp
        Commun/Environnement/spline.h
        Commun/Environnement/TGAWriter.cpp
        Commun/Environnement/TGAWriter.h

        Commun/Strategie.cpp)

add_library(Strategie STATIC ${STRAT_SOURCE})
target_link_libraries(Strategie)

set(COMMUNICATION_SOURCE
		Commun/Communication/CAN.cpp
		Commun/Communication/RS232.cpp
		Commun/Communication/Serie.cpp
		Commun/Communication/NullCommunicator.cpp
		Commun/Communication/TCPIP.cpp
		Commun/Communication/NamedPipe.cpp
		Commun/Modules/Module.hpp
		Commun/Modules/ModuleManager.cpp
        Commun/Modules/ModuleIO2019.cpp
		Commun/Modules/ModuleMotors2019.cpp
		Commun/Modules/ModuleMoving2019.cpp
		Commun/Modules/ModuleServos2019.cpp
		)
add_library(SharedWithRust Commun/Modules/SharedWithRust.c)

set(ROBOT_SOURCE
        Commun/Robot.cpp
        Commun/Deplacement.cpp
        Commun/Ascenseur.cpp
		Commun/Moteur.cpp
        Commun/Servo.cpp
        Commun/Evitement.cpp)

set(ROBOT_INTERFACE_ELEC
		Commun/Robot.cpp
		Commun/Deplacement.cpp
		Commun/Ascenseur.cpp
		Commun/Moteur.cpp
		Commun/Servo.cpp
		../elec/simulator/Interface.cpp
		Commun/Evitement.cpp)

add_library(Robot STATIC ${ROBOT_SOURCE} ${COMMUNICATION_SOURCE})
add_library(Robot_Interfacer_Elec ${ROBOT_INTERFACE_ELEC} ${COMMUNICATION_SOURCE})
target_link_libraries(Robot Outils_Robot Strategie SharedWithRust)
target_link_libraries(Robot_Interfacer_Elec Outils_Robot Strategie SharedWithRust)

add_executable(debug_TestCAN Commun/Test/TestCan.cpp Cartes/Carte.cpp Commun/Test/CarteTestCAN.cpp)
target_link_libraries(debug_TestCAN Robot)
add_executable(debug_TestServo Commun/Test/TestServo.cpp Cartes/Carte.cpp Cartes/CarteServosNova2017.cpp)
target_link_libraries(debug_TestServo Robot)
add_executable(debug_TestContacteur Commun/Test/TestContacteur.cpp Cartes/Carte.cpp Cartes/CarteIO2018.cpp)
target_link_libraries(debug_TestContacteur Robot)
add_executable(debug_TestAscenseur Commun/Test/TestAscenseur.cpp Cartes/Carte.cpp Cartes/CarteAsservissement2009.cpp)
target_link_libraries(debug_TestAscenseur Robot)
add_executable(debug_TesteurRS232 Commun/Test/Testeur_RS232.cpp)
target_link_libraries(debug_TesteurRS232 Robot)
add_executable(debug_TesteurCAN Commun/Test/Testeur_CAN.cpp)
target_link_libraries(debug_TesteurCAN Robot)
add_executable(debug_TestRS23 Commun/Test/TestAscenseur.cpp)
target_link_libraries(debug_TestRS23 robotPrincipal)

#add_subdirectory(Lidar)

#include_directories(libcwiimote/src)
add_definitions(-D_ENABLE_TILT -D_ENABLE_FORCE)
#add_subdirectory(libcwiimote)

if (${TARGET} STREQUAL rasPi)
else ()
	add_subdirectory(Cartes)

  	target_link_libraries(Robot PetriRuntime)

  	# Test A*
    add_executable(AstarTest ${STRAT_SOURCE} Commun/Environnement/test.cpp)
    target_link_libraries(AstarTest Outils_Robot Strategie Robot Cartes)

    add_subdirectory(Principal)
endif()

file(COPY Commun/Environnement/table_strat.json DESTINATION ${CMAKE_BINARY_DIR}/src/robot/commun)
