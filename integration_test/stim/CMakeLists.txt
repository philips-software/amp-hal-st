if ("${TARGET_MCU}" STREQUAL "stm32f767")
    add_executable(integration_test.stim Main.cpp)
    target_link_libraries(integration_test.stim PRIVATE
        services.util
        hal_st.instantiations
    )

    emil_generate_artifacts(TARGET integration_test.stim LST MAP BIN HEX)
endif()
