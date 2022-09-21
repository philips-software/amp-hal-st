add_library(hal_st.cortex EXCLUDE_FROM_ALL STATIC)
install(TARGETS hal_st.cortex EXPORT halStTargets)

target_include_directories(hal_st.cortex PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(hal_st.cortex PUBLIC
    hal_st.std_periph
    infra.event
)

target_sources(hal_st.cortex PRIVATE
    EventDispatcherCortex.cpp
    InterruptCortex.cpp
    EventDispatcherCortex.hpp
    InterruptCortex.hpp
)
