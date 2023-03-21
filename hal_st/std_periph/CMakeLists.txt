add_subdirectory(std_periph_stm32f0xx)
add_subdirectory(std_periph_stm32f2xx)
add_subdirectory(std_periph_stm32f3xx)
add_subdirectory(std_periph_stm32f4xx)
add_subdirectory(std_periph_stm32f7xx)
add_subdirectory(std_periph_stm32wbxx)
add_subdirectory(windows_cmsis_stub)
add_subdirectory(atomic)

add_library(hal_st.std_periph ${HALST_EXCLUDE_FROM_ALL} INTERFACE)
install(TARGETS hal_st.std_periph EXPORT halStTargets)

if("${TARGET_MCU_FAMILY}" STREQUAL stm32f0xx)
    target_link_libraries(hal_st.std_periph INTERFACE hal_st.atomic)
    target_link_libraries(hal_st.std_periph INTERFACE hal_st.std_periph_stm32wbxx)
elseif("${TARGET_MCU_FAMILY}" STREQUAL stm32f2xx)
    target_link_libraries(hal_st.std_periph INTERFACE hal_st.std_periph_stm32f2xx)
elseif("${TARGET_MCU_FAMILY}" STREQUAL stm32f3xx)
    target_link_libraries(hal_st.std_periph INTERFACE hal_st.std_periph_stm32f3xx)
elseif("${TARGET_MCU_FAMILY}" STREQUAL stm32f4xx)
    target_link_libraries(hal_st.std_periph INTERFACE hal_st.std_periph_stm32f4xx)
elseif("${TARGET_MCU_FAMILY}" STREQUAL stm32f7xx)
    target_link_libraries(hal_st.std_periph INTERFACE hal_st.std_periph_stm32f7xx)
elseif("${TARGET_MCU_FAMILY}" STREQUAL stm32wbxx)
    target_link_libraries(hal_st.std_periph INTERFACE hal_st.std_periph_stm32wbxx)
elseif(EMIL_HOST_BUILD)
    target_link_libraries(hal_st.std_periph INTERFACE hal_st.cmsis_windows_stub)
endif()

target_link_libraries(hal_st.std_periph INTERFACE
    $<TARGET_OBJECTS:hal_st.startup>
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
