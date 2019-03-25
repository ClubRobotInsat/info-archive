set(HIGH_LEVEL_FUNCTIONS_SOURCE
        Avoidance.cpp
        IO.cpp
        Navigation.cpp
        Pumps.cpp
        Servos.cpp
        Utils.cpp)
add_library(HighLevelFunctions STATIC ${HIGH_LEVEL_FUNCTIONS_SOURCE})

# Dossier de génération de code de pétri.
execute_process(
        COMMAND mkdir -p ${root_SOURCE_DIR}/src/robot/Strategy/PetriLab/src
)
if(EXISTS "/usr/bin/petrilab")
    execute_process(
            COMMAND petrilab -u Template.petri
            WORKING_DIRECTORY ${root_SOURCE_DIR}/src/robot/Strategy/PetriLab
    )
endif()
