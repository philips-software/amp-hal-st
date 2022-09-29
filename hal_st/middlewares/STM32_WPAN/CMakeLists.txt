if ("${TARGET_MCU_VENDOR}" STREQUAL st AND "${TARGET_MCU_FAMILY}" STREQUAL stm32wbxx)

    add_library(hal_st.stm32_wpan EXCLUDE_FROM_ALL STATIC)
    install(TARGETS hal_st.stm32_wpan EXPORT halStTargets)

    target_include_directories(hal_st.stm32_wpan PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/app_specific>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ble>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ble/core>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ble/core/auto>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ble/core/template>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/interface/patterns/ble_thread>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/interface/patterns/ble_thread/shci>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/interface/patterns/ble_thread/tl>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/utilities>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )

    target_link_libraries(hal_st.stm32_wpan PUBLIC
        hal_st.cmsis_arm
        hal_st.std_periph_stm32wbxx
    )

    target_sources(hal_st.stm32_wpan PRIVATE
        ble/core/auto/ble_gap_aci.c
        ble/core/auto/ble_gap_aci.h
        ble/core/auto/ble_gatt_aci.c
        ble/core/auto/ble_gatt_aci.h
        ble/core/auto/ble_hal_aci.c
        ble/core/auto/ble_hal_aci.h
        ble/core/auto/ble_hci_le.c
        ble/core/auto/ble_hci_le.h
        ble/core/auto/ble_l2cap_aci.c
        ble/core/auto/ble_l2cap_aci.h
        ble/core/template/osal.c
        ble/core/template/osal.h
        ble/svc/Src/bls.c
        ble/svc/Src/svc_ctl.c
        interface/patterns/ble_thread/shci/shci.c
        interface/patterns/ble_thread/shci/shci.h
        interface/patterns/ble_thread/tl/hci_tl.c
        interface/patterns/ble_thread/tl/hci_tl.h
        interface/patterns/ble_thread/tl/hci_tl_if.c
        interface/patterns/ble_thread/tl/shci_tl.c
        interface/patterns/ble_thread/tl/shci_tl.h
        interface/patterns/ble_thread/tl/shci_tl_if.c
        interface/patterns/ble_thread/tl/tl_mbox.c
        interface/patterns/ble_thread/tl/tl.h

        #Init workaround
        utilities/otp.c
        utilities/stm_list.c
        utilities/dbg_trace.c
        utilities/stm_queue.c
        app_specific/target/hw_ipcc.c
    )

endif()
