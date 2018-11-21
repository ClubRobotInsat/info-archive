add_executable(TEST_SERVOS servos.cpp)
target_link_libraries(TEST_SERVOS Robot)

# On ne peut pas utiliser la librairie "Catch2::Catch2" pour générer un unique fichier de test
add_library(catch_main OBJECT "unit.cpp")
target_include_directories(catch_main PRIVATE "../third_parties/catch/single_include/catch2")


# Génération automatique de tous les exécutables de test
file(GLOB files "unit-*.cpp")
# Suppression de certains fichiers ; voir https://stackoverflow.com/questions/15550777/how-do-i-exclude-a-single-file-from-a-cmake-fileglob-pattern#15551302
list(REMOVE_ITEM files "${CMAKE_SOURCE_DIR}/test/unit-strategy_generator.cpp")

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
# Par défaut, 'Robot' regroupe tout
target_link_libraries(unit_testing_frames Outils_Robot)
target_link_libraries(unit_testing_modules Modules)
target_link_libraries(unit_testing_communication Communication)
target_link_libraries(unit_testing_constants Outils_Robot)
target_link_libraries(unit_testing_repere Outils_Robot)
target_link_libraries(unit_testing_robot Robot)

set(UNITTEST_TARGET_NAME "unit_testing_all")
add_executable(${UNITTEST_TARGET_NAME}
        $<TARGET_OBJECTS:catch_main>
        ${files})

target_include_directories(${UNITTEST_TARGET_NAME} PRIVATE "../third_parties/catch/single_include/catch2")
target_link_libraries(${UNITTEST_TARGET_NAME} Robot)

add_test(NAME "${UNITTEST_TARGET_NAME}"
        COMMAND ${UNITTEST_TARGET_NAME} "*"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
