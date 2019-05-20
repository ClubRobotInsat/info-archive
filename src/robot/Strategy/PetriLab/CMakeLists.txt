set(HIGH_LEVEL_FUNCTIONS_SOURCE
        Avoidance.cpp
        IO.cpp
        Navigation.cpp
        PumpsPrimary.cpp
        PumpsSecondary.cpp
        ServosPrimary.cpp
        ServosSecondary.cpp
        Utils.cpp)
add_library(HighLevelFunctions STATIC ${HIGH_LEVEL_FUNCTIONS_SOURCE})

# Dossier de génération de code de pétri.
execute_process(
        COMMAND mkdir -p ${root_SOURCE_DIR}/src/robot/Strategy/PetriLab/src
)
if(EXISTS "/usr/bin/petrilab")
    if(RASPI)
        set(PETRILAB_PROFILE "ARM release")
    else()
        set(PETRILAB_PROFILE "Default")
    endif()
    execute_process(
            COMMAND petrilab --update --profile ${PETRILAB_PROFILE} Primary.petri
            WORKING_DIRECTORY ${root_SOURCE_DIR}/src/robot/Strategy/PetriLab
    )
    execute_process(
            COMMAND petrilab --update --profile ${PETRILAB_PROFILE} Secondary.petri
            WORKING_DIRECTORY ${root_SOURCE_DIR}/src/robot/Strategy/PetriLab
    )
endif()
