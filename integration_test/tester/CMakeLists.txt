set_source_files_properties(
    ForwardingEchoToTested.cpp
    ForwardingEchoToTested.hpp
    Main.cpp
    Tester.cpp
    Tester.hpp

    PROPERTIES COMPILE_FLAGS
        "$<$<CXX_COMPILER_ID:GNU>:-Wno-deprecated-declarations>$<$<CXX_COMPILER_ID:Clang>:-Wno-deprecated-declarations>$<$<CXX_COMPILER_ID:AppleClang>:-Wno-deprecated-declarations>$<$<CXX_COMPILER_ID:MSVC>:/wd4996>"
)

######################################################################

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
    services.st_util
)

emil_generate_artifacts(TARGET integration_test.tester BIN)

halst_target_default_linker_scripts(integration_test.tester)
halst_target_default_init(integration_test.tester)
