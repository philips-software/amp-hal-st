add_executable(integration_test.tested)
emil_build_for(integration_test.tested TARGET_MCU stm32f767)

target_sources(integration_test.tested PRIVATE
    EchoFromTester.hpp
    Main.cpp
    Tested.cpp
    Tested.hpp
)

target_link_libraries(integration_test.tested PRIVATE
    integration_test.logic
    hal_st.instantiations
)

emil_generate_artifacts(TARGET integration_test.tested BIN)

halst_target_default_linker_scripts(integration_test.tested)
halst_target_default_init(integration_test.tested)
