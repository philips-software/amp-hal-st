add_executable(integration_test.tester)
emil_build_for(integration_test.tester TARGET_MCU stm32f767)

protocol_buffer_echo_cpp(integration_test.tester Tester.proto)

target_sources(integration_test.tester PRIVATE
    Main.cpp
)

target_link_libraries(integration_test.tester PRIVATE
    services.util
    hal_st.instantiations
    protobuf.echo
)
