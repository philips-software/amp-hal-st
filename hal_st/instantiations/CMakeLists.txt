add_library(hal_st.instantiations EXCLUDE_FROM_ALL STATIC)
install(TARGETS hal_st.instantiations EXPORT halStTargets)

target_include_directories(hal_st.instantiations PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(hal_st.instantiations PUBLIC
    hal_st.stm32fxxx
    infra.event
    infra.timer
)

target_sources(hal_st.instantiations PRIVATE
    DiscoveryTracerInfrastructure.hpp
    DiscoveryUi.hpp
    NucleoTracerInfrastructure.hpp
    NucleoUi.hpp
    StmEventInfrastructure.cpp
    StmEventInfrastructure.hpp
    StmTracerInfrastructure.cpp
    StmTracerInfrastructure.hpp
)
