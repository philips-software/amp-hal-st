add_executable(integration_test.tested)
emil_build_for(integration_test.tested TARGET_MCU stm32f767 stm32wb55)

target_sources(integration_test.tested PRIVATE
    EchoFromTester.hpp
    $<$<STREQUAL:${TARGET_MCU},stm32f767>:MainStm32f767.cpp>
    $<$<STREQUAL:${TARGET_MCU},stm32wb55>:MainStm32wb55.cpp>
    Tested.cpp
    Tested.hpp
)

target_link_libraries(integration_test.tested PRIVATE
    integration_test.logic
    hal_st.instantiations
)

halst_target_default_linker_scripts(integration_test.tested)
halst_target_default_init(integration_test.tested)
