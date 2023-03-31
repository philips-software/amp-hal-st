add_subdirectory(std_periph_stm32f0xx)
add_subdirectory(std_periph_stm32f2xx)
add_subdirectory(std_periph_stm32f3xx)
add_subdirectory(std_periph_stm32f4xx)

include(stm32f7xx_hal_driver.cmake)

add_subdirectory(std_periph_stm32wbxx)
add_subdirectory(windows_cmsis_stub)
add_subdirectory(atomic)

add_library(hal_st.std_periph ${HALST_EXCLUDE_FROM_ALL} INTERFACE)
install(TARGETS hal_st.std_periph EXPORT halStTargets)

if ("${TARGET_MCU_FAMILY}" STREQUAL stm32f0xx)
    target_link_libraries(hal_st.std_periph INTERFACE
        hal_st.atomic
    )
endif()

if (EMIL_HOST_BUILD)
    target_link_libraries(hal_st.std_periph INTERFACE
        hal_st.cmsis_windows_stub
    )
else()
    target_link_libraries(hal_st.std_periph INTERFACE
        hal_st.std_periph_${TARGET_MCU_FAMILY}
    )
endif()

target_link_libraries(hal_st.std_periph INTERFACE
    $<TARGET_OBJECTS:hal_st.startup>
)

add_library(hal_st.default_linker_scripts INTERFACE)

target_link_libraries(hal_st.default_linker_scripts INTERFACE
    hal_st.${TARGET_MCU_FAMILY}_default_linker_scripts>
)

add_subdirectory(default_init)

function(halst_target_linker_scripts)
    set(singleArgs TARGET)
    set(multiArgs LINKER_SCRIPTS)
    cmake_parse_arguments(PARSE_ARGV 0 HALST "" "${singleArgs}" "${multiArgs}")

    foreach(script ${HALST_LINKER_SCRIPTS})
        target_link_options(${HALST_TARGET} PUBLIC LINKER:-T${script})
    endforeach()

    set_property(TARGET ${HALST_TARGET} PROPERTY INTERFACE_LINK_DEPENDS ${HALST_LINKER_SCRIPTS})
endfunction()

emil_exclude_directory_from_clang_format(.)
