SET(SIMU_OU_ROBOT Robot)

if(${BITS} EQUAL 32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
	set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -m32")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32")
endif()

add_subdirectory(../commun commun)

add_subdirectory(Modules)
add_subdirectory(Communication)

set(ROBOT_SOURCE
		Robot.cpp)
add_library(Robot STATIC ${ROBOT_SOURCE})
target_link_libraries(Robot Modules Communication)
