add_library(hal_st.std_periph_stm32f2xx EXCLUDE_FROM_ALL STATIC)
install(TARGETS hal_st.std_periph_stm32f2xx EXPORT halStTargets)

target_include_directories(hal_st.std_periph_stm32f2xx PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/inc>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/inc/cmsis>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/inc/stm32f2-hal>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/inc>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/inc/cmsis>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/inc/stm32f2-hal>"
)

target_link_libraries(hal_st.std_periph_stm32f2xx PUBLIC
    hal_st.cmsis_cortex
)

target_compile_definitions(hal_st.std_periph_stm32f2xx PUBLIC
    USE_HAL_DRIVER
    $<$<BOOL:${EMIL_HOST_BUILD}>:STM32F205xx> # For syntax recognition
    $<$<NOT:$<CONFIG:MinSizeRel>>:USE_FULL_ASSERT>
)

if (HALST_INCLUDE_DEFAULT_LINKER_SCRIPTS)
    target_link_options(hal_st.std_periph_stm32f2xx PUBLIC
        LINKER:-T${CMAKE_CURRENT_LIST_DIR}/ldscripts/mem.ld,
               -T${CMAKE_CURRENT_LIST_DIR}/../ldscripts/sections.ld
    )

    set_property(TARGET hal_st.std_periph_stm32f2xx PROPERTY INTERFACE_LINK_DEPENDS
        ${CMAKE_CURRENT_LIST_DIR}/ldscripts/mem.ld
        ${CMAKE_CURRENT_LIST_DIR}/../ldscripts/sections.ld
    )
endif()

target_sources(hal_st.std_periph_stm32f2xx PRIVATE
    inc/stm32f2xx_hal_conf.h
    inc/cmsis/cmsis_device.h
    inc/cmsis/cmsis_device_hal.h
    inc/cmsis/stm32f205xx.h
    inc/cmsis/stm32f207xx.h
    inc/cmsis/stm32f215xx.h
    inc/cmsis/stm32f217xx.h
    inc/cmsis/system_stm32f2xx.h
    inc/stm32f2-hal/stm32f2xx_hal.h
    inc/stm32f2-hal/stm32f2xx_hal_adc.h
    inc/stm32f2-hal/stm32f2xx_hal_adc_ex.h
    inc/stm32f2-hal/stm32f2xx_hal_can.h
    inc/stm32f2-hal/stm32f2xx_hal_conf_template.h
    inc/stm32f2-hal/stm32f2xx_hal_cortex.h
    inc/stm32f2-hal/stm32f2xx_hal_crc.h
    inc/stm32f2-hal/stm32f2xx_hal_cryp.h
    inc/stm32f2-hal/stm32f2xx_hal_dac.h
    inc/stm32f2-hal/stm32f2xx_hal_dac_ex.h
    inc/stm32f2-hal/stm32f2xx_hal_dcmi.h
    inc/stm32f2-hal/stm32f2xx_hal_def.h
    inc/stm32f2-hal/stm32f2xx_hal_dma.h
    inc/stm32f2-hal/stm32f2xx_hal_dma_ex.h
    inc/stm32f2-hal/stm32f2xx_hal_eth.h
    inc/stm32f2-hal/stm32f2xx_hal_flash.h
    inc/stm32f2-hal/stm32f2xx_hal_flash_ex.h
    inc/stm32f2-hal/stm32f2xx_hal_gpio.h
    inc/stm32f2-hal/stm32f2xx_hal_gpio_ex.h
    inc/stm32f2-hal/stm32f2xx_hal_hash.h
    inc/stm32f2-hal/stm32f2xx_hal_hcd.h
    inc/stm32f2-hal/stm32f2xx_hal_i2c.h
    inc/stm32f2-hal/stm32f2xx_hal_i2s.h
    inc/stm32f2-hal/stm32f2xx_hal_irda.h
    inc/stm32f2-hal/stm32f2xx_hal_iwdg.h
    inc/stm32f2-hal/stm32f2xx_hal_nand.h
    inc/stm32f2-hal/stm32f2xx_hal_nor.h
    inc/stm32f2-hal/stm32f2xx_hal_pccard.h
    inc/stm32f2-hal/stm32f2xx_hal_pcd.h
    inc/stm32f2-hal/stm32f2xx_hal_pcd_ex.h
    inc/stm32f2-hal/stm32f2xx_hal_pwr.h
    inc/stm32f2-hal/stm32f2xx_hal_pwr_ex.h
    inc/stm32f2-hal/stm32f2xx_hal_rcc.h
    inc/stm32f2-hal/stm32f2xx_hal_rcc_ex.h
    inc/stm32f2-hal/stm32f2xx_hal_rng.h
    inc/stm32f2-hal/stm32f2xx_hal_rtc.h
    inc/stm32f2-hal/stm32f2xx_hal_rtc_ex.h
    inc/stm32f2-hal/stm32f2xx_hal_sd.h
    inc/stm32f2-hal/stm32f2xx_hal_smartcard.h
    inc/stm32f2-hal/stm32f2xx_hal_spi.h
    inc/stm32f2-hal/stm32f2xx_hal_sram.h
    inc/stm32f2-hal/stm32f2xx_hal_tim.h
    inc/stm32f2-hal/stm32f2xx_hal_tim_ex.h
    inc/stm32f2-hal/stm32f2xx_hal_uart.h
    inc/stm32f2-hal/stm32f2xx_hal_usart.h
    inc/stm32f2-hal/stm32f2xx_hal_wwdg.h
    inc/stm32f2-hal/stm32f2xx_ll_fsmc.h
    inc/stm32f2-hal/stm32f2xx_ll_sdmmc.h
    inc/stm32f2-hal/stm32f2xx_ll_usb.h
    inc/stm32f2-hal/Legacy/stm32_hal_legacy.h
    src/cmsis/system_stm32f2xx.c
    src/cmsis/vectors_stm32f2xx.c
    src/stm32f2-hal/stm32f2xx_hal.c
    src/stm32f2-hal/stm32f2xx_hal_adc.c
    src/stm32f2-hal/stm32f2xx_hal_adc_ex.c
    src/stm32f2-hal/stm32f2xx_hal_can.c
    src/stm32f2-hal/stm32f2xx_hal_cortex.c
    src/stm32f2-hal/stm32f2xx_hal_crc.c
    src/stm32f2-hal/stm32f2xx_hal_cryp.c
    src/stm32f2-hal/stm32f2xx_hal_dac_ex.c
    src/stm32f2-hal/stm32f2xx_hal_dac.c
    src/stm32f2-hal/stm32f2xx_hal_dcmi.c
    src/stm32f2-hal/stm32f2xx_hal_dma_ex.c
    src/stm32f2-hal/stm32f2xx_hal_dma.c
    src/stm32f2-hal/stm32f2xx_hal_eth.c
    src/stm32f2-hal/stm32f2xx_hal_flash_ex.c
    src/stm32f2-hal/stm32f2xx_hal_flash.c
    src/stm32f2-hal/stm32f2xx_hal_gpio.c
    src/stm32f2-hal/stm32f2xx_hal_hash.c
    src/stm32f2-hal/stm32f2xx_hal_hcd.c
    src/stm32f2-hal/stm32f2xx_hal_i2c.c
    src/stm32f2-hal/stm32f2xx_hal_i2s.c
    src/stm32f2-hal/stm32f2xx_hal_irda.c
    src/stm32f2-hal/stm32f2xx_hal_iwdg.c
    src/stm32f2-hal/stm32f2xx_hal_msp_template.c
    src/stm32f2-hal/stm32f2xx_hal_nand.c
    src/stm32f2-hal/stm32f2xx_hal_nor.c
    src/stm32f2-hal/stm32f2xx_hal_pccard.c
    src/stm32f2-hal/stm32f2xx_hal_pcd_ex.c
    src/stm32f2-hal/stm32f2xx_hal_pcd.c
    src/stm32f2-hal/stm32f2xx_hal_pwr_ex.c
    src/stm32f2-hal/stm32f2xx_hal_pwr.c
    src/stm32f2-hal/stm32f2xx_hal_rcc_ex.c
    src/stm32f2-hal/stm32f2xx_hal_rcc.c
    src/stm32f2-hal/stm32f2xx_hal_rng.c
    src/stm32f2-hal/stm32f2xx_hal_rtc_ex.c
    src/stm32f2-hal/stm32f2xx_hal_rtc.c
    src/stm32f2-hal/stm32f2xx_hal_sd.c
    src/stm32f2-hal/stm32f2xx_hal_smartcard.c
    src/stm32f2-hal/stm32f2xx_hal_spi.c
    src/stm32f2-hal/stm32f2xx_hal_sram.c
    src/stm32f2-hal/stm32f2xx_hal_tim_ex.c
    src/stm32f2-hal/stm32f2xx_hal_tim.c
    src/stm32f2-hal/stm32f2xx_hal_uart.c
    src/stm32f2-hal/stm32f2xx_hal_usart.c
    src/stm32f2-hal/stm32f2xx_hal_wwdg.c
    src/stm32f2-hal/stm32f2xx_ll_fsmc.c
    src/stm32f2-hal/stm32f2xx_ll_sdmmc.c
    src/stm32f2-hal/stm32f2xx_ll_usb.c
)
