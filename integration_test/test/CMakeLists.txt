add_executable(integration_test.test)
emil_build_for(integration_test.test HOST Windows)

target_sources(integration_test.test PRIVATE
    FixtureEcho.cpp
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
    gtest
    gmock
    services.network_instantiations
)

if (EMIL_BUILD_WIN)
    target_link_libraries(integration_test.test PRIVATE hal.windows)
endif()

if (EMIL_BUILD_UNIX OR EMIL_BUILD_DARWIN)
    target_link_libraries(integration_test.test PRIVATE hal.unix)
endif()
