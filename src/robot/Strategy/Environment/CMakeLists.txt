set(ENVIRONMENT_SOURCES
        environment.cpp
        node.cpp
        shapes.h
        TGAWriter.cpp)

add_library(Environment STATIC ${ENVIRONMENT_SOURCES})
target_link_libraries(Environment Outils_Robot)

add_executable(test_splines test.cpp)
target_link_libraries(test_splines Environment)
