set(LIDAR_DRV_SOURCE
        Driver/hokuyoLidar.cpp
        Driver/lidar.cpp
        Driver/sickLidar.cpp
)
add_library(LidarDriver STATIC ${LIDAR_DRV_SOURCE})
target_link_libraries(LidarDriver Robot Outils_Robot usb-1.0)

set(AFF_LIDAR_SOURCE
        affLidar.cpp
        filtre.cpp)
add_executable(affLidar ${AFF_LIDAR_SOURCE})
target_link_libraries(affLidar LidarDriver glfw)

set(LIDAR_CORE_SOURCE
        filtre.cpp
        FindRobots.cpp
        lidarsToGrid.cpp)
add_library(Lidar STATIC ${LIDAR_CORE_SOURCE})
target_link_libraries(Lidar LidarDriver)

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

#add_custom_command(TARGET lidarServer PRE_BUILD
#COMMAND ${CMAKE_COMMAND} -E
#copy ${CMAKE_SOURCE_DIR}/robot/Lidar/table.tga $<TARGET_FILE_DIR:lidarServer>)
