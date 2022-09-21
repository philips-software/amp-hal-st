add_library(hal_st.std_periph_stm32f7xx EXCLUDE_FROM_ALL STATIC)
install(TARGETS hal_st.std_periph_stm32f7xx EXPORT halStTargets)

target_include_directories(hal_st.std_periph_stm32f7xx PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/inc>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/inc/cmsis>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/inc/stm32f7-hal>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/inc>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/inc/cmsis>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/inc/stm32f7-hal>"
)

target_link_libraries(hal_st.std_periph_stm32f7xx PUBLIC
    hal_st.cmsis_arm
    hal_st.cmsis_cortex
)

target_compile_definitions(hal_st.std_periph_stm32f7xx PUBLIC
    USE_HAL_DRIVER
    STM32F7
    $<$<STREQUAL:${TARGET_MCU},stm32f746>:STM32F746xx>
    $<$<STREQUAL:${TARGET_MCU},stm32f767>:STM32F767xx>
    $<$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>:STM32F746xx> # For syntax recognition
    $<$<NOT:$<CONFIG:MinSizeRel>>:USE_FULL_ASSERT>
    $<$<NOT:$<CONFIG:MinSizeRel>>:TRACE>
    $<$<NOT:$<CONFIG:MinSizeRel>>:OS_USE_TRACE_ITM>
)

if (HALST_INCLUDE_DEFAULT_LINKER_SCRIPTS)
    if ("${TARGET_MCU}" STREQUAL stm32f767)
        set(LINKER_FILE mem_stm32f767.ld)
    elseif ("${TARGET_MCU}" STREQUAL stm32f746)
        set(LINKER_FILE mem_stm32f746.ld)
    endif()

    target_link_options(hal_st.std_periph_stm32f7xx PUBLIC
        LINKER:-T${CMAKE_CURRENT_LIST_DIR}/ldscripts/${LINKER_FILE},
               -T${CMAKE_CURRENT_LIST_DIR}/../ldscripts/sections.ld
    )

    set_property(TARGET hal_st.std_periph_stm32f7xx PROPERTY INTERFACE_LINK_DEPENDS
        ${CMAKE_CURRENT_LIST_DIR}/ldscripts/${LINKER_FILE}
        ${CMAKE_CURRENT_LIST_DIR}/../ldscripts/sections.ld
    )
endif()

target_sources(hal_st.std_periph_stm32f7xx PRIVATE
    inc/stm32f7xx_hal_conf.h
    inc/cmsis/cmsis_device.h
    inc/cmsis/cmsis_device_hal.h
    inc/cmsis/stm32f7xx.h
    inc/cmsis/stm32f745xx.h
    inc/cmsis/stm32f746xx.h
    inc/cmsis/stm32f756xx.h
    inc/cmsis/stm32f765xx.h
    inc/cmsis/stm32f767xx.h
    inc/cmsis/stm32f777xx.h
    inc/cmsis/stm32f779xx.h
    inc/cmsis/system_stm32f7xx.h
    inc/stm32f7-hal/stm32f7xx_hal.h
    inc/stm32f7-hal/stm32f7xx_hal_adc.h
    inc/stm32f7-hal/stm32f7xx_hal_adc_ex.h
    inc/stm32f7-hal/stm32f7xx_hal_can.h
    inc/stm32f7-hal/stm32f7xx_hal_cec.h
    inc/stm32f7-hal/stm32f7xx_hal_conf_template.h
    inc/stm32f7-hal/stm32f7xx_hal_cortex.h
    inc/stm32f7-hal/stm32f7xx_hal_crc.h
    inc/stm32f7-hal/stm32f7xx_hal_crc_ex.h
    inc/stm32f7-hal/stm32f7xx_hal_cryp.h
    inc/stm32f7-hal/stm32f7xx_hal_cryp_ex.h
    inc/stm32f7-hal/stm32f7xx_hal_dac.h
    inc/stm32f7-hal/stm32f7xx_hal_dac_ex.h
    inc/stm32f7-hal/stm32f7xx_hal_dcmi.h
    inc/stm32f7-hal/stm32f7xx_hal_dcmi_ex.h
    inc/stm32f7-hal/stm32f7xx_hal_def.h
    inc/stm32f7-hal/stm32f7xx_hal_dfsdm.h
    inc/stm32f7-hal/stm32f7xx_hal_dma.h
    inc/stm32f7-hal/stm32f7xx_hal_dma_ex.h
    inc/stm32f7-hal/stm32f7xx_hal_dma2d.h
    inc/stm32f7-hal/stm32f7xx_hal_dsi.h
    inc/stm32f7-hal/stm32f7xx_hal_eth.h
    inc/stm32f7-hal/stm32f7xx_hal_flash.h
    inc/stm32f7-hal/stm32f7xx_hal_flash_ex.h
    inc/stm32f7-hal/stm32f7xx_hal_gpio.h
    inc/stm32f7-hal/stm32f7xx_hal_gpio_ex.h
    inc/stm32f7-hal/stm32f7xx_hal_hash.h
    inc/stm32f7-hal/stm32f7xx_hal_hash_ex.h
    inc/stm32f7-hal/stm32f7xx_hal_hcd.h
    inc/stm32f7-hal/stm32f7xx_hal_i2c.h
    inc/stm32f7-hal/stm32f7xx_hal_i2c_ex.h
    inc/stm32f7-hal/stm32f7xx_hal_i2s.h
    inc/stm32f7-hal/stm32f7xx_hal_irda.h
    inc/stm32f7-hal/stm32f7xx_hal_irda_ex.h
    inc/stm32f7-hal/stm32f7xx_hal_iwdg.h
    inc/stm32f7-hal/stm32f7xx_hal_jpeg.h
    inc/stm32f7-hal/stm32f7xx_hal_lptim.h
    inc/stm32f7-hal/stm32f7xx_hal_ltdc.h
    inc/stm32f7-hal/stm32f7xx_hal_ltdc_ex.h
    inc/stm32f7-hal/stm32f7xx_hal_mdios.h
    inc/stm32f7-hal/stm32f7xx_hal_nand.h
    inc/stm32f7-hal/stm32f7xx_hal_nor.h
    inc/stm32f7-hal/stm32f7xx_hal_pcd.h
    inc/stm32f7-hal/stm32f7xx_hal_pcd_ex.h
    inc/stm32f7-hal/stm32f7xx_hal_pwr.h
    inc/stm32f7-hal/stm32f7xx_hal_pwr_ex.h
    inc/stm32f7-hal/stm32f7xx_hal_qspi.h
    inc/stm32f7-hal/stm32f7xx_hal_rcc.h
    inc/stm32f7-hal/stm32f7xx_hal_rcc_ex.h
    inc/stm32f7-hal/stm32f7xx_hal_rng.h
    inc/stm32f7-hal/stm32f7xx_hal_rtc.h
    inc/stm32f7-hal/stm32f7xx_hal_rtc_ex.h
    inc/stm32f7-hal/stm32f7xx_hal_sai.h
    inc/stm32f7-hal/stm32f7xx_hal_sd.h
    inc/stm32f7-hal/stm32f7xx_hal_sdram.h
    inc/stm32f7-hal/stm32f7xx_hal_smartcard.h
    inc/stm32f7-hal/stm32f7xx_hal_smartcard_ex.h
    inc/stm32f7-hal/stm32f7xx_hal_spdifrx.h
    inc/stm32f7-hal/stm32f7xx_hal_spi.h
    inc/stm32f7-hal/stm32f7xx_hal_sram.h
    inc/stm32f7-hal/stm32f7xx_hal_tim.h
    inc/stm32f7-hal/stm32f7xx_hal_tim_ex.h
    inc/stm32f7-hal/stm32f7xx_hal_uart.h
    inc/stm32f7-hal/stm32f7xx_hal_uart_ex.h
    inc/stm32f7-hal/stm32f7xx_hal_usart.h
    inc/stm32f7-hal/stm32f7xx_hal_usart_ex.h
    inc/stm32f7-hal/stm32f7xx_hal_wwdg.h
    inc/stm32f7-hal/stm32f7xx_ll_fmc.h
    inc/stm32f7-hal/stm32f7xx_ll_sdmmc.h
    inc/stm32f7-hal/stm32f7xx_ll_usb.h
    src/stm32f7xx_hal_conf.c
    src/cmsis/system_stm32f7xx.c
    src/cmsis/vectors_stm32f7xx.c
    src/stm32f7-hal/stm32f7xx_hal.c
    src/stm32f7-hal/stm32f7xx_hal_adc.c
    src/stm32f7-hal/stm32f7xx_hal_adc_ex.c
    src/stm32f7-hal/stm32f7xx_hal_can.c
    src/stm32f7-hal/stm32f7xx_hal_cec.c
    src/stm32f7-hal/stm32f7xx_hal_cortex.c
    src/stm32f7-hal/stm32f7xx_hal_crc.c
    src/stm32f7-hal/stm32f7xx_hal_crc_ex.c
    src/stm32f7-hal/stm32f7xx_hal_cryp.c
    src/stm32f7-hal/stm32f7xx_hal_cryp_ex.c
    src/stm32f7-hal/stm32f7xx_hal_dac.c
    src/stm32f7-hal/stm32f7xx_hal_dac_ex.c
    src/stm32f7-hal/stm32f7xx_hal_dcmi.c
    src/stm32f7-hal/stm32f7xx_hal_dcmi_ex.c
    src/stm32f7-hal/stm32f7xx_hal_dfsdm.c
    src/stm32f7-hal/stm32f7xx_hal_dma.c
    src/stm32f7-hal/stm32f7xx_hal_dma_ex.c
    src/stm32f7-hal/stm32f7xx_hal_dma2d.c
    src/stm32f7-hal/stm32f7xx_hal_dsi.c
    src/stm32f7-hal/stm32f7xx_hal_eth.c
    src/stm32f7-hal/stm32f7xx_hal_flash.c
    src/stm32f7-hal/stm32f7xx_hal_flash_ex.c
    src/stm32f7-hal/stm32f7xx_hal_gpio.c
    src/stm32f7-hal/stm32f7xx_hal_hash.c
    src/stm32f7-hal/stm32f7xx_hal_hash_ex.c
    src/stm32f7-hal/stm32f7xx_hal_hcd.c
    src/stm32f7-hal/stm32f7xx_hal_i2c.c
    src/stm32f7-hal/stm32f7xx_hal_i2c_ex.c
    src/stm32f7-hal/stm32f7xx_hal_i2s.c
    src/stm32f7-hal/stm32f7xx_hal_irda.c
    src/stm32f7-hal/stm32f7xx_hal_iwdg.c
    src/stm32f7-hal/stm32f7xx_hal_jpeg.c
    src/stm32f7-hal/stm32f7xx_hal_lptim.c
    src/stm32f7-hal/stm32f7xx_hal_ltdc.c
    src/stm32f7-hal/stm32f7xx_hal_ltdc_ex.c
    src/stm32f7-hal/stm32f7xx_hal_mdios.c
    src/stm32f7-hal/stm32f7xx_hal_msp_template.c
    src/stm32f7-hal/stm32f7xx_hal_nand.c
    src/stm32f7-hal/stm32f7xx_hal_nor.c
    src/stm32f7-hal/stm32f7xx_hal_pcd.c
    src/stm32f7-hal/stm32f7xx_hal_pcd_ex.c
    src/stm32f7-hal/stm32f7xx_hal_pwr.c
    src/stm32f7-hal/stm32f7xx_hal_pwr_ex.c
    src/stm32f7-hal/stm32f7xx_hal_qspi.c
    src/stm32f7-hal/stm32f7xx_hal_rcc.c
    src/stm32f7-hal/stm32f7xx_hal_rcc_ex.c
    src/stm32f7-hal/stm32f7xx_hal_rng.c
    src/stm32f7-hal/stm32f7xx_hal_rtc.c
    src/stm32f7-hal/stm32f7xx_hal_rtc_ex.c
    src/stm32f7-hal/stm32f7xx_hal_sai.c
    src/stm32f7-hal/stm32f7xx_hal_sai_ex.c
    src/stm32f7-hal/stm32f7xx_hal_sd.c
    src/stm32f7-hal/stm32f7xx_hal_sdram.c
    src/stm32f7-hal/stm32f7xx_hal_smartcard.c
    src/stm32f7-hal/stm32f7xx_hal_smartcard_ex.c
    src/stm32f7-hal/stm32f7xx_hal_spdifrx.c
    src/stm32f7-hal/stm32f7xx_hal_spi.c
    src/stm32f7-hal/stm32f7xx_hal_sram.c
    src/stm32f7-hal/stm32f7xx_hal_tim.c
    src/stm32f7-hal/stm32f7xx_hal_tim_ex.c
    src/stm32f7-hal/stm32f7xx_hal_uart.c
    src/stm32f7-hal/stm32f7xx_hal_usart.c
    src/stm32f7-hal/stm32f7xx_hal_wwdg.c
    src/stm32f7-hal/stm32f7xx_ll_fmc.c
    src/stm32f7-hal/stm32f7xx_ll_sdmmc.c
    src/stm32f7-hal/stm32f7xx_ll_usb.c
)
