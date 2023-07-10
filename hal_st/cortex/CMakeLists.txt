add_library(hal_st.cortex ${HALST_EXCLUDE_FROM_ALL} STATIC)

target_include_directories(hal_st.cortex PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(hal_st.cortex PUBLIC
    st.hal_driver
    infra.event
)

target_sources(hal_st.cortex PRIVATE
    InterruptCortex.cpp
    InterruptCortex.hpp
)
