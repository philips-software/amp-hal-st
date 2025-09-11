macro(add_sesame_example target_name)

    add_executable(${target_name})
    emil_build_for(${target_name}
        TARGET_MCU stm32f767
        PREREQUISITE_BOOL HALST_BUILD_EXAMPLES
    )

    protocol_buffer_echo_cpp(${target_name} LedsAndButton.proto)

    target_sources(${target_name} PRIVATE
        Main.cpp
    )

    target_link_libraries(${target_name} PRIVATE
        services.util
        hal_st.instantiations
    )

    halst_target_default_linker_scripts(${target_name})
    halst_target_default_init(${target_name})

    emil_generate_artifacts(TARGET ${target_name} HEX)

endmacro()

add_sesame_example(examples_st.sesame)
add_sesame_example(examples_st.sesame_symmetric)
add_sesame_example(examples_st.sesame_diffie_hellman)

target_compile_definitions(examples_st.sesame_symmetric PUBLIC SECURE_SYMMETRIC)
emil_target_range_source(examples_st.sesame_symmetric ExamplePeerA.key symmetricKey key_material SymmetricKey)

target_compile_definitions(examples_st.sesame_diffie_hellman PUBLIC SECURE_DIFFIE_HELLMAN)
emil_target_range_source(examples_st.sesame_diffie_hellman ExampleRoot.der rootCaCertificate key_material RootCaCertificate)
emil_target_range_source(examples_st.sesame_diffie_hellman ExampleDevice.der deviceCertificate key_material DeviceCertificate)
emil_target_range_source(examples_st.sesame_diffie_hellman ExampleDevice.prv deviceCertificatePrivateKey key_material DeviceCertificatePrivateKey)
