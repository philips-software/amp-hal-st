add_executable(integration_test.test)
emil_build_for(integration_test.test HOST Windows)

target_sources(integration_test.test PRIVATE
    FixtureEcho.hpp
    Hooks.cpp
    StepsGpio.cpp
    StepsUart.cpp
    Waiting.cpp
    Waiting.hpp
)

target_link_libraries(integration_test.test PRIVATE
    args
    cucumber-cpp-runner
    integration_test.logic
    hal.generic
    hal.windows
    gtest
    gmock
)
