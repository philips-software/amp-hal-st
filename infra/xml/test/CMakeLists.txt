add_executable(infra.xml_test)
emil_build_for(infra.xml_test BOOL HALST_BUILD_TESTS)
emil_add_test(infra.xml_test)

target_link_libraries(infra.xml_test PUBLIC
    gmock_main
    infra.xml
)

target_sources(infra.xml_test PRIVATE
    TestXmlNavigator.cpp
)
