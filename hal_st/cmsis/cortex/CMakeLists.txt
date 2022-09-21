add_library(hal_st.cmsis_cortex EXCLUDE_FROM_ALL STATIC)
install(TARGETS hal_st.cmsis_cortex EXPORT halStTargets)

target_include_directories(hal_st.cmsis_cortex PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_sources(hal_st.cmsis_cortex PRIVATE
    exception_handlers.c
    exception_handlers.h
)
