add_subdirectory(mbed)
add_subdirectory(Interface_Can)

set(SOURCE_FILES_SIMULATOR
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
        code-elec-nucleo/src/Utils.cpp
        simulator/Serial.cpp
        simulator/Interface.cpp
        simulator/Simulator.cpp
        mbed/mbed.h)

include_directories(mbed)
include_directories(code-elec-nucleo/src/lib)
include_directories(code-elec-nucleo/src/herkulex)
include_directories(simulator)


add_library(IATestElecAndRobot ../robot/Principal/IA/IATestRobot.cpp)
target_link_libraries(IATestElecAndRobot robotPrincipalInterfaceElec)
target_compile_definitions(IATestElecAndRobot PRIVATE -DSIMULATED_MAIN)


add_executable(elec_simulator ${SOURCE_FILES_SIMULATOR})
target_compile_definitions(elec_simulator PRIVATE -DSIMULATED_MAIN)
target_link_libraries(elec_simulator mbed-simulated pthread IATestElecAndRobot)
