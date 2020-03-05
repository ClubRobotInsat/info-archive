add_executable(TEST_SERVOS servos.cpp)
target_link_libraries(TEST_SERVOS Strategy)

add_executable(TEST_MOVING moving.cpp)
target_link_libraries(TEST_MOVING Strategy)

add_executable(TEST_COLORDETECTION colorDetection.cpp)
target_link_libraries(TEST_COLORDETECTION Strategy)

add_executable(TEST_IO_PUMPS io_pumps.cpp)
target_link_libraries(TEST_IO_PUMPS Strategy)
