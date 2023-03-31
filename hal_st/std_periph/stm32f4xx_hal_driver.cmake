add_library(hal_st.std_periph_stm32f4xx EXCLUDE_FROM_ALL STATIC)
install(TARGETS hal_st.std_periph_stm32f4xx EXPORT halStTargets)

target_include_directories(hal_st.std_periph_stm32f4xx PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/STM32F4xx_HAL_Driver/Inc>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/CMSIS_STM32F4xx/Core/Include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/CMSIS_STM32F4xx/Device/ST/STM32F4xx/Include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/hal_conf>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/STM32F4xx_HAL_Driver/Inc>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/CMSIS_STM32F4xx/Core/Include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/CMSIS_STM32F4xx/Device/ST/STM32F4xx/Include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/hal_conf>"
)

target_link_libraries(hal_st.std_periph_stm32f4xx PUBLIC
    hal_st.cmsis_cortex
)

target_compile_definitions(hal_st.std_periph_stm32f4xx PUBLIC
    USE_HAL_DRIVER=1
    STM32F4=1
    DEVICE_HEADER="stm32f4xx.h"
    $<$<STREQUAL:${TARGET_MCU},stm32f407>:STM32F407xx=1>
    $<$<STREQUAL:${TARGET_MCU},stm32f429>:STM32F429xx=1>
    $<$<NOT:$<CONFIG:MinSizeRel>>:USE_FULL_ASSERT=1>
    $<$<NOT:$<CONFIG:MinSizeRel>>:TRACE=1>
    $<$<NOT:$<CONFIG:MinSizeRel>>:OS_USE_TRACE_ITM=1>
)

if (HALST_INCLUDE_DEFAULT_LINKER_SCRIPTS)
    target_link_options(hal_st.std_periph_stm32f4xx PUBLIC
        LINKER:-T${CMAKE_CURRENT_LIST_DIR}/ldscripts/mem_${TARGET_MCU}.ld,
               -T${CMAKE_CURRENT_LIST_DIR}/ldscripts/sections.ld
    )

    set_property(TARGET hal_st.std_periph_stm32f4xx PROPERTY INTERFACE_LINK_DEPENDS
        ${CMAKE_CURRENT_LIST_DIR}/ldscripts/mem_${TARGET_MCU}.ld
        ${CMAKE_CURRENT_LIST_DIR}/ldscripts/sections.ld
    )
endif()

# Assembler does not understand -Werror
set_target_properties(hal_st.std_periph_stm32f4xx PROPERTIES COMPILE_WARNING_AS_ERROR Off)

file(GLOB sources
    STM32F4xx_HAL_DRIVER/Inc/*.h
    STM32F4xx_HAL_DRIVER/Src/*.c
    CMSIS_STM32F4xx/Core/Include/*.h
    CMSIS_STM32F4xx/Device/ST/STM32F4xx/Include/*.h
    CMSIS_STM32F4xx/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c
    CMSIS_STM32F4xx/Device/ST/STM32F4xx/Source/Templates/gcc/startup_${TARGET_MCU}xx.s
    hal_conf/stm32f4xx_hal_conf.h
)

target_sources(hal_st.std_periph_stm32f4xx PRIVATE
    ${sources}
)
