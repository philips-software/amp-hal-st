add_executable(hal_st.blink_nucleo144 Main.cpp)
emil_build_for(hal_st.blink_nucleo144 PREREQUISITE_BOOL HALST_BUILD_EXAMPLES)
set_target_properties(hal_st.blink_nucleo144 PROPERTIES EXCLUDE_FROM_ALL $<NOT:$<STREQUAL:${TARGET_MCU_VENDOR},st>>)

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
emil_build_for(hal_st.blink_nucleo64 PREREQUISITE_BOOL HALST_BUILD_EXAMPLES)
set_target_properties(hal_st.blink_nucleo64 PROPERTIES EXCLUDE_FROM_ALL $<NOT:$<STREQUAL:${TARGET_MCU_VENDOR},st>>)

target_compile_definitions(hal_st.blink_nucleo64 PUBLIC
    NUCLEO=Nucleo64Ui
)

target_link_libraries(hal_st.blink_nucleo64 PRIVATE
    services.util
    hal_st.instantiations
)

halst_target_default_linker_scripts(hal_st.blink_nucleo64)
halst_target_default_init(hal_st.blink_nucleo64)

emil_generate_artifacts(TARGET hal_st.blink_nucleo64 LST MAP BIN HEX)
