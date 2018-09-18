set(COMMUNICATION_SOURCE
        CAN.cpp
        RS232.cpp
        Serie.cpp
        NullCommunicator.cpp
        TCPIP.cpp
        NamedPipe.cpp
        ElecCommunicator.hpp)
add_library(Communication STATIC ${COMMUNICATION_SOURCE})
