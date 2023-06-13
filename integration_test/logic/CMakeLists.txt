add_library(integration_test.logic ${HALST_EXCLUDE_FROM_ALL})

target_include_directories(integration_test.logic PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
)

protocol_buffer_echo_cpp(integration_test.logic Testing.proto)

target_sources(integration_test.logic PRIVATE
    Gpio.cpp
    Gpio.hpp
)

target_link_libraries(integration_test.logic PUBLIC
    services.util
    protobuf.echo
)
