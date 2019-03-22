include_directories(../../../src/robot/Strategy/Environment)
set(STRAT_SOURCE
        ../../../src/robot/Strategy/Environment/environment.cpp
        ../../../src/robot/Strategy/Environment/node.cpp
        ../../../src/robot/Strategy/Environment/shapes.h
        ../../../src/commun/MathToolbox/spline.cpp
        ../../../src/robot/Strategy/Environment/TGAWriter.cpp

        StrategiePrincipal.cpp)

add_library(Strategie STATIC ${STRAT_SOURCE})
target_link_libraries(Strategie)

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
add_subdirectory(Commun)

set(ROBOT_SOURCE
        Commun/Robot.cpp
 		Commun/ModuleInterfacers/Deplacement.cpp
 		Commun/ModuleInterfacers/Ascenseur.cpp
 		Commun/ModuleInterfacers/Moteur.cpp
 		Commun/ModuleInterfacers/Servo.cpp
 		Commun/ModuleInterfacers/Evitement.cpp)

set(ROBOT_INTERFACE_ELEC
 		Commun/Robot.cpp
 		Commun/ModuleInterfacers/Deplacement.cpp
 		Commun/ModuleInterfacers/Ascenseur.cpp
 		Commun/ModuleInterfacers/Moteur.cpp
 		Commun/ModuleInterfacers/Servo.cpp
 		../elec/simulator/Interface.cpp
 		Commun/ModuleInterfacers/Evitement.cpp)

add_library(Robot STATIC ${ROBOT_SOURCE})
add_library(Robot_Interfacer_Elec ../elec/simulator/Interface.cpp)# ${ROBOT_INTERFACE_ELEC})
target_link_libraries(Robot Outils_Robot Strategie Communication Modules)
target_link_libraries(Robot_Interfacer_Elec Outils_Robot Strategie Communication Modules)


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

  	#target_link_libraries(Robot PetriRuntime)

  	# Test A*
    add_executable(AstarTest ${STRAT_SOURCE} Strategy/Environment/test.cpp)
    target_link_libraries(AstarTest Outils_Robot Strategie Robot Cartes)

    add_subdirectory(Principal)
endif()

file(COPY Strategy/Environment/table_strat.json DESTINATION ${CMAKE_BINARY_DIR}/src/robot/commun)
