add_executable(hal_st.freertos_nucleo144 Main.cpp)
emil_build_for(hal_st.freertos_nucleo144 TARGET_MCU_VENDOR st PREREQUISITE_BOOL HALST_BUILD_EXAMPLES_FREERTOS)

target_link_libraries(hal_st.freertos_nucleo144 PRIVATE
    hal_st.instantiations
    osal.thread
    osal.freertos_system_time
)

halst_target_default_linker_scripts(hal_st.freertos_nucleo144)
halst_target_default_init(hal_st.freertos_nucleo144)

emil_generate_artifacts(TARGET hal_st.freertos_nucleo144 LST MAP BIN HEX)
