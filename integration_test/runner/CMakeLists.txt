add_executable(integration_test.runner)
emil_build_for(integration_test.runner HOST Windows)

target_sources(integration_test.runner PRIVATE
    FixtureEcho.cpp
    FixtureEcho.hpp
    Hooks.cpp
    Main.cpp
    StepsGpio.cpp
    StepsUart.cpp
)

target_link_libraries(integration_test.runner PRIVATE
    args
    cucumber-cpp
    integration_test.logic
    hal.generic
    gtest
    gmock
    services.network_instantiations
)

if (EMIL_BUILD_WIN)
    target_link_libraries(integration_test.runner PRIVATE hal.windows)
endif()

if (EMIL_BUILD_UNIX OR EMIL_BUILD_DARWIN)
    target_link_libraries(integration_test.runner PRIVATE hal.unix)
endif()
