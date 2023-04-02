include(hal_driver.cmake)
add_hal_driver(hal_st.std_periph_stm32f4xx STM32F4xx_HAL_Driver CMSIS_STM32F4xx)
add_hal_driver(hal_st.std_periph_stm32f7xx STM32F7xx_HAL_Driver CMSIS_STM32F7xx)
add_hal_driver(hal_st.std_periph_stm32g4xx STM32G4xx_HAL_Driver CMSIS_STM32G4xx)
add_hal_driver(hal_st.std_periph_stm32wbxx STM32WBxx_HAL_Driver CMSIS_STM32WBxx)

target_compile_definitions(hal_st.std_periph_stm32f4xx PUBLIC
    STM32F4=1
    DEVICE_HEADER="stm32f4xx.h"
    $<$<STREQUAL:${TARGET_MCU},stm32f407>:STM32F407xx=1>
    $<$<STREQUAL:${TARGET_MCU},stm32f429>:STM32F429xx=1>
)

target_compile_definitions(hal_st.std_periph_stm32f7xx PUBLIC
    STM32F7=1
    DEVICE_HEADER="stm32f7xx.h"
    $<$<STREQUAL:${TARGET_MCU},stm32f746>:STM32F746xx=1>
    $<$<STREQUAL:${TARGET_MCU},stm32f767>:STM32F767xx=1>
)

target_compile_definitions(hal_st.std_periph_stm32g4xx PUBLIC
    USE_HAL_DRIVER=1
    STM32G4=1
    DEVICE_HEADER="stm32g4xx.h"
    $<$<STREQUAL:${TARGET_MCU},stm32g431>:STM32G431xx=1>
)

target_compile_definitions(hal_st.std_periph_stm32wbxx PUBLIC
    STM32WB=1
    STM32WB55xx=1
    DEVICE_HEADER="stm32wbxx.h"
)

add_library(hal_st.std_periph ${HALST_EXCLUDE_FROM_ALL} INTERFACE)
install(TARGETS hal_st.std_periph EXPORT halStTargets)

if (TARGET_MCU_VENDOR STREQUAL st)
    target_link_libraries(hal_st.std_periph INTERFACE
        hal_st.std_periph_${TARGET_MCU_FAMILY}
    )
endif()

target_link_libraries(hal_st.std_periph INTERFACE
    $<TARGET_OBJECTS:hal_st.startup>
)

add_subdirectory(default_init)

function(halst_target_linker_scripts)
    set(singleArgs TARGET)
    set(multiArgs LINKER_SCRIPTS)
    cmake_parse_arguments(PARSE_ARGV 0 HALST "" "${singleArgs}" "${multiArgs}")

    foreach(script ${HALST_LINKER_SCRIPTS})
        cmake_path(IS_RELATIVE script is_relative)
        if (is_relative)
            target_link_options(${HALST_TARGET} PUBLIC LINKER:-T${CMAKE_CURRENT_LIST_DIR}/${script})
        else()
            target_link_options(${HALST_TARGET} PUBLIC LINKER:-T${script})
        endif()
    endforeach()

    set_property(TARGET ${HALST_TARGET} PROPERTY INTERFACE_LINK_DEPENDS ${HALST_LINKER_SCRIPTS})
endfunction()

function(halst_target_default_linker_scripts target)
    halst_target_linker_scripts(TARGET ${target} LINKER_SCRIPTS ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/ldscripts/mem_${TARGET_MCU}.ld ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/ldscripts/sections.ld)
endfunction()

emil_exclude_directory_from_clang_format(.)
