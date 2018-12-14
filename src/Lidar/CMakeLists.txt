set(LIDAR_DRV_SOURCE
        Driver/hokuyoLidar.cpp
        Driver/lidar.cpp
        Driver/sickLidar.cpp
)
add_library(Lidar STATIC ${LIDAR_DRV_SOURCE})
target_link_libraries(Lidar Robot Outils_Robot usb-1.0)

set(AFF_LIDAR_SOURCE
        affLidar.cpp
        filtre.cpp)
add_executable(affLidar ${AFF_LIDAR_SOURCE})
target_link_libraries(affLidar Lidar glfw)

set(AFF_GRILLE_SOURCE
        affGrille.cpp
        affiche.cpp
        filtre.cpp
        lidarsToGrid.cpp
        FindRobots.cpp)
add_executable(affGrille ${AFF_GRILLE_SOURCE})
target_link_libraries(affGrille Lidar Graphique)


set(LIDAR_SERVER
        affiche.cpp
        filtre.cpp
        lidarServer.cpp
        lidarServerFront.cpp
        lidarsToGrid.cpp
        FindRobots.cpp)
add_executable(lidarServer ${LIDAR_SERVER})
target_link_libraries(lidarServer Lidar Graphique)

add_custom_command(TARGET lidarServer PRE_BUILD
COMMAND ${CMAKE_COMMAND} -E
copy ${CMAKE_SOURCE_DIR}/robot/Lidar/table.tga $<TARGET_FILE_DIR:lidarServer>)
