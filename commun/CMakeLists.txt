

set(OUTILS_SOURCE
	RobotPrincipal/Constantes.cpp
	ConstantesCommunes.cpp
	log/Log.cpp
	log/LogHTML.cpp
	log/LogRTF.cpp
	log/LogTermVT100.cpp
	Units/Units.cpp
	sockets/Socket.cpp
	sockets/tcpSock.cpp
	Trame.cpp
	MathToolbox/MathToolbox.cpp
)

add_library(Outils STATIC ${OUTILS_SOURCE})
target_link_libraries(Outils pthread)
add_dependencies(Outils InfoSubmodules)

add_subdirectory(GLFW)
