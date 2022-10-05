add_library(hal_st.std_periph_stm32f3xx EXCLUDE_FROM_ALL STATIC)
install(TARGETS hal_st.std_periph_stm32f3xx EXPORT halStTargets)

target_include_directories(hal_st.std_periph_stm32f3xx SYSTEM PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/inc>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/inc/cmsis>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/inc/stm32f3-hal>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/inc>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/inc/cmsis>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/inc/stm32f3-hal>"
)

target_link_libraries(hal_st.std_periph_stm32f3xx PUBLIC
    hal_st.cmsis_arm
    hal_st.cmsis_cortex
)

target_compile_definitions(hal_st.std_periph_stm32f3xx PUBLIC
    USE_HAL_DRIVER
    HSE_VALUE=16000000
    $<$<BOOL:${CMAKE_HOST_BUILD}>:STM32F732xx> # For syntax recognition
    $<$<NOT:$<CONFIG:MinSizeRel>>:USE_FULL_ASSERT>
    $<$<NOT:$<CONFIG:MinSizeRel>>:TRACE>
    $<$<NOT:$<CONFIG:MinSizeRel>>:OS_USE_TRACE_ITM>
)

if (HALST_INCLUDE_DEFAULT_LINKER_SCRIPTS)
    target_link_options(hal_st.std_periph_stm32f3xx PUBLIC
        LINKER:-T${CMAKE_CURRENT_LIST_DIR}/ldscripts/mem.ld,
               -T${CMAKE_CURRENT_LIST_DIR}/../ldscripts/sections.ld
    )

    set_property(TARGET hal_st.std_periph_stm32f3xx PROPERTY INTERFACE_LINK_DEPENDS
        ${CMAKE_CURRENT_LIST_DIR}/ldscripts/mem.ld
        ${CMAKE_CURRENT_LIST_DIR}/../ldscripts/sections.ld
    )
endif()

target_sources(hal_st.std_periph_stm32f3xx PRIVATE
    inc/stm32f3xx_hal_conf.h
    inc/cmsis/cmsis_device.h
    inc/cmsis/cmsis_device_hal.h
    inc/cmsis/stm32f3xx.h
    inc/cmsis/stm32f301x8.h
    inc/cmsis/stm32f302x8.h
    inc/cmsis/stm32f302xc.h
    inc/cmsis/stm32f302xe.h
    inc/cmsis/stm32f303x8.h
    inc/cmsis/stm32f303xc.h
    inc/cmsis/stm32f303xe.h
    inc/cmsis/stm32f318xx.h
    inc/cmsis/stm32f328xx.h
    inc/cmsis/stm32f334x8.h
    inc/cmsis/stm32f358xx.h
    inc/cmsis/stm32f373xc.h
    inc/cmsis/stm32f358xx.h
    inc/cmsis/stm32f398xx.h
    inc/cmsis/system_stm32f3xx.h
    inc/stm32f3-hal/stm32f3xx_hal.h
    inc/stm32f3-hal/stm32f3xx_hal_adc.h
    inc/stm32f3-hal/stm32f3xx_hal_adc_ex.h
    inc/stm32f3-hal/stm32f3xx_hal_can.h
    inc/stm32f3-hal/stm32f3xx_hal_cec.h
    inc/stm32f3-hal/stm32f3xx_hal_comp.h
    inc/stm32f3-hal/stm32f3xx_hal_comp_ex.h
    inc/stm32f3-hal/stm32f3xx_hal_conf_template.h
    inc/stm32f3-hal/stm32f3xx_hal_cortex.h
    inc/stm32f3-hal/stm32f3xx_hal_crc.h
    inc/stm32f3-hal/stm32f3xx_hal_crc_ex.h
    inc/stm32f3-hal/stm32f3xx_hal_dac.h
    inc/stm32f3-hal/stm32f3xx_hal_dac_ex.h
    inc/stm32f3-hal/stm32f3xx_hal_def.h
    inc/stm32f3-hal/stm32f3xx_hal_dma.h
    inc/stm32f3-hal/stm32f3xx_hal_dma_ex.h
    inc/stm32f3-hal/stm32f3xx_hal_flash.h
    inc/stm32f3-hal/stm32f3xx_hal_flash_ex.h
    inc/stm32f3-hal/stm32f3xx_hal_gpio.h
    inc/stm32f3-hal/stm32f3xx_hal_gpio_ex.h
    inc/stm32f3-hal/stm32f3xx_hal_hrtim.h
    inc/stm32f3-hal/stm32f3xx_hal_i2c.h
    inc/stm32f3-hal/stm32f3xx_hal_i2c_ex.h
    inc/stm32f3-hal/stm32f3xx_hal_i2s.h
    inc/stm32f3-hal/stm32f3xx_hal_i2s_ex.h
    inc/stm32f3-hal/stm32f3xx_hal_irda.h
    inc/stm32f3-hal/stm32f3xx_hal_irda_ex.h
    inc/stm32f3-hal/stm32f3xx_hal_iwdg.h
    inc/stm32f3-hal/stm32f3xx_hal_nand.h
    inc/stm32f3-hal/stm32f3xx_hal_nor.h
    inc/stm32f3-hal/stm32f3xx_hal_opamp.h
    inc/stm32f3-hal/stm32f3xx_hal_opamp_ex.h
    inc/stm32f3-hal/stm32f3xx_hal_pccard.h
    inc/stm32f3-hal/stm32f3xx_hal_pcd.h
    inc/stm32f3-hal/stm32f3xx_hal_pcd_ex.h
    inc/stm32f3-hal/stm32f3xx_hal_pwr.h
    inc/stm32f3-hal/stm32f3xx_hal_pwr_ex.h
    inc/stm32f3-hal/stm32f3xx_hal_rcc.h
    inc/stm32f3-hal/stm32f3xx_hal_rcc_ex.h
    inc/stm32f3-hal/stm32f3xx_hal_rtc.h
    inc/stm32f3-hal/stm32f3xx_hal_rtc_ex.h
    inc/stm32f3-hal/stm32f3xx_hal_sdadc.h
    inc/stm32f3-hal/stm32f3xx_hal_smartcard.h
    inc/stm32f3-hal/stm32f3xx_hal_smartcard_ex.h
    inc/stm32f3-hal/stm32f3xx_hal_smbus.h
    inc/stm32f3-hal/stm32f3xx_hal_spi.h
    inc/stm32f3-hal/stm32f3xx_hal_spi_ex.h
    inc/stm32f3-hal/stm32f3xx_hal_sram.h
    inc/stm32f3-hal/stm32f3xx_hal_tim.h
    inc/stm32f3-hal/stm32f3xx_hal_tim_ex.h
    inc/stm32f3-hal/stm32f3xx_hal_tsc.h
    inc/stm32f3-hal/stm32f3xx_hal_usart.h
    inc/stm32f3-hal/stm32f3xx_hal_usart_ex.h
    inc/stm32f3-hal/stm32f3xx_hal_wwdg.h
    inc/stm32f3-hal/stm32f3xx_ll_fmc.h
    inc/stm32f3-hal/Legacy/stm32_hal_legacy.h
    src/cmsis/system_stm32f3xx.c
    src/cmsis/vectors_stm32f3xx.c
    src/stm32f3-hal/stm32f3xx_hal.c
    src/stm32f3-hal/stm32f3xx_hal_adc.c
    src/stm32f3-hal/stm32f3xx_hal_adc_ex.c
    src/stm32f3-hal/stm32f3xx_hal_can.c
    src/stm32f3-hal/stm32f3xx_hal_cec.c
    src/stm32f3-hal/stm32f3xx_hal_comp.c
    src/stm32f3-hal/stm32f3xx_hal_cortex.c
    src/stm32f3-hal/stm32f3xx_hal_crc.c
    src/stm32f3-hal/stm32f3xx_hal_crc_ex.c
    src/stm32f3-hal/stm32f3xx_hal_dac.c
    src/stm32f3-hal/stm32f3xx_hal_dac_ex.c
    src/stm32f3-hal/stm32f3xx_hal_dma.c
    src/stm32f3-hal/stm32f3xx_hal_flash.c
    src/stm32f3-hal/stm32f3xx_hal_flash_ex.c
    src/stm32f3-hal/stm32f3xx_hal_gpio.c
    src/stm32f3-hal/stm32f3xx_hal_hrtim.c
    src/stm32f3-hal/stm32f3xx_hal_i2c.c
    src/stm32f3-hal/stm32f3xx_hal_i2c_ex.c
    src/stm32f3-hal/stm32f3xx_hal_i2s.c
    src/stm32f3-hal/stm32f3xx_hal_i2s_ex.c
    src/stm32f3-hal/stm32f3xx_hal_irda.c
    src/stm32f3-hal/stm32f3xx_hal_iwdg.c
    src/stm32f3-hal/stm32f3xx_hal_msp_template.c
    src/stm32f3-hal/stm32f3xx_hal_nand.c
    src/stm32f3-hal/stm32f3xx_hal_nor.c
    src/stm32f3-hal/stm32f3xx_hal_opamp.c
    src/stm32f3-hal/stm32f3xx_hal_opamp_ex.c
    src/stm32f3-hal/stm32f3xx_hal_pccard.c
    src/stm32f3-hal/stm32f3xx_hal_pcd.c
    src/stm32f3-hal/stm32f3xx_hal_pcd_ex.c
    src/stm32f3-hal/stm32f3xx_hal_pwr.c
    src/stm32f3-hal/stm32f3xx_hal_pwr_ex.c
    src/stm32f3-hal/stm32f3xx_hal_rcc.c
    src/stm32f3-hal/stm32f3xx_hal_rcc_ex.c
    src/stm32f3-hal/stm32f3xx_hal_rtc.c
    src/stm32f3-hal/stm32f3xx_hal_rtc_ex.c
    src/stm32f3-hal/stm32f3xx_hal_sdadc.c
    src/stm32f3-hal/stm32f3xx_hal_smartcard.c
    src/stm32f3-hal/stm32f3xx_hal_smartcard_ex.c
    src/stm32f3-hal/stm32f3xx_hal_smbus.c
    src/stm32f3-hal/stm32f3xx_hal_spi.c
    src/stm32f3-hal/stm32f3xx_hal_spi_ex.c
    src/stm32f3-hal/stm32f3xx_hal_sram.c
    src/stm32f3-hal/stm32f3xx_hal_tim.c
    src/stm32f3-hal/stm32f3xx_hal_tim_ex.c
    src/stm32f3-hal/stm32f3xx_hal_tsc.c
    src/stm32f3-hal/stm32f3xx_hal_uart.c
    src/stm32f3-hal/stm32f3xx_hal_uart_ex.c
    src/stm32f3-hal/stm32f3xx_hal_uart_ex.c
    src/stm32f3-hal/stm32f3xx_hal_wwdg.c
    src/stm32f3-hal/stm32f3xx_ll_fmc.c
)
