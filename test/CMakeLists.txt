add_library(catch_main OBJECT "unit.cpp")
set_target_properties(catch_main PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
        COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
        COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
        )
target_include_directories(catch_main PRIVATE "../third_parties/catch/single_include")

file(GLOB files "unit-*.cpp")

set(UNITTEST_TARGET_NAME "unit_testing")
add_executable(${UNITTEST_TARGET_NAME}
        $<TARGET_OBJECTS:catch_main>
        ${files})

target_include_directories(${UNITTEST_TARGET_NAME} PRIVATE "../third_parties/catch/single_include")
target_link_libraries(${UNITTEST_TARGET_NAME} Outils_Robot robotPrincipal)

add_test(NAME "${UNITTEST_TARGET_NAME}_default"
        COMMAND ${UNITTEST_TARGET_NAME}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_test(NAME "${UNITTEST_TARGET_NAME}_all"
        COMMAND ${UNITTEST_TARGET_NAME} "*"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})