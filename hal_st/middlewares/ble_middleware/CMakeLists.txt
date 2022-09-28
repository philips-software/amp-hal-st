if ("${TARGET_MCU_VENDOR}" STREQUAL st AND "${TARGET_MCU_FAMILY}" STREQUAL stm32wbxx)

    add_library(hal_st.ble_middleware EXCLUDE_FROM_ALL STATIC)
    install(TARGETS hal_st.ble_middleware EXPORT halStTargets)

    target_include_directories(hal_st.ble_middleware PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../../..>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )

    target_link_libraries(hal_st.ble_middleware PUBLIC
        hal_st.default_init
        hal_st.std_periph
        hal_st.stm32_wpan
        hal_st.stm32fxxx
        hal_st.synchronous_stm32fxxx
        services.ble
        services.util
        services.tracer
        services.network
    )

    target_sources(hal_st.ble_middleware PRIVATE
        BondStorageSt.cpp
        BondStorageSt.hpp
        GapPeripheralSt.cpp
        GapPeripheralSt.hpp
        GattServerSt.cpp
        GattServerSt.hpp
        HciEventObserver.hpp
        SystemTransportLayer.cpp
        SystemTransportLayer.hpp
        TracingGapPeripheralSt.cpp
        TracingGapPeripheralSt.hpp
        TracingGattServerSt.cpp
        TracingGattServerSt.hpp
        TracingSystemTransportLayer.cpp
        TracingSystemTransportLayer.hpp
    )

endif()
