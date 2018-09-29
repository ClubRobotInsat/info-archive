add_executable(unit_testing_modules unit-modules.cpp)
target_link_libraries(unit_testing_modules Modules Catch2::Catch2)

add_executable(unit_testing_communication unit-communication.cpp)
target_link_libraries(unit_testing_communication Communication Catch2::Catch2)

add_executable(unit_testing_robot_commun unit-robot_commun.cpp)
target_link_libraries(unit_testing_robot_commun Robot Catch2::Catch2)

add_executable(unit_ini unit-ini.cpp)# ../third_parties/ini/include/inicpp.h)
target_link_libraries(unit_ini Catch2::Catch2)

# add_library(catch_main OBJECT "unit.cpp")
# set_target_properties(catch_main PROPERTIES
#         CXX_STANDARD 11
#         CXX_STANDARD_REQUIRED ON
#         COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
#         COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
#         )
# target_include_directories(catch_main PRIVATE "../third_parties/catch/single_include/catch2")
#
#
# add_executable(unit_testing_modules $<TARGET_OBJECTS:catch_main> unit-modules_elec.cpp)
# target_include_directories(unit_testing_modules PRIVATE "../third_parties/catch/single_include/catch2")
# target_link_libraries(unit_testing_modules Trame_Robot Modules)
#
# add_test(NAME "test_modules"
#         COMMAND unit_testing_modules
#         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
#
# file(GLOB files "unit-*.cpp")
#
# set(UNITTEST_TARGET_NAME "unit_testing")
# add_executable(${UNITTEST_TARGET_NAME}
#         $<TARGET_OBJECTS:catch_main>
#         ${files})
#
# target_include_directories(${UNITTEST_TARGET_NAME} PRIVATE "../third_parties/catch/single_include/catch2")
# target_link_libraries(${UNITTEST_TARGET_NAME} Outils_Robot robotPrincipal)
#
# add_test(NAME "${UNITTEST_TARGET_NAME}_default"
#         COMMAND ${UNITTEST_TARGET_NAME}
#         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
#
# add_test(NAME "${UNITTEST_TARGET_NAME}_all"
#         COMMAND ${UNITTEST_TARGET_NAME} "*"
#         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
