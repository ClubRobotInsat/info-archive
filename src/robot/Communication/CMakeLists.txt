set(COMMUNICATION_SOURCE
        RS232.cpp
        Serial.cpp
        NullCommunicator.cpp
        TCPIP.cpp
        NamedPipe.cpp
        UDP.cpp
        Communicator.hpp
        Protocol.hpp
        CommunicatorParsing.hpp)
add_library(Communication STATIC ${COMMUNICATION_SOURCE})
target_link_libraries(Communication Outils_Robot)