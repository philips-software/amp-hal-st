add_executable(integration_test.runner)
emil_build_for(integration_test.runner HOST Windows)

target_sources(integration_test.runner PRIVATE
    FixtureEcho.cpp
    FixtureEcho.hpp
    Hooks.cpp
    StepsGpio.cpp
    StepsUart.cpp
    Waiting.cpp
    Waiting.hpp
)

target_link_libraries(integration_test.runner PRIVATE
    args
    cucumber-cpp-runner
    integration_test.logic
    hal.generic
    gtest
    gmock
    services.network_instantiations
)
