if ("${TARGET_MCU}" STREQUAL "stm32f767" AND ${HALST_BUILD_EXAMPLES_FREERTOS})
    add_executable(hal_st.freertos Main.cpp)
    target_link_libraries(hal_st.freertos PRIVATE
        hal_st.instantiations
        osal.thread
        osal.freertos_system_time
    )
endif()
