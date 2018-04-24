set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14 -std=gnu++14 -Wdeprecated")

set(SOURCE_FILES_ELEC_SIMULATOR mbed.h DigitalOut.h PwmOut.h Serial.cpp Serial.h CircularBuffer.cpp CircularBuffer.h
        Can.cpp Can.h Callback.cpp Callback.h FunctionPointer.h InterruptIn.h PinMode.h mbed.cpp)

add_library(mbed-simulated ${SOURCE_FILES_ELEC_SIMULATOR})