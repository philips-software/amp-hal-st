add_subdirectory(STM32CubeWB)
add_subdirectory(STM32CubeWBA)

add_library(hal_st.stm32_wpan INTERFACE)
emil_build_for(hal_st.stm32_wpan TARGET_MCU_VENDOR st)

if (TARGET_MCU_VENDOR STREQUAL st)
    target_link_libraries(hal_st.stm32_wpan INTERFACE
        hal_st.stm32_wpan_${TARGET_MCU_FAMILY}
    )
endif()
