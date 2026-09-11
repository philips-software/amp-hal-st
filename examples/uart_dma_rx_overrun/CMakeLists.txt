add_executable(examples_st.uart_dma_rx_overrun Main.cpp)
emil_build_for(examples_st.uart_dma_rx_overrun
    TARGET_MCU stm32wba65
    PREREQUISITE_BOOL HALST_BUILD_EXAMPLES
)

target_link_libraries(examples_st.uart_dma_rx_overrun PRIVATE
    services.util
    hal_st.instantiations
)

halst_target_default_linker_scripts(examples_st.uart_dma_rx_overrun)
halst_target_default_init(examples_st.uart_dma_rx_overrun)

emil_generate_artifacts(TARGET examples_st.uart_dma_rx_overrun LST MAP BIN HEX)
