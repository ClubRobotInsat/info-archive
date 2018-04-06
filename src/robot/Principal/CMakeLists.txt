
set(ROBOT_PRINC_SOURCE
        ../Commun/Strategie.cpp
        librobot/Robot.cpp
        librobot/MecaManager.cpp
        librobot/Strategie.cpp
        #librobot/Ascenseur.cpp
        petri/Deplacement.cpp
        petri/Servo.cpp
        petri/Utils.cpp
        petri/PetriCommon.cpp petri/Servo.cpp petri/Servo.h)

include_directories(${root_SOURCE_DIR}/../third-parties/petri)

#Commenté car n'existe plus
#compile_petri(TEST_PETRI petri/Test.petri Test.so)
compile_petri(IA_PETRI petri/IA2018.petri IA2018.so)
#compile_petri(HOMOLOGATION_PETRI petri/Homologation.petri Homologation.so)

add_library(robotPrincipal STATIC ${ROBOT_PRINC_SOURCE})
#add_dependencies(robotPrincipal HOMOLOGATION_PETRI)
target_link_libraries(robotPrincipal Cartes Robot Strategie)

#add_executable(debug_controle_clavier debug/ControleClavier.cpp)
#target_link_libraries(debug_controle_clavier robotPrincipal glfw_Robot)

add_executable(debug_calibration_depla debug/CalibrationDepla.cpp)
target_compile_options(debug_calibration_depla PUBLIC -Wno-deprecated)
target_link_libraries(debug_calibration_depla robotPrincipal)

#compile plus
#add_executable(coupe_off_controle_wiimote_sans_meca coupe_off/ControleWiimoteSansMeca.cpp)
#target_link_libraries(coupe_off_controle_wiimote_sans_meca robotPrincipal)

#Ajout de Wiicpp de manière "cmakienne"
#find_package(Wiicpp REQUIRED)
#if (WIICPP_FOUND)
#	include_directories(${WIICPP_INCLUDE_DIR})
#	message(WARNING "Wiicpp location" : ${WIICPP_INCLUDE_DIR})
#endif(WIICPP_FOUND)

include_directories(/usr/local/include/wiic/)
#find_library(WIICPP_LIBRARY
#		NAMES libwiicpp.so
#		PATH /usr/arm-linux-gnueabihf/lib
#		[NO_DEFAULT_PATH]
#		[NO_CMAKE_ENVIRONMENT_PATH]
#		[NO_CMAKE_PATH]
#		[NO_SYSTEM_ENVIRONMENT_PATH]
#		[NO_CMAKE_SYSTEM_PATH]
#		[NO_CMAKE_FIND_ROOT_PATH])
#cmake_policy(SET CMP0060 NEW)
set(wiimote_sources IA/IAWiimote.cpp wiimote/Wiimote.cpp)
add_executable(IAWiimote ${wiimote_sources})
include_directories(/usr/local/include/wiic/)

if (RASPI)
	message(STATUS "Compiling IAWiimote for the raspi")

	add_library(libwiimotecpp SHARED IMPORTED)
	add_library(libwiimotec SHARED IMPORTED)
	set_target_properties(libwiimotecpp PROPERTIES IMPORTED_LOCATION "../../../precompiled-libraries/libwiicpp.so")
	set_target_properties(libwiimotec PROPERTIES IMPORTED_LOCATION "../../../precompiled-libraries/libwiic.so")

	message(STATUS "Je vais chercher libwiimote dans ${PROJECT_SOURCE_DIR}/precompiled-libraries/libwiicpp.so")

	#link_directories(${PROJECT_SOURCE_DIR}/precompiled-libraries/)
	#add_subdirectory(precompiled-libraries)
	target_link_libraries(IAWiimote robotPrincipal libwiimotecpp libwiimotec)
else()
	target_link_libraries(IAWiimote robotPrincipal wiicpp)
endif()


add_executable(IATestDeplacement IA/IATestDeplacement.cpp)
target_link_libraries(IATestDeplacement robotPrincipal)

add_executable(IATestMeca IA/IATestMeca.cpp)
target_link_libraries(IATestMeca robotPrincipal)

add_executable(IATestSimu IA/IATestSimu.cpp)
target_link_libraries(IATestSimu robotPrincipal)

add_executable(IATestEvitement IA/IATestEvitement.cpp)
target_link_libraries(IATestEvitement robotPrincipal)

add_executable(IAPrincipal IA/IAPrincipal.cpp)
target_link_libraries(IAPrincipal robotPrincipal)
#add_dependencies(IAPrincipal IA_PETRI HOMOLOGATION_PETRI)

add_executable(IATestLidar IA/IATestLidar.cpp)
target_link_libraries(IATestLidar robotPrincipal)

add_executable(IATestRobot IA/IATestRobot.cpp)
target_link_libraries(IATestRobot robotPrincipal)

link_libraries(pthread)
add_executable(TestStrategyGenerator librobot/StrategyGenerator/Action.hpp
									 librobot/StrategyGenerator/Element.hpp
									 librobot/StrategyGenerator/Table.hpp
									 librobot/StrategyGenerator/MagicStrategy.cpp
        							 librobot/StrategyGenerator/DecisionalTree.hpp
									 librobot/StrategyGenerator/main.cpp)
target_link_libraries(TestStrategyGenerator Outils_Robot)
