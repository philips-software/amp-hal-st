add_executable(integration_test.flash_tested.test)
emil_build_for(integration_test.flash_tested.test BOOL HALST_BUILD_TESTS)
emil_add_test(integration_test.flash_tested.test)

target_link_libraries(integration_test.flash_tested.test PUBLIC
    gmock_main
    integration_test.flash_tested
    protobuf.test_doubles
)

target_sources(integration_test.flash_tested.test PRIVATE
    TestFlash.cpp
)
