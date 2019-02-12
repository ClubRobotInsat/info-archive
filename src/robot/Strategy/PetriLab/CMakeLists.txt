set(HIGH_LEVEL_FUNCTIONS_SOURCE
        Avoidance.cpp
        Navigation.cpp
        Servos.cpp
        Utils.cpp)
add_library(HighLevelFunctions STATIC ${HIGH_LEVEL_FUNCTIONS_SOURCE})

# Dossier de génération de code de pétri.
execute_process(
        COMMAND mkdir -p ${root_SOURCE_DIR}/src/robot/Strategy/PetriLab/src
)