if ("${TARGET_MCU}" STREQUAL "stm32f767")
    add_executable(hal_st.blink Main.cpp)
    target_link_libraries(hal_st.blink PRIVATE
        services.util
        hal_st.instantiations
    )

    emil_generate_artifacts(TARGET hal_st.blink LST MAP BIN HEX)
endif()
