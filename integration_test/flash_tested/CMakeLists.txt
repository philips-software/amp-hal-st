add_library(integration_test.flash_tested ${HALST_EXCLUDE_FROM_ALL})

target_include_directories(integration_test.flash_tested PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
)

protocol_buffer_echo_cpp(integration_test.flash_tested Flash.proto)

target_sources(integration_test.flash_tested PRIVATE
    Flash.cpp
    Flash.hpp
)

target_link_libraries(integration_test.flash_tested PUBLIC
    services.util
    protobuf.echo
)

add_subdirectory(test)
