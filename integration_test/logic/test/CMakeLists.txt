add_executable(integration_test.logic.test)
emil_build_for(integration_test.logic.test BOOL HALST_BUILD_TESTS)
emil_add_test(integration_test.logic.test)

target_link_libraries(integration_test.logic.test PUBLIC
    gmock_main
    integration_test.logic
    protobuf.test_doubles
)

target_sources(integration_test.logic.test PRIVATE
    TestFlash.cpp
)
