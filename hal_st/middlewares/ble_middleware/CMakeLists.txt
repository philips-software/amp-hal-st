if ("${TARGET_MCU_VENDOR}" STREQUAL st AND "${TARGET_MCU_FAMILY}" STREQUAL stm32wbxx)

    add_library(hal_st.ble_middleware ${HALST_EXCLUDE_FROM_ALL} STATIC)
    install(TARGETS hal_st.ble_middleware EXPORT halStTargets)

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

    target_sources(hal_st.ble_middleware PRIVATE
        BondStorageSt.cpp
        BondStorageSt.hpp
        GapCentralSt.cpp
        GapCentralSt.hpp
        GapPeripheralSt.cpp
        GapPeripheralSt.hpp
        GapSt.cpp
        GapSt.hpp
        GattServerSt.cpp
        GattServerSt.hpp
        HciEventObserver.hpp
        SystemTransportLayer.cpp
        SystemTransportLayer.hpp
        TracingGapCentralSt.cpp
        TracingGapCentralSt.hpp
        TracingGapPeripheralSt.cpp
        TracingGapPeripheralSt.hpp
        TracingGattServerSt.cpp
        TracingGattServerSt.hpp
        TracingSystemTransportLayer.cpp
        TracingSystemTransportLayer.hpp
    )

endif()
