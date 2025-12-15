add_library(hal_st.default_init STATIC)
emil_build_for(hal_st.default_init TARGET_MCU_VENDOR st PREREQUISITE_BOOL HALST_STANDALONE)

target_include_directories(hal_st.default_init PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(hal_st.default_init PUBLIC
    st.hal_driver
    hal_st.cortex
    hal_st.instantiations
)

# Assembler does not understand -Werror
set_target_properties(hal_st.default_init PROPERTIES COMPILE_WARNING_AS_ERROR Off)

target_sources(hal_st.default_init PRIVATE
    $<$<STREQUAL:${TARGET_MCU_FAMILY},stm32g0xx>:Atomic.cpp>
    DefaultInit.cpp
)

if (TARGET_MCU_VENDOR STREQUAL st)
    get_target_property(startup_source st.hal_driver_${TARGET_MCU_FAMILY} HALST_STARTUP_SOURCE)

    target_sources(hal_st.default_init PRIVATE
        ${startup_source}
    )
endif()

function(halst_target_default_init target)
    target_link_libraries(${target} PUBLIC
        $<TARGET_OBJECTS:hal_st.default_init>
    )
endfunction()
