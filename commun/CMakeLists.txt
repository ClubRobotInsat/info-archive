

set(OUTILS_SOURCE
	2015/RobotPrincipal/Constantes2015.cpp
	2015/RobotSecondaire/Constantes2015.cpp
	2015/ConstantesCommunes2015.cpp
	log/Log.cpp
	log/LogHTML.cpp
	log/LogRTF.cpp
	log/LogTermVT100.cpp
	sockets/Socket.cpp
	Trame.cpp
	MathToolbox/MathToolbox.cpp
)

add_library(Outils STATIC ${OUTILS_SOURCE})
target_link_libraries(Outils pthread)

add_subdirectory(GLFW)
