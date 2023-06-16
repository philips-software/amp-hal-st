add_executable(integration_test.tester)
emil_build_for(integration_test.tester TARGET_MCU stm32f767)

target_sources(integration_test.tester PRIVATE
    Main.cpp
    EchoFromCloud.hpp
    ForwardingEchoToTested.cpp
    ForwardingEchoToTested.hpp
    Tester.cpp
    Tester.hpp
)

target_link_libraries(integration_test.tester PRIVATE
    integration_test.logic
    hal_st.instantiations
)

halst_target_default_linker_scripts(integration_test.tester)
halst_target_default_init(integration_test.tester)
