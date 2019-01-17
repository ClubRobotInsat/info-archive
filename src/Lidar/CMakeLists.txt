set(LIDAR_DRV_SOURCE
        Driver/hokuyoLidar.cpp
        Driver/lidar.cpp
        Driver/sickLidar.cpp
)
add_library(LidarDriver STATIC ${LIDAR_DRV_SOURCE})
if(RASPI)
    message(STATUS "Compiling Lidar for the raspi")

    add_library(libusb-1.0 SHARED IMPORTED)
    set_target_properties(libusb-1.0 PROPERTIES IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/precompiled-libraries/libusb-1.0.so")

    target_link_libraries(LidarDriver Robot Outils_Robot libusb-1.0)
else()
    target_link_libraries(LidarDriver Robot Outils_Robot usb-1.0)

    # GLFW-based applications
    set(AFF_LIDAR_SOURCE
            affLidar.cpp
            filtre.cpp)
    add_executable(affLidar ${AFF_LIDAR_SOURCE})
    target_link_libraries(affLidar LidarDriver glfw)

    set(AFF_GRILLE_SOURCE
            affGrille.cpp
            Display.cpp)
    add_executable(affGrille ${AFF_GRILLE_SOURCE})
    target_link_libraries(affGrille Lidar glfw)

    set(LIDAR_SERVER
            Display.cpp
            lidarServer.cpp
            lidarServerFront.cpp)
    add_executable(lidarServer ${LIDAR_SERVER})
    target_link_libraries(lidarServer Lidar glfw)
endif()

set(LIDAR_CORE_SOURCE
        filtre.cpp
        FindRobots.cpp
        lidarsToGrid.cpp)
add_library(Lidar STATIC ${LIDAR_CORE_SOURCE})
target_link_libraries(Lidar LidarDriver)

set(PRINT_ROBOTS_SOURCE
        PrintRobots.cpp)
add_executable(printRobots ${PRINT_ROBOTS_SOURCE})
target_link_libraries(printRobots Lidar)

#add_custom_command(TARGET lidarServer PRE_BUILD
#COMMAND ${CMAKE_COMMAND} -E
#copy ${CMAKE_SOURCE_DIR}/robot/Lidar/table.tga $<TARGET_FILE_DIR:lidarServer>)
