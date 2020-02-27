add_subdirectory(resources)

set(OUTILS_SOURCE
        AsciiArt.hpp
        Constants.cpp
        Json.h
        log/Log.cpp
        log/LogHTML.cpp
        log/LogRTF.cpp
        log/LogTermVT100.cpp
        MathToolbox/MathToolbox.cpp
        MathToolbox/Repere.cpp
        MathToolbox/spline.cpp
        sockets/Socket.cpp
        sockets/tcpSock.cpp
        communication/GlobalFrame.cpp
        communication/Trame.cpp
        ../../third_parties/include/Units/Units.cpp)



add_library(Outils_Robot STATIC ${OUTILS_SOURCE})
target_link_libraries(Outils_Robot pthread Resources)
add_dependencies(Outils_Robot InfoSubmodules)

add_library(Outils_Simu STATIC ${OUTILS_SOURCE})
target_link_libraries(Outils_Simu pthread Resources)
add_dependencies(Outils_Simu InfoSubmodules)

#file(COPY table_2017.json table_2018.json DESTINATION ${CMAKE_BINARY_DIR}/src)
include_file(table_2019.json)
include_file(table_2020.json)
include_file(../robot.ini)
#target_compile_definitions(Outils_${SIMU_OU_ROBOT} PRIVATE INI_FILE_PATH="${CMAKE_BINARY_DIR}/src/robot.ini")
