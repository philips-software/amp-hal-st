add_library(integration_test.logic ${HALST_EXCLUDE_FROM_ALL})

protocol_buffer_echo_cpp(integration_test.logic Testing.proto)

#target_sources(integration_test.logic PRIVATE
#)

target_link_libraries(integration_test.logic PUBLIC
    services.util
    protobuf.echo
)
