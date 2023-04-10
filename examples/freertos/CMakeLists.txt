if (${HALST_BUILD_EXAMPLES_FREERTOS})
    add_executable(hal_st.freertos_nucleo144 Main.cpp)
    set_target_properties(hal_st.freertos_nucleo144 PROPERTIES EXCLUDE_FROM_ALL $<NOT:$<STREQUAL:${TARGET_MCU_VENDOR},st>>)

    target_link_libraries(hal_st.freertos_nucleo144 PRIVATE
        hal_st.instantiations
        osal.thread
        osal.freertos_system_time
    )

    halst_target_default_linker_scripts(hal_st.freertos_nucleo144)
    halst_target_default_init(hal_st.freertos_nucleo144)

    emil_generate_artifacts(TARGET hal_st.freertos_nucleo144 LST MAP BIN HEX)
endif()
