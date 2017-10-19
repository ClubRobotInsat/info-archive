include_directories(SYSTEM ../../../third_parties/asio/asio/include)
include_directories(SYSTEM ../../../third_parties/websocketpp)

set(SERVER_LIB_SRC
        SimuWebSocketServer.cpp
        SimuWebSocketServer.h
)

add_library(Server STATIC ${SERVER_LIB_SRC})
link_libraries(pthread)
