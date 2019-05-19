add_subdirectory(RealWorld)

# On ne peut pas utiliser la librairie "Catch2::Catch2" pour générer un unique fichier de test
add_library(catch_main OBJECT "unit.cpp")
target_include_directories(catch_main PRIVATE "../third_parties/catch/single_include/catch2")

# Génération automatique de tous les exécutables de test
file(GLOB files "unit-*.cpp")
# Suppression de certains fichiers ; voir https://stackoverflow.com/questions/15550777/how-do-i-exclude-a-single-file-from-a-cmake-fileglob-pattern#15551302
list(REMOVE_ITEM files "${CMAKE_SOURCE_DIR}/test/unit-strategy_generator.cpp")

add_library(test_utils STATIC TestUtils.cpp)

if(EXISTS "/usr/bin/petrilab")
    add_library(petri_utils STATIC PetriSample.cpp)

    if(RASPI)
        set(PETRILAB_PROFILE "ARM release")
        target_link_libraries(petri_utils PetriRuntime dl)
    else()
        set(PETRILAB_PROFILE "Default")
        target_link_libraries(petri_utils PetriRuntime)
    endif()

    add_definitions(-DPETRILAB_EXISTS)
    execute_process(
            COMMAND petrilab --update --profile ${PETRILAB_PROFILE} Sample.petri
            WORKING_DIRECTORY ${root_SOURCE_DIR}/test
    )

    target_link_libraries(test_utils Strategy petri_utils)
else()
    list(REMOVE_ITEM files "${CMAKE_SOURCE_DIR}/test/unit-petri.cpp")
endif()

if(RASPI)
    list(REMOVE_ITEM files "${CMAKE_SOURCE_DIR}/test/unit-simu_modules.cpp")
endif()

foreach(file ${files})
    get_filename_component(file_basename ${file} NAME_WE)
    string(REGEX REPLACE "unit-([^$]+)" "unit_testing_\\1" testcase ${file_basename})

    add_executable(${testcase} $<TARGET_OBJECTS:catch_main> ${file})
    target_include_directories(${testcase} PRIVATE "../third_parties/catch/single_include/catch2")
    target_compile_definitions(${testcase} PRIVATE CATCH_CONFIG_FAST_COMPILE)

    add_test(NAME "${testcase}"
            COMMAND ${testcase} "*"
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    set_tests_properties("${testcase}" PROPERTIES LABELS "all")
endforeach()

# Linkage de chaque exécutable par rapport à ses librairies nécessaires
# Par défaut, 'Strategy' regroupe tout
target_link_libraries(unit_testing_frames Outils_Robot)
target_link_libraries(unit_testing_modules Modules Strategy test_utils)
target_link_libraries(unit_testing_communication Communication)
target_link_libraries(unit_testing_constants Outils_Robot)
target_link_libraries(unit_testing_repere Outils_Robot)
target_link_libraries(unit_testing_robot Robot)
target_link_libraries(unit_testing_servos_conversions Modules)
target_link_libraries(unit_testing_interfacers ModuleInterfacers)
target_link_libraries(unit_testing_strategy Strategy test_utils)
if(EXISTS "/usr/bin/petrilab")
    target_link_libraries(unit_testing_petri Strategy test_utils)
endif()

# Tests spéciaux (avec des problèmes de compatibilité c++17
add_executable(unit_testing14_simulator_gtk $<TARGET_OBJECTS:catch_main> unit14-simulator-gtk.cpp)
target_link_libraries(unit_testing14_simulator_gtk GtkSimu)
set_property(TARGET unit_testing14_simulator_gtk PROPERTY CXX_STANDARD 14)
target_include_directories(unit_testing14_simulator_gtk PRIVATE "../third_parties/catch/single_include/catch2")
target_compile_definitions(unit_testing14_simulator_gtk PRIVATE CATCH_CONFIG_FAST_COMPILE)
add_test(NAME "unit_testing14_simulator_gtk"
        COMMAND unit_testing14_simulator_gtk "*"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

# Ajout d'un executable qui lance tous les tests
set(UNITTEST_TARGET_NAME "unit_testing_all")
add_executable(${UNITTEST_TARGET_NAME}
        $<TARGET_OBJECTS:catch_main>
        ${files})
target_include_directories(${UNITTEST_TARGET_NAME} PRIVATE "../third_parties/catch/single_include/catch2")

if(RASPI)
    target_link_libraries(${UNITTEST_TARGET_NAME} Strategy test_utils)
else()
    target_link_libraries(unit_testing_simu_modules LibSimu Modules)
    target_link_libraries(${UNITTEST_TARGET_NAME} Strategy test_utils LibSimu)
endif()

add_test(NAME "${UNITTEST_TARGET_NAME}"
        COMMAND ${UNITTEST_TARGET_NAME} "*"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
