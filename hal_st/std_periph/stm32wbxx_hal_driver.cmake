add_library(hal_st.std_periph_stm32wbxx EXCLUDE_FROM_ALL STATIC)
install(TARGETS hal_st.std_periph_stm32wbxx EXPORT halStTargets)

target_include_directories(hal_st.std_periph_stm32wbxx PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/STM32WBxx_HAL_Driver/Inc>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/CMSIS_STM32WBxx/Core/Include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/CMSIS_STM32WBxx/Device/ST/STM32WBxx/Include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/hal_conf>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/STM32WBxx_HAL_Driver/Inc>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/CMSIS_STM32WBxx/Core/Include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/CMSIS_STM32WBxx/Device/ST/STM32WBxx/Include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/hal_conf>"
)

target_link_libraries(hal_st.std_periph_stm32wbxx PUBLIC
    hal_st.cmsis_cortex
)

target_compile_definitions(hal_st.std_periph_stm32wbxx PUBLIC
    USE_HAL_DRIVER=1
    STM32WB=1
    STM32WB55xx=1
    DEVICE_HEADER="stm32wbxx.h"
    $<$<STREQUAL:${TARGET_MCU},stm32f407>:STM32F407xx=1>
    $<$<STREQUAL:${TARGET_MCU},stm32f429>:STM32F429xx=1>
    $<$<NOT:$<CONFIG:MinSizeRel>>:USE_FULL_ASSERT=1>
)

if (HALST_INCLUDE_DEFAULT_LINKER_SCRIPTS)
    target_link_options(hal_st.std_periph_stm32wbxx PUBLIC
        LINKER:-T${CMAKE_CURRENT_LIST_DIR}/ldscripts/mem_${TARGET_MCU}.ld,
               -T${CMAKE_CURRENT_LIST_DIR}/ldscripts/sections.ld
    )

    set_property(TARGET hal_st.std_periph_stm32wbxx PROPERTY INTERFACE_LINK_DEPENDS
        ${CMAKE_CURRENT_LIST_DIR}/ldscripts/mem_${TARGET_MCU}.ld
        ${CMAKE_CURRENT_LIST_DIR}/ldscripts/sections.ld
    )
endif()

# Assembler does not understand -Werror
set_target_properties(hal_st.std_periph_stm32wbxx PROPERTIES COMPILE_WARNING_AS_ERROR Off)

file(GLOB sources
    STM32WBxx_HAL_DRIVER/Inc/*.h
    STM32WBxx_HAL_DRIVER/Src/*.c
    CMSIS_STM32WBxx/Core/Include/*.h
    CMSIS_STM32WBxx/Device/ST/STM32WBxx/Include/*.h
    CMSIS_STM32WBxx/Device/ST/STM32WBxx/Source/Templates/system_stm32wbxx.c
    CMSIS_STM32WBxx/Device/ST/STM32WBxx/Source/Templates/gcc/startup_${TARGET_MCU}xx.s
    hal_conf/stm32wbxx_hal_conf.h
)

list(REMOVE_ITEM sources
    ${CMAKE_CURRENT_LIST_DIR}/STM32WBxx_HAL_DRIVER/Src/stm32wbxx_hal_timebase_rtc_wakeup_template.c
    ${CMAKE_CURRENT_LIST_DIR}/STM32WBxx_HAL_DRIVER/Src/stm32wbxx_hal_timebase_rtc_alarm_template.c
)

target_sources(hal_st.std_periph_stm32wbxx PRIVATE
    ${sources}
)
