add_executable(hal_st.blink_nucleo144 Main.cpp)
set_target_properties(hal_st.blink_nucleo144 PROPERTIES EXCLUDE_FROM_ALL $<NOT:$<STREQUAL:${TARGET_MCU_VENDOR},st>>)

target_link_libraries(hal_st.blink_nucleo144 PRIVATE
    services.util
    hal_st.instantiations
)

halst_target_default_linker_scripts(hal_st.blink_nucleo144)
halst_target_default_init(hal_st.blink_nucleo144)

emil_generate_artifacts(TARGET hal_st.blink_nucleo144 LST MAP BIN HEX)
