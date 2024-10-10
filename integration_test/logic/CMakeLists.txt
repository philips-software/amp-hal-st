add_library(integration_test.logic ${HALST_EXCLUDE_FROM_ALL})

target_include_directories(integration_test.logic PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
)

protocol_buffer_echo_cpp(integration_test.logic Testing.proto)
protocol_buffer_echo_cpp(integration_test.logic Flash.proto)

target_sources(integration_test.logic PRIVATE
    Flash.cpp
    Flash.hpp
    Gpio.cpp
    Gpio.hpp
    Peripheral.cpp
    Peripheral.hpp
    Tested.cpp
    Tested.hpp
    Tester.cpp
    Tester.hpp
    Uart.cpp
    Uart.hpp
)

target_link_libraries(integration_test.logic PUBLIC
    services.util
    protobuf.echo
)

add_subdirectory(test)
