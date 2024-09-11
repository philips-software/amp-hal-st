include(hal_driver.cmake)

add_hal_driver(st.hal_driver_stm32f4xx STM32F4xx_HAL_Driver CMSIS_STM32F4xx)
emil_build_for(st.hal_driver_stm32f4xx TARGET_MCU_FAMILY stm32f4xx PREREQUISITE_BOOL HALST_STANDALONE)

target_compile_definitions(st.hal_driver_stm32f4xx PUBLIC
    STM32F4=1
    DEVICE_HEADER="stm32f4xx.h"
    $<$<STREQUAL:${TARGET_MCU},stm32f407>:STM32F407xx=1>
    $<$<STREQUAL:${TARGET_MCU},stm32f429>:STM32F429xx=1>
)

add_hal_driver(st.hal_driver_stm32f7xx STM32F7xx_HAL_Driver CMSIS_STM32F7xx)
emil_build_for(st.hal_driver_stm32f7xx TARGET_MCU_FAMILY stm32f7xx PREREQUISITE_BOOL HALST_STANDALONE)

target_compile_definitions(st.hal_driver_stm32f7xx PUBLIC
    STM32F7=1
    DEVICE_HEADER="stm32f7xx.h"
    $<$<STREQUAL:${TARGET_MCU},stm32f746>:STM32F746xx=1>
    $<$<STREQUAL:${TARGET_MCU},stm32f767>:STM32F767xx=1>
)

add_hal_driver(st.hal_driver_stm32g0xx STM32G0xx_HAL_Driver CMSIS_STM32G0xx)
emil_build_for(st.hal_driver_stm32g0xx TARGET_MCU_FAMILY stm32g0xx PREREQUISITE_BOOL HALST_STANDALONE)

target_compile_definitions(st.hal_driver_stm32g0xx PUBLIC
    USE_HAL_DRIVER=1
    STM32G0=1
    DEVICE_HEADER="stm32g0xx.h"
    $<$<STREQUAL:${TARGET_MCU},stm32g070>:STM32G070xx=1>
)

add_hal_driver(st.hal_driver_stm32g4xx STM32G4xx_HAL_Driver CMSIS_STM32G4xx)
emil_build_for(st.hal_driver_stm32g4xx TARGET_MCU_FAMILY stm32g4xx PREREQUISITE_BOOL HALST_STANDALONE)

target_compile_definitions(st.hal_driver_stm32g4xx PUBLIC
    USE_HAL_DRIVER=1
    STM32G4=1
    DEVICE_HEADER="stm32g4xx.h"
    $<$<STREQUAL:${TARGET_MCU},stm32g431>:STM32G431xx=1>
    $<$<STREQUAL:${TARGET_MCU},stm32g474>:STM32G474xx=1>
)

add_hal_driver(st.hal_driver_stm32wbxx STM32WBxx_HAL_Driver CMSIS_STM32WBxx)
emil_build_for(st.hal_driver_stm32wbxx TARGET_MCU_FAMILY stm32wbxx PREREQUISITE_BOOL HALST_STANDALONE)

target_compile_definitions(st.hal_driver_stm32wbxx PUBLIC
    STM32WB=1
    STM32WB55xx=1
    DEVICE_HEADER="stm32wbxx.h"
)

add_hal_driver(st.hal_driver_stm32wbaxx STM32WBAxx_HAL_Driver CMSIS_STM32WBAxx)
emil_build_for(st.hal_driver_stm32wbaxx TARGET_MCU_FAMILY stm32wbaxx PREREQUISITE_BOOL HALST_STANDALONE)

target_compile_definitions(st.hal_driver_stm32wbaxx PUBLIC
    STM32WBA=1
    $<$<STREQUAL:${TARGET_MCU},stm32wba52>:STM32WBA52xx=1>
    DEVICE_HEADER="stm32wbaxx.h"
)

# Fix optimization for long file which is breaking the compiler
set_source_files_properties(STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_dma_ex.c PROPERTIES COMPILE_FLAGS -Os)

add_hal_driver(st.hal_driver_stm32h5xx STM32H5xx_HAL_Driver CMSIS_STM32H5xx)
emil_build_for(st.hal_driver_stm32h5xx TARGET_MCU_FAMILY stm32h5xx PREREQUISITE_BOOL HALST_STANDALONE)

target_compile_definitions(st.hal_driver_stm32h5xx PUBLIC
    USE_HAL_DRIVER=1
    STM32H5=1
    DEVICE_HEADER="stm32h5xx.h"
    $<$<STREQUAL:${TARGET_MCU},stm32h563>:STM32H563xx=1>
    $<$<STREQUAL:${TARGET_MCU},stm32h573>:STM32H573xx=1>
)

# Fix optimization for long file which is breaking the compiler
set_source_files_properties(STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma_ex.c PROPERTIES COMPILE_FLAGS -Os)

add_library(st.hal_driver INTERFACE)
emil_build_for(st.hal_driver TARGET_MCU_VENDOR st PREREQUISITE_BOOL HALST_STANDALONE)

if (TARGET_MCU_VENDOR STREQUAL st)
    target_link_libraries(st.hal_driver INTERFACE
        st.hal_driver_${TARGET_MCU_FAMILY}
    )
endif()

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
