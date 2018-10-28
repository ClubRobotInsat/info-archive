set(COMMUNICATION_SOURCE
        RS232.cpp
        Serial.cpp
        NullCommunicator.cpp
        TCPIP.cpp
        NamedPipe.cpp
        Communicator.hpp)
add_library(Communication STATIC ${COMMUNICATION_SOURCE})
