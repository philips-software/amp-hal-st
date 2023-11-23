add_executable(integration_test.test EXCLUDE_FROM_ALL)
emil_build_for(integration_test.test HOST All)

target_sources(integration_test.test PRIVATE
    fixtures/Fixture.hpp
    hooks/Hooks.cpp
    steps/Steps.cpp
)

target_link_libraries(integration_test.test PRIVATE
    cucumber-cpp-runner
    integration_test.logic
)
