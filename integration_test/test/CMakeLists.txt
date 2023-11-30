add_executable(integration_test.test)
emil_build_for(integration_test.test HOST All)

target_sources(integration_test.test PRIVATE
    FixtureEcho.hpp
    FixtureEventDispatcher.hpp
    FixtureEventDispatcher.cpp
    FixtureSystemChanges.hpp
    Hooks.cpp
    Main.cpp
    RunInSync.cpp
    RunInSync.hpp
    StepsGpio.cpp
)

target_link_libraries(integration_test.test PRIVATE
    cucumber-cpp
    cucumber-cpp.report
    gherkin-cpp
    integration_test.logic
    hal.windows
    gtest
    gmock
)
