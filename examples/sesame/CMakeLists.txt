add_executable(hal_st.sesame)
emil_build_for(hal_st.sesame
    TARGET_MCU stm32f767
    PREREQUISITE_BOOL HALST_BUILD_EXAMPLES
)

protocol_buffer_echo_cpp(hal_st.sesame LedsAndButton.proto)

target_sources(hal_st.sesame PRIVATE
    Main.cpp
)

target_link_libraries(hal_st.sesame PRIVATE
    services.util
    hal_st.instantiations
)

halst_target_default_linker_scripts(hal_st.sesame)
halst_target_default_init(hal_st.sesame)

emil_generate_artifacts(TARGET hal_st.sesame HEX)
