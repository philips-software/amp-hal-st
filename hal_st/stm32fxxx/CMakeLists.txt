add_library(hal_st.stm32fxxx ${HALST_EXCLUDE_FROM_ALL} STATIC)
install(TARGETS hal_st.stm32fxxx EXPORT halStTargets)

target_include_directories(hal_st.stm32fxxx PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

if (HALST_INCLUDE_DEFAULT_INIT)
    # hal_st.default_init is an OBJECT library
    # it is important that this dependency is listed
    # first.
    target_link_libraries(hal_st.stm32fxxx PUBLIC
        $<TARGET_OBJECTS:hal_st.default_init>
    )
endif()

target_link_libraries(hal_st.stm32fxxx PUBLIC
    hal.interfaces
    infra.timer
    hal_st.std_periph
    hal_st.cortex
)

target_sources(hal_st.stm32fxxx PRIVATE
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:AnalogToDigitalPinStm.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:AnalogToDigitalPinStm.hpp>
    $<$<STREQUAL:${TARGET_MCU},stm32wb55>:BackupRamStm.cpp>
    $<$<STREQUAL:${TARGET_MCU},stm32wb55>:BackupRamStm.hpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:CanStm.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:CanStm.hpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:DacStm.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:DacStm.hpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:DigitalToAnalogPinStm.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:DigitalToAnalogPinStm.hpp>
    DmaStm.cpp
    DmaStm.hpp
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:EthernetMacStm.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:EthernetMacStm.hpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:EthernetSmiStm.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:EthernetSmiStm.hpp>
    FlashInternalStm.cpp
    FlashInternalStm.hpp
    GpioStm.cpp
    GpioStm.hpp
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:I2cStm.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:I2cStm.hpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:QuadSpiStm.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:QuadSpiStm.hpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:QuadSpiStmDma.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:QuadSpiStmDma.hpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:RandomDataGeneratorStm.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:RandomDataGeneratorStm.hpp>
    ResetStm.cpp
    ResetStm.hpp
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:SdRamStm.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:SdRamStm.hpp>
    SpiDataSizeConfiguratorStm.cpp
    SpiDataSizeConfiguratorStm.hpp
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:SingleSpeedQuadSpiStmDma.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:SingleSpeedQuadSpiStmDma.hpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:SpiMasterStmDma.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:SpiMasterStmDma.hpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:SpiSlaveStmDma.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:SpiSlaveStmDma.hpp>
    SystemTickStm.cpp
    SystemTickStm.hpp
    SystemTickTimerService.cpp
    SystemTickTimerService.hpp
    UartStm.cpp
    UartStm.hpp
    UartStmDma.cpp
    UartStmDma.hpp
    $<$<OR:$<STREQUAL:${TARGET_MCU},stm32wb55>,$<STREQUAL:${TARGET_MCU_FAMILY},stm32f7xx>>:UartStmDuplexDma.cpp>
    $<$<OR:$<STREQUAL:${TARGET_MCU},stm32wb55>,$<STREQUAL:${TARGET_MCU_FAMILY},stm32f7xx>>:UartStmDuplexDma.hpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:UsbLinkLayerStm.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:UsbLinkLayerStm.hpp>
    UniqueDeviceId.cpp
    UniqueDeviceId.hpp
    WatchDogStm.cpp
    WatchDogStm.hpp
)

if ("${TARGET_MCU_VENDOR}" STREQUAL st)
    if (${TARGET_MCU} STREQUAL stm32f372)
        set(gpio_xml "${CMAKE_CURRENT_SOURCE_DIR}/ip/GPIO-STM32F373_gpio_v1_0_Modes.xml")
        set(mcu_xml "${CMAKE_CURRENT_SOURCE_DIR}/mcu/STM32F373C(8-B-C)Tx.xml")
    elseif (${TARGET_MCU} STREQUAL stm32f407)
        set(gpio_xml "${CMAKE_CURRENT_SOURCE_DIR}/ip/GPIO-STM32F417_gpio_v1_0_Modes.xml")
        set(mcu_xml "${CMAKE_CURRENT_SOURCE_DIR}/mcu/STM32F407V(E-G)Tx.xml")
    elseif (${TARGET_MCU} STREQUAL stm32f429)
        set(gpio_xml "${CMAKE_CURRENT_SOURCE_DIR}/ip/GPIO-STM32F427_gpio_v1_0_Modes.xml")
        set(mcu_xml "${CMAKE_CURRENT_SOURCE_DIR}/mcu/STM32F429Z(E-G-I)Tx.xml")
    elseif (${TARGET_MCU} STREQUAL stm32f746)
        set(gpio_xml "${CMAKE_CURRENT_SOURCE_DIR}/ip/GPIO-STM32F746_gpio_v1_0_Modes.xml")
        set(mcu_xml "${CMAKE_CURRENT_SOURCE_DIR}/mcu/STM32F746Z(E-G)Tx.xml")
    elseif (${TARGET_MCU} STREQUAL stm32f767)
        set(gpio_xml "${CMAKE_CURRENT_SOURCE_DIR}/ip/GPIO-STM32F76x_gpio_v1_0_Modes.xml")
        set(mcu_xml "${CMAKE_CURRENT_SOURCE_DIR}/mcu/STM32F767Z(G-I)Tx.xml")
    elseif (${TARGET_MCU_VARIANT} STREQUAL stm32wb55cg)
        set(gpio_xml "${CMAKE_CURRENT_SOURCE_DIR}/ip/GPIO-STM32WB55x_gpio_v1_0_Modes.xml")
        set(mcu_xml "${CMAKE_CURRENT_SOURCE_DIR}/mcu/STM32WB55CGUx.xml")
    elseif (${TARGET_MCU_VARIANT} STREQUAL stm32wb55rg)
        set(gpio_xml "${CMAKE_CURRENT_SOURCE_DIR}/ip/GPIO-STM32WB55x_gpio_v1_0_Modes.xml")
        set(mcu_xml "${CMAKE_CURRENT_SOURCE_DIR}/mcu/STM32WB55RGVx.xml")
    else()
        message(FATAL_ERROR "Unknown TARGET_MCU \"${TARGET_MCU}\"")
    endif()
else()
    set(gpio_xml "${CMAKE_CURRENT_SOURCE_DIR}/ip/GPIO-STM32F76x_gpio_v1_0_Modes.xml")
    set(mcu_xml "${CMAKE_CURRENT_SOURCE_DIR}/mcu/STM32F767Z(G-I)Tx.xml")
endif()

generate_xslt(hal_st.stm32fxxx generated/stm32fxxx/PinoutTableDefaultStructure.xml GeneratePinoutTableStructure.xsl PinoutTableItems.xml
    --stringparam gpio-document ${gpio_xml} --stringparam mcu-document ${mcu_xml})
generate_xslt(hal_st.stm32fxxx generated/stm32fxxx/PinoutTableDefault.cpp GeneratePinoutTableCpp.xsl "${CMAKE_CURRENT_BINARY_DIR}/generated/stm32fxxx/PinoutTableDefaultStructure.xml")
generate_xslt(hal_st.stm32fxxx generated/stm32fxxx/PinoutTableDefault.hpp GeneratePinoutTableHpp.xsl "${CMAKE_CURRENT_BINARY_DIR}/generated/stm32fxxx/PinoutTableDefaultStructure.xml")

generate_xslt(hal_st.stm32fxxx generated/stm32fxxx/PeripheralTableStructure.xml GeneratePeripheralTableStructure.xsl $<IF:$<STREQUAL:${TARGET_MCU_FAMILY},stm32wbxx>,PeripheralTableWbxx.xml,PeripheralTableFxxx.xml>
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
