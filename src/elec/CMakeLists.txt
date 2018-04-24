add_subdirectory(mbed)
add_subdirectory(Interface_Can)

set(SOURCE_FILES_NUCLEO
        code-elec-nucleo/src/lib/Servo.cpp
        code-elec-nucleo/src/lib/QEI.cpp
        code-elec-nucleo/src/herkulex/HerkulexServo.cpp
        code-elec-nucleo/src/herkulex/HerkulexBus.cpp
        code-elec-nucleo/src/main.cpp
        code-elec-nucleo/src/Brushless.cpp
        code-elec-nucleo/src/Motor.cpp
        code-elec-nucleo/src/Robot.cpp
        code-elec-nucleo/src/SimpleMotor.cpp
        code-elec-nucleo/src/Trame.cpp
        code-elec-nucleo/src/TrameReader.cpp
        mbed/mbed.h)

include_directories(mbed)
include_directories(code-elec-nucleo/src/lib)
include_directories(code-elec-nucleo/src/herkulex)

add_executable(nucleo ${SOURCE_FILES_NUCLEO})
target_link_libraries(nucleo mbed-simulated)
target_compile_definitions(nucleo PRIVATE SIMULATED_MAIN)