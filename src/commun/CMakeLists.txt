set(OUTILS_SOURCE
        ConstantesCommunes.cpp
        Json.h
        log/Log.cpp
        log/LogHTML.cpp
        log/LogRTF.cpp
        log/LogTermVT100.cpp
        MathToolbox/MathToolbox.cpp
        MathToolbox/Repere.cpp
        RobotPrincipal/Constantes.cpp
        sockets/Socket.cpp
        sockets/tcpSock.cpp
        Trame.cpp
        ../../third_parties/Units/Units.cpp)

add_library(Outils_${SIMU_OU_ROBOT} STATIC ${OUTILS_SOURCE})
target_link_libraries(Outils_${SIMU_OU_ROBOT} pthread)
add_dependencies(Outils_${SIMU_OU_ROBOT} InfoSubmodules)

file(COPY table_2017.json table_2018.json DESTINATION ${CMAKE_BINARY_DIR}/src)
