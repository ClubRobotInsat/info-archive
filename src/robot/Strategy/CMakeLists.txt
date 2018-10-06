add_subdirectory(Environment)

set(STRAT_SOURCE
        AbstractStrategy.cpp)
add_library(Strategy STATIC ${STRAT_SOURCE})
target_link_libraries(Strategy Robot Environment)