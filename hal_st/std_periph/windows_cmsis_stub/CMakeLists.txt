add_library(hal_st.cmsis_windows_stub EXCLUDE_FROM_ALL INTERFACE)
install(TARGETS hal_st.cmsis_windows_stub EXPORT halStCmsisArmTargets)

target_include_directories(hal_st.cmsis_windows_stub INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_compile_definitions(hal_st.cmsis_windows_stub INTERFACE
    # For syntax recognition
    STM32F746xx
)

target_sources(hal_st.cmsis_windows_stub PRIVATE
    cmsis_device.h
)
