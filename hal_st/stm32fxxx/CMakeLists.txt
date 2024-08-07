add_library(hal_st.stm32fxxx STATIC)
emil_build_for(hal_st.stm32fxxx TARGET_MCU_VENDOR st PREREQUISITE_BOOL HALST_STANDALONE)

target_include_directories(hal_st.stm32fxxx PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(hal_st.stm32fxxx PUBLIC
    hal.interfaces
    infra.timer
    st.hal_driver
    hal_st.cortex
)

target_sources(hal_st.stm32fxxx PRIVATE
    $<$<OR:$<STREQUAL:${TARGET_MCU},stm32wb55>,$<STREQUAL:${TARGET_MCU_FAMILY},stm32wbaxx>>:AdcDmaStm.cpp>
    $<$<OR:$<STREQUAL:${TARGET_MCU},stm32wb55>,$<STREQUAL:${TARGET_MCU_FAMILY},stm32wbaxx>>:AdcDmaStm.hpp>
    AnalogToDigitalPinStm.cpp
    AnalogToDigitalPinStm.hpp
    BackupRamStm.cpp
    BackupRamStm.hpp
    CanStm.cpp
    CanStm.hpp
    $<$<STREQUAL:${TARGET_MCU},stm32f407>:DefaultClockDiscoveryF407G.cpp>
    $<$<STREQUAL:${TARGET_MCU},stm32f407>:DefaultClockDiscoveryF407G.hpp>
    $<$<STREQUAL:${TARGET_MCU},stm32f746>:DefaultClockDiscoveryF746G.cpp>
    $<$<STREQUAL:${TARGET_MCU},stm32f746>:DefaultClockDiscoveryF746G.hpp>
    $<$<STREQUAL:${TARGET_MCU},stm32h573>:DefaultClockDiscoveryH573I.cpp>
    $<$<STREQUAL:${TARGET_MCU},stm32h573>:DefaultClockDiscoveryH573I.hpp>
    $<$<STREQUAL:${TARGET_MCU},stm32f429>:DefaultClockNucleoF429ZI.cpp>
    $<$<STREQUAL:${TARGET_MCU},stm32f429>:DefaultClockNucleoF429ZI.hpp>
    $<$<STREQUAL:${TARGET_MCU},stm32f767>:DefaultClockNucleoF767ZI.cpp>
    $<$<STREQUAL:${TARGET_MCU},stm32f767>:DefaultClockNucleoF767ZI.hpp>
    $<$<STREQUAL:${TARGET_MCU},stm32g070>:DefaultClockNucleoG070RB.cpp>
    $<$<STREQUAL:${TARGET_MCU},stm32g070>:DefaultClockNucleoG070RB.hpp>
    $<$<STREQUAL:${TARGET_MCU},stm32g431>:DefaultClockNucleoG431RB.cpp>
    $<$<STREQUAL:${TARGET_MCU},stm32g431>:DefaultClockNucleoG431RB.hpp>
    $<$<STREQUAL:${TARGET_MCU},stm32g474>:DefaultClockNucleoG474RE.cpp>
    $<$<STREQUAL:${TARGET_MCU},stm32g474>:DefaultClockNucleoG474RE.hpp>
    $<$<STREQUAL:${TARGET_MCU},stm32h563>:DefaultClockNucleoH563ZI.cpp>
    $<$<STREQUAL:${TARGET_MCU},stm32h563>:DefaultClockNucleoH563ZI.hpp>
    $<$<STREQUAL:${TARGET_MCU},stm32wb55>:DefaultClockNucleoWB55RG.cpp>
    $<$<STREQUAL:${TARGET_MCU},stm32wb55>:DefaultClockNucleoWB55RG.hpp>
    $<$<STREQUAL:${TARGET_MCU},stm32wba52>:DefaultClockNucleoWBA52CG.cpp>
    $<$<STREQUAL:${TARGET_MCU},stm32wba52>:DefaultClockNucleoWBA52CG.hpp>
    DigitalToAnalogPinStm.cpp
    DigitalToAnalogPinStm.hpp
    $<$<NOT:$<STREQUAL:${TARGET_MCU_FAMILY},stm32g0xx>>:DmaStm.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU_FAMILY},stm32g0xx>>:DmaStm.hpp>
    EthernetMacStm.cpp
    EthernetMacStm.hpp
    EthernetSmiStm.cpp
    EthernetSmiStm.hpp
    $<$<NOT:$<STREQUAL:${TARGET_MCU_FAMILY},stm32h5xx>>:FlashInternalStm.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU_FAMILY},stm32h5xx>>:FlashInternalStm.hpp>
    GpioStm.cpp
    GpioStm.hpp
    $<$<NOT:$<STREQUAL:${TARGET_MCU_FAMILY},stm32g0xx>>:I2cStm.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU_FAMILY},stm32g0xx>>:I2cStm.hpp>
    QuadSpiStm.cpp
    QuadSpiStm.hpp
    $<$<NOT:$<STREQUAL:${TARGET_MCU_FAMILY},stm32g0xx>>:QuadSpiStmDma.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU_FAMILY},stm32g0xx>>:QuadSpiStmDma.hpp>
    LowPowerStrategyStm.cpp
    LowPowerStrategyStm.hpp
    RandomDataGeneratorStm.cpp
    RandomDataGeneratorStm.hpp
    ResetStm.cpp
    ResetStm.hpp
    SdRamStm.cpp
    SdRamStm.hpp
    SpiDataSizeConfiguratorStm.cpp
    SpiDataSizeConfiguratorStm.hpp
    $<$<NOT:$<STREQUAL:${TARGET_MCU_FAMILY},stm32g0xx>>:SingleSpeedQuadSpiStmDma.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU_FAMILY},stm32g0xx>>:SingleSpeedQuadSpiStmDma.hpp>
    SpiMasterStm.cpp
    SpiMasterStm.hpp
    $<$<NOT:$<STREQUAL:${TARGET_MCU_FAMILY},stm32g0xx>>:SpiMasterStmDma.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU_FAMILY},stm32g0xx>>:SpiMasterStmDma.hpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU_FAMILY},stm32g0xx>>:SpiSlaveStmDma.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU_FAMILY},stm32g0xx>>:SpiSlaveStmDma.hpp>
    SystemTickStm.cpp
    SystemTickStm.hpp
    SystemTickTimerService.cpp
    SystemTickTimerService.hpp
    TimerStm.cpp
    TimerStm.hpp
    $<$<NOT:$<STREQUAL:${TARGET_MCU_FAMILY},stm32g0xx>>:UartStm.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU_FAMILY},stm32g0xx>>:UartStm.hpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU_FAMILY},stm32g0xx>>:UartStmDma.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU_FAMILY},stm32g0xx>>:UartStmDma.hpp>
    $<$<OR:$<STREQUAL:${TARGET_MCU},stm32wb55>,$<STREQUAL:${TARGET_MCU_FAMILY},stm32f7xx>,$<STREQUAL:${TARGET_MCU_FAMILY},stm32g4xx>,$<STREQUAL:${TARGET_MCU_FAMILY},stm32wbaxx>>:UartStmDuplexDma.cpp>
    $<$<OR:$<STREQUAL:${TARGET_MCU},stm32wb55>,$<STREQUAL:${TARGET_MCU_FAMILY},stm32f7xx>,$<STREQUAL:${TARGET_MCU_FAMILY},stm32g4xx>,$<STREQUAL:${TARGET_MCU_FAMILY},stm32wbaxx>>:UartStmDuplexDma.hpp>
    UsbLinkLayerStm.cpp
    UsbLinkLayerStm.hpp
    UniqueDeviceId.cpp
    UniqueDeviceId.hpp
    WatchDogStm.cpp
    WatchDogStm.hpp
)

if (TARGET_MCU_VENDOR STREQUAL st)
    if (HALST_XML_GPIO AND HALST_XML_STM32)
        set(gpio_xml "${CMAKE_SOURCE_DIR}/${HALST_XML_GPIO}")
        set(mcu_xml "${CMAKE_SOURCE_DIR}/${HALST_XML_STM32}")
    elseif (TARGET_MCU STREQUAL stm32f407)
        set(gpio_xml "${CMAKE_CURRENT_SOURCE_DIR}/ip/GPIO-STM32F417_gpio_v1_0_Modes.xml")
        set(mcu_xml "${CMAKE_CURRENT_SOURCE_DIR}/mcu/STM32F407V(E-G)Tx.xml")
    elseif (TARGET_MCU STREQUAL stm32f429)
        set(gpio_xml "${CMAKE_CURRENT_SOURCE_DIR}/ip/GPIO-STM32F427_gpio_v1_0_Modes.xml")
        set(mcu_xml "${CMAKE_CURRENT_SOURCE_DIR}/mcu/STM32F429Z(E-G-I)Tx.xml")
    elseif (TARGET_MCU STREQUAL stm32f746)
        set(gpio_xml "${CMAKE_CURRENT_SOURCE_DIR}/ip/GPIO-STM32F746_gpio_v1_0_Modes.xml")
        set(mcu_xml "${CMAKE_CURRENT_SOURCE_DIR}/mcu/STM32F746Z(E-G)Tx.xml")
    elseif (TARGET_MCU STREQUAL stm32f767)
        set(gpio_xml "${CMAKE_CURRENT_SOURCE_DIR}/ip/GPIO-STM32F76x_gpio_v1_0_Modes.xml")
        set(mcu_xml "${CMAKE_CURRENT_SOURCE_DIR}/mcu/STM32F767Z(G-I)Tx.xml")
    elseif (TARGET_MCU STREQUAL stm32g070)
        set(gpio_xml "${CMAKE_CURRENT_SOURCE_DIR}/ip/GPIO-STM32G07x_gpio_v1_0_Modes.xml")
        set(mcu_xml "${CMAKE_CURRENT_SOURCE_DIR}/mcu/STM32G070RBTx.xml")
    elseif (TARGET_MCU STREQUAL stm32g431)
        set(gpio_xml "${CMAKE_CURRENT_SOURCE_DIR}/ip/GPIO-STM32G43x_gpio_v1_0_Modes.xml")
        set(mcu_xml "${CMAKE_CURRENT_SOURCE_DIR}/mcu/STM32G431R(6-8-B)Tx.xml")
    elseif (TARGET_MCU STREQUAL stm32g474)
        set(gpio_xml "${CMAKE_CURRENT_SOURCE_DIR}/ip/GPIO-STM32G47x_gpio_v1_0_Modes.xml")
        set(mcu_xml "${CMAKE_CURRENT_SOURCE_DIR}/mcu/STM32G474R(B-C-E)Tx.xml")
    elseif (TARGET_MCU_VARIANT STREQUAL stm32wb55rg)
        set(gpio_xml "${CMAKE_CURRENT_SOURCE_DIR}/ip/GPIO-STM32WB55x_gpio_v1_0_Modes.xml")
        set(mcu_xml "${CMAKE_CURRENT_SOURCE_DIR}/mcu/STM32WB55RGVx.xml")
    elseif (TARGET_MCU STREQUAL stm32wba52)
        set(gpio_xml "${CMAKE_CURRENT_SOURCE_DIR}/ip/GPIO-STM32WBA52x_gpio_v1_0_Modes.xml")
        set(mcu_xml "${CMAKE_CURRENT_SOURCE_DIR}/mcu/STM32WBA52CGUx.xml")
    elseif (TARGET_MCU STREQUAL stm32h563)
        set(gpio_xml "${CMAKE_CURRENT_SOURCE_DIR}/ip/GPIO-STM32H56x_gpio_v1_0_Modes.xml")
        set(mcu_xml "${CMAKE_CURRENT_SOURCE_DIR}/mcu/STM32H563ZITx.xml")
    elseif (TARGET_MCU STREQUAL stm32h573)
        set(gpio_xml "${CMAKE_CURRENT_SOURCE_DIR}/ip/GPIO-STM32H56x_gpio_v1_0_Modes.xml")
        set(mcu_xml "${CMAKE_CURRENT_SOURCE_DIR}/mcu/STM32H573IIKxQ.xml")
    else()
        message(FATAL_ERROR "Unknown TARGET_MCU \"${TARGET_MCU}\". Please configure HALST_XML_GPIO and HALST_XML_STM32.")
    endif()

    generate_xslt(hal_st.stm32fxxx generated/stm32fxxx/PinoutTableDefaultStructure.xml GeneratePinoutTableStructure.xsl PinoutTableItems.xml
        --stringparam gpio-document ${gpio_xml} --stringparam mcu-document ${mcu_xml})
    generate_xslt(hal_st.stm32fxxx generated/stm32fxxx/PinoutTableDefault.cpp GeneratePinoutTableCpp.xsl "${CMAKE_CURRENT_BINARY_DIR}/generated/stm32fxxx/PinoutTableDefaultStructure.xml")
    generate_xslt(hal_st.stm32fxxx generated/stm32fxxx/PinoutTableDefault.hpp GeneratePinoutTableHpp.xsl "${CMAKE_CURRENT_BINARY_DIR}/generated/stm32fxxx/PinoutTableDefaultStructure.xml")

    if (TARGET_MCU_FAMILY STREQUAL stm32wbxx)
        set(peripheral_xml "PeripheralTableWbxx.xml")
    elseif (TARGET_MCU_FAMILY STREQUAL stm32wbaxx)
        set(peripheral_xml "PeripheralTableWbaxx.xml")
    elseif (TARGET_MCU_FAMILY STREQUAL stm32h5xx)
        set(peripheral_xml "PeripheralTableH5xx.xml")
    else()
        set(peripheral_xml "PeripheralTableFxxx.xml")
    endif()

    generate_xslt(hal_st.stm32fxxx generated/stm32fxxx/PeripheralTableStructure.xml GeneratePeripheralTableStructure.xsl ${peripheral_xml}
        --stringparam mcu-document ${mcu_xml})
    generate_xslt(hal_st.stm32fxxx generated/stm32fxxx/PeripheralTable.cpp GeneratePeripheralTableCpp.xsl "${CMAKE_CURRENT_BINARY_DIR}/generated/stm32fxxx/PeripheralTableStructure.xml")
    generate_xslt(hal_st.stm32fxxx generated/stm32fxxx/PeripheralTable.hpp GeneratePeripheralTableHpp.xsl "${CMAKE_CURRENT_BINARY_DIR}/generated/stm32fxxx/PeripheralTableStructure.xml")

    target_sources(hal_st.stm32fxxx PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}/generated/stm32fxxx/PinoutTableDefaultStructure.xml"
        "${CMAKE_CURRENT_BINARY_DIR}/generated/stm32fxxx/PinoutTableDefault.cpp"
        "${CMAKE_CURRENT_BINARY_DIR}/generated/stm32fxxx/PinoutTableDefault.hpp"
        "${CMAKE_CURRENT_BINARY_DIR}/generated/stm32fxxx/PeripheralTableStructure.xml"
        "${CMAKE_CURRENT_BINARY_DIR}/generated/stm32fxxx/PeripheralTable.cpp"
        "${CMAKE_CURRENT_BINARY_DIR}/generated/stm32fxxx/PeripheralTable.hpp"
    )
endif()
