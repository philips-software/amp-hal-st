add_executable(examples_st.sesame)
emil_build_for(examples_st.sesame
    TARGET_MCU stm32f767
    PREREQUISITE_BOOL HALST_BUILD_EXAMPLES
)

protocol_buffer_echo_cpp(examples_st.sesame LedsAndButton.proto)

target_sources(examples_st.sesame PRIVATE
    Main.cpp
)

target_link_libraries(examples_st.sesame PRIVATE
    services.util
    hal_st.instantiations
)

halst_target_default_linker_scripts(examples_st.sesame)
halst_target_default_init(examples_st.sesame)

emil_generate_artifacts(TARGET examples_st.sesame HEX)
