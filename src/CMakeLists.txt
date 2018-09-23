include_directories(commun)

add_subdirectory(robot)
add_subdirectory(simulateur)
add_subdirectory(elec)

file(COPY robot.ini DESTINATION ${CMAKE_BINARY_DIR}/src)
