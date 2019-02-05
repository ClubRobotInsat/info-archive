add_subdirectory(Environment)
add_subdirectory(ModuleInterfacers)
add_subdirectory(PetriLab)

set(STRAT_SOURCE
        AbstractStrategy.cpp)
add_library(Strategy STATIC ${STRAT_SOURCE})
target_link_libraries(Strategy ModuleInterfacers)
