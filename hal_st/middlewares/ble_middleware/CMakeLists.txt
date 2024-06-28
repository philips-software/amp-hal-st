add_library(hal_st.ble_middleware STATIC)
emil_build_for(hal_st.ble_middleware TARGET_MCU_FAMILY stm32wbxx stm32wbaxx PREREQUISITE_BOOL HALST_STANDALONE)

set(BLE_STACK_LIBRARY_FILE  ${CMAKE_CURRENT_LIST_DIR}/../STM32_WPAN/STM32CubeWBA/ble/stack/lib/stm32wba_ble_stack_basic.a)
set(LINK_LAYER_LIBRARY_FILE ${CMAKE_CURRENT_LIST_DIR}/../STM32_WPAN/STM32CubeWBA/link_layer/ll_cmd_lib/lib/LinkLayer_BLE_Basic_lib.a)

target_include_directories(hal_st.ble_middleware PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(hal_st.ble_middleware PUBLIC
    hal_st.stm32_wpan
    services.ble
    services.util
    services.tracer
)

if ("${TARGET_MCU_FAMILY}" STREQUAL stm32wbaxx)
    target_link_libraries(hal_st.ble_middleware PRIVATE
        "$<LINK_GROUP:RESCAN,hal_st.ble_platform,${BLE_STACK_LIBRARY_FILE},${LINK_LAYER_LIBRARY_FILE}>"
    )
endif()

target_sources(hal_st.ble_middleware PRIVATE
    BondStorageSt.cpp
    BondStorageSt.hpp
    GapCentralSt.cpp
    GapCentralSt.hpp
    GapPeripheralSt.cpp
    GapPeripheralSt.hpp
    GapSt.cpp
    GapSt.hpp
    GattClientSt.cpp
    GattClientSt.hpp
    GattServerSt.cpp
    GattServerSt.hpp
    HciEventObserver.hpp
    SystemTransportLayer.cpp
    SystemTransportLayer.hpp
    TracingGapCentralSt.cpp
    TracingGapCentralSt.hpp
    TracingGapPeripheralSt.cpp
    TracingGapPeripheralSt.hpp
    TracingGattClientSt.cpp
    TracingGattClientSt.hpp
    TracingGattServerSt.cpp
    TracingGattServerSt.hpp
    TracingSystemTransportLayer.cpp
    TracingSystemTransportLayer.hpp
)

add_subdirectory(platform)
