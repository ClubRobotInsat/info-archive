cmake_minimum_required(VERSION 3.0)

project(root)

option(TARGET "Architecture cible" pc)
option(COLOR "Sortie en couleur de la compilation" ON)
option(DEBUG "Compilation avec les symboles de debug et sans optimisations" OFF)
set(BITS "32" CACHE . "Build en 32 ou 64 bits")

# Mise à jour des submodules Git.
execute_process(
	COMMAND git submodule update --init
	WORKING_DIRECTORY ${root_SOURCE_DIR}
)
add_custom_target(InfoSubmodules
	COMMAND git submodule update --init
	WORKING_DIRECTORY ${root_SOURCE_DIR}
)

add_subdirectory(petri)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
if(DEBUG)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")
	set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -flto")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -flto")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "^(Apple)?Clang$")
	if (COLOR)
		set(CMAKE_COLOR_MAKEFILE ON)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fcolor-diagnostics")
	else ()
		set(CMAKE_COLOR_MAKEFILE OFF)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-color-diagnostics")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-color-diagnostics")
	endif()

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-stack-address")

else()

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-local-addr")

endif()

if(${BITS} EQUAL 32)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
   set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -m32")
   set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-value -Wuninitialized  -Wunused-parameter -Wunreachable-code -Wno-sign-compare -Woverloaded-virtual -Werror=return-type")

function(compile_petri TARGET_NAME PATH_TO_DOC OUTPUT)
	add_custom_target(${TARGET_NAME}
		COMMAND ${root_SOURCE_DIR}/petri/petri --update --arch ${BITS} ${CMAKE_CURRENT_SOURCE_DIR}/${PATH_TO_DOC}
	)
	add_dependencies(${TARGET_NAME} Editor)
endfunction()

# Il y a du code pour les ombres, apparamment. Ca a pas l'air de marcher.
# debug/CGTK semble désactivé depuis un moment également.
add_definitions(-DGTKWIN_DISABLE)

include_directories(commun /usr/local/include/)
link_directories(/usr/local/lib/)

add_subdirectory(commun)
add_subdirectory(robot)

# Juste lancer cmake avec les bons compilos dans env CC / CXX
add_subdirectory(simulateur)



