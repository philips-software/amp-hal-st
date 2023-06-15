add_library(hal_st.cortex STATIC)
emil_build_for(hal_st.cortex TARGET_MCU_VENDOR st PREREQUISITE_BOOL HALST_STANDALONE)
install(TARGETS hal_st.cortex EXPORT halStTargets)

target_include_directories(hal_st.cortex PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(hal_st.cortex PUBLIC
    st.hal_driver
    infra.event
)

target_sources(hal_st.cortex PRIVATE
    EventDispatcherCortex.cpp
    InterruptCortex.cpp
    EventDispatcherCortex.hpp
    InterruptCortex.hpp
)
