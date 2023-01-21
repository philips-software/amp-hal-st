add_subdirectory(std_periph_stm32f0xx)
add_subdirectory(std_periph_stm32f2xx)
add_subdirectory(std_periph_stm32f3xx)
add_subdirectory(std_periph_stm32f4xx)
add_subdirectory(std_periph_stm32f7xx)
add_subdirectory(std_periph_stm32wbxx)
add_subdirectory(windows_cmsis_stub)
add_subdirectory(atomic)

add_library(hal_st.std_periph EXCLUDE_FROM_ALL INTERFACE)
install(TARGETS hal_st.std_periph EXPORT halStTargets)

target_link_libraries(hal_st.std_periph INTERFACE
    $<$<STREQUAL:${TARGET_MCU_FAMILY},stm32f0xx>:hal_st.atomic>
    $<$<STREQUAL:${TARGET_MCU_FAMILY},stm32f0xx>:hal_st.std_periph_stm32f0xx>
    $<$<STREQUAL:${TARGET_MCU_FAMILY},stm32f2xx>:hal_st.std_periph_stm32f2xx>
    $<$<STREQUAL:${TARGET_MCU_FAMILY},stm32f3xx>:hal_st.std_periph_stm32f3xx>
    $<$<STREQUAL:${TARGET_MCU_FAMILY},stm32f4xx>:hal_st.std_periph_stm32f4xx>
    $<$<STREQUAL:${TARGET_MCU_FAMILY},stm32f7xx>:hal_st.std_periph_stm32f7xx>
    $<$<STREQUAL:${TARGET_MCU_FAMILY},stm32wbxx>:hal_st.std_periph_stm32wbxx>
    $<$<IN_LIST:${TARGET_MCU_FAMILY},stm32f0xx;stm32f2xx;stm32f3xx;stm32f4xx;stm32f7xx;stm32wbxx>:$<TARGET_OBJECTS:hal_st.startup>>
    $<$<BOOL:${EMIL_HOST_BUILD}>:hal_st.cmsis_windows_stub>
)

add_library(hal_st.default_linker_scripts INTERFACE)

target_link_libraries(hal_st.default_linker_scripts INTERFACE
    $<$<STREQUAL:${TARGET_MCU_FAMILY},stm32f0xx>:hal_st.stm32f0xx_default_linker_scripts>
    $<$<STREQUAL:${TARGET_MCU_FAMILY},stm32f2xx>:hal_st.stm32f2xx_default_linker_scripts>
    $<$<STREQUAL:${TARGET_MCU_FAMILY},stm32f3xx>:hal_st.stm32f3xx_default_linker_scripts>
    $<$<STREQUAL:${TARGET_MCU_FAMILY},stm32f4xx>:hal_st.stm32f4xx_default_linker_scripts>
    $<$<STREQUAL:${TARGET_MCU_FAMILY},stm32f7xx>:hal_st.stm32f7xx_default_linker_scripts>
    $<$<STREQUAL:${TARGET_MCU_FAMILY},stm32wbxx>:hal_st.stm32wbxx_default_linker_scripts>
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
