add_library(hal_st.instantiations STATIC)
emil_build_for(hal_st.instantiations TARGET_MCU_VENDOR st PREREQUISITE_BOOL HALST_STANDALONE)

target_include_directories(hal_st.instantiations PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(hal_st.instantiations PUBLIC
    hal_st.stm32fxxx
    hal_st.synchronous_stm32fxxx
    infra.event
    infra.timer
    services.tracer
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
    TracingResetStm.cpp
    TracingResetStm.hpp
    FaultTracer.cpp
)
