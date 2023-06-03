add_executable(integration_test.stim)
emil_build_for(integration_test.stim TARGET_MCU stm32f767)

target_sources(integration_test.stim PRIVATE
    Main.cpp
)

target_link_libraries(integration_test.stim PRIVATE
    services.util
    hal_st.instantiations
)
