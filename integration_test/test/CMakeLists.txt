add_executable(integration_test.test)
emil_build_for(integration_test.test HOST Windows)

target_sources(integration_test.test PRIVATE
    FixtureEcho.hpp
    Hooks.cpp
    StepsGpio.cpp
    Waiting.cpp
    Waiting.hpp
)

target_link_libraries(integration_test.test PRIVATE
    cucumber-cpp-runner
    gherkin-cpp
    integration_test.logic
    hal.generic
    hal.windows
    gtest
    gmock
)
