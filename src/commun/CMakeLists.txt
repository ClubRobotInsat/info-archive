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
        sockets/Socket.cpp
        sockets/tcpSock.cpp
        communication/GlobalFrame.cpp
        communication/Trame.cpp
        ../../third_parties/Units/Units.cpp)

add_library(Outils_${SIMU_OU_ROBOT} STATIC ${OUTILS_SOURCE})
target_link_libraries(Outils_${SIMU_OU_ROBOT} pthread)
add_dependencies(Outils_${SIMU_OU_ROBOT} InfoSubmodules)

target_compile_definitions(Outils_${SIMU_OU_ROBOT} PRIVATE INI_FILE_PATH="${CMAKE_BINARY_DIR}/src/robot.ini")

#file(COPY table_2017.json table_2018.json DESTINATION ${CMAKE_BINARY_DIR}/src)

# Cette fonction prends un fichier normal et le rends #include-able pour s'affranchir du fichier table_2018.json
# Le pré-processeur copie automagiquement le fichier dans un std::string
function(make_includable input_file output_file)
    file(READ ${input_file} content)
    set(delim "for_c++_include")
    set(content "R\"${delim}(\n${content})${delim}\"")
    file(WRITE ${output_file} "${content}")
endfunction(make_includable)

make_includable(table_2018.json .table_2018.json.includable)
