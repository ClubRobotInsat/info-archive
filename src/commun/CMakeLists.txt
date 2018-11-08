set(OUTILS_SOURCE
        AsciiArt.hpp
        Constants.cpp
        EmbeddedFiles.cpp
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
        ../../third_parties/Units/Units.cpp)

add_library(Outils_Robot STATIC ${OUTILS_SOURCE})
target_link_libraries(Outils_Robot pthread)
add_dependencies(Outils_Robot InfoSubmodules)

add_library(Outils_Simu STATIC ${OUTILS_SOURCE})
target_link_libraries(Outils_Simu pthread)
add_dependencies(Outils_Simu InfoSubmodules)

#file(COPY table_2017.json table_2018.json DESTINATION ${CMAKE_BINARY_DIR}/src)

# Cette fonction prends un fichier normal et le rends #include-able pour s'affranchir du fichier table_2018.json
# Le pré-processeur copie automagiquement le fichier dans un std::string
message("-- Creating storage file")
file(WRITE "embedded" "")

function(include_file path)
    file(READ ${path} content)
    set(delim "for_c++_include")
    set(content "R\"${delim}(\n${content})${delim}\"")
    file(APPEND "embedded" "{ \"${path}\", ${content} },\n")
endfunction(include_file)

include_file(table_2018.json)
include_file(../robot.ini)
#target_compile_definitions(Outils_${SIMU_OU_ROBOT} PRIVATE INI_FILE_PATH="${CMAKE_BINARY_DIR}/src/robot.ini")
