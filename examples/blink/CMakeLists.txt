add_executable(hal_st.blink_nucleo144 Main.cpp)
emil_build_for(hal_st.blink_nucleo144
    TARGET_MCU stm32f429
    TARGET_MCU stm32f746
    TARGET_MCU stm32f767
    TARGET_MCU stm32h563
    PREREQUISITE_BOOL HALST_BUILD_EXAMPLES
)

target_compile_definitions(hal_st.blink_nucleo144 PUBLIC
    NUCLEO=Nucleo144Ui
)

target_link_libraries(hal_st.blink_nucleo144 PRIVATE
    services.util
    hal_st.instantiations
)

halst_target_default_linker_scripts(hal_st.blink_nucleo144)
halst_target_default_init(hal_st.blink_nucleo144)

emil_generate_artifacts(TARGET hal_st.blink_nucleo144 LST MAP BIN HEX)

######################################################################

add_executable(hal_st.blink_nucleo64 Main.cpp)
emil_build_for(hal_st.blink_nucleo64
    TARGET_MCU stm32g431
    TARGET_MCU stm32g474
    TARGET_MCU stm32wb55
    TARGET_MCU stm32wba52
    PREREQUISITE_BOOL HALST_BUILD_EXAMPLES
)

target_compile_definitions(hal_st.blink_nucleo64 PUBLIC
    $<$<STREQUAL:${TARGET_MCU},stm32wba52>:NUCLEO=Nucleo64WBAUi>
    $<$<STREQUAL:${TARGET_MCU},stm32wb55>:NUCLEO=Nucleo64WBUi>
    $<$<OR:$<STREQUAL:${TARGET_MCU},stm32g431>,$<STREQUAL:${TARGET_MCU},stm32g474>>:NUCLEO=Nucleo64Ui>
)

target_link_libraries(hal_st.blink_nucleo64 PRIVATE
    services.util
    hal_st.instantiations
)

halst_target_default_linker_scripts(hal_st.blink_nucleo64)
halst_target_default_init(hal_st.blink_nucleo64)

emil_generate_artifacts(TARGET hal_st.blink_nucleo64 LST MAP BIN HEX)
