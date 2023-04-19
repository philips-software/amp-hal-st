---
name: Add or update HAL driver
about: Import a new STM32[f,g,l,...] HAL driver
title: 'feat: add/update HAL driver for STM32x'
labels: ''
assignees: ''

---

# How to import a HAL driver
- [ ] Download STMCubeXX library from st.com, unzip
    - [ ] Apply patch if available
- [ ] From Drivers, copy STM32xxxx_HAL_Driver
    - [ ] Remove the user manual chm files; they are big
    - [ ] Add a version.txt specifying the version downloaded
    - [ ] Copy inc/stm32fxxx_hal_conf_template.h to hal_conf/stm32fxxx_hal_conf.h
        - [ ] Add #define HSE_VALUE to an extern unsigned int hse_value
- [ ] From Drivers/CMSIS, copy Core, Device, and top level files to CMSIS_STM32xxxx
    - [ ] In CMSIS_STM32xxxx/Source/Templates/gcc/startup_stm32xxxx.s:
        - [ ] Add declaration of Default_Handler_forwarded
        - [ ] Change Default_Handler to branch to Default_Handler_forwarded
- [ ] Add mem_stm32xxxx.ld files to ldscripts
- [ ] Add DefaultClockSTM32xxxx.c for Nucleo and/or Discovery boards to hal_st.stm32fxxx
- [ ] Download STM32CubeIDE, install
    - [ ] For microcontrollers used in a Nucleo:
        - [ ] From STM32CubeIDE_x.xx.x/STM32CubeIDE/plugins/com.st.stm32cube.common.mx_x.x.x.x/db/mcu, copy STM32xxxx(x-x)x.xml to hal_st/stm32fxxx/mcu
        - [ ] From STM32CubeIDE_x.xx.x/STM32CubeIDE/plugins/com.st.stm32cube.common.mx_x.x.x.x/db/mcu/IP, copy GPIO-STM32xxxx_gpio_v1_0_Modes.xml to hal_st/stm32fxxx/ip
        - [ ] Check that the namespace of these xml files is "http://mcd.rou.st.com/modules.php?name=mcu"
        - [ ] Update hal_st/stm32fxxx/CMakeLists with these files
- [ ] Update st/CMakeLists.txt
    - [ ] Add section to invoke add_hal_driver
    - [ ] Add/update list of target MCUs
