# cmake-dependency-scan pkg:github/STMicroelectronics/STM32CubeWBA@502fbc072efc4d253459d82ba77dcd4c434837f7#Middlewares/ST/STM32_WPAN

add_library(hal_st.stm32_wpan_stm32wbaxx STATIC)
emil_build_for(hal_st.stm32_wpan_stm32wbaxx TARGET_MCU_FAMILY stm32wbaxx PREREQUISITE_BOOL HALST_STANDALONE)

add_definitions(-DBLE)

target_include_directories(hal_st.stm32_wpan_stm32wbaxx PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/app_specific>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ble>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ble/stack/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ble/stack/include/auto>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ble/svc/Inc>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/link_layer/ll_cmd_lib/config/ble_full>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/link_layer/ll_cmd_lib/inc>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/link_layer/ll_cmd_lib/inc/ot_inc>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/link_layer/ll_sys/inc>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(hal_st.stm32_wpan_stm32wbaxx PUBLIC
    st.hal_driver
    ${CMAKE_CURRENT_LIST_DIR}/ble/stack/lib/stm32wba_ble_stack_full.a
    ${CMAKE_CURRENT_LIST_DIR}/link_layer/ll_cmd_lib/lib/LinkLayer_BLE_Full_lib.a
)

target_sources(hal_st.stm32_wpan_stm32wbaxx PRIVATE
    ble/svc/Inc/common_blesvc.h
    ble/svc/Src/svc_ctl.c

    # Link layer
    link_layer/ll_cmd_lib/config/ble_full/ll_fw_config.h
    link_layer/ll_cmd_lib/inc/_40nm_reg_files/DWC_ble154combo.h
    link_layer/ll_cmd_lib/inc/ot_inc/crypto.h
    link_layer/ll_cmd_lib/inc/ot_inc/error.h
    link_layer/ll_cmd_lib/inc/ot_inc/instance.h
    link_layer/ll_cmd_lib/inc/ot_inc/logging.h
    link_layer/ll_cmd_lib/inc/ot_inc/radio.h
    link_layer/ll_cmd_lib/inc/ot_inc/toolchain.h
    link_layer/ll_cmd_lib/inc/ot_inc/types.h
    link_layer/ll_cmd_lib/inc/bsp.h
    link_layer/ll_cmd_lib/inc/common_types.h
    link_layer/ll_cmd_lib/inc/dtm.h
    link_layer/ll_cmd_lib/inc/event_manager.h
    link_layer/ll_cmd_lib/inc/evnt_schdlr_gnrc_if.h
    link_layer/ll_cmd_lib/inc/hci.h
    link_layer/ll_cmd_lib/inc/ll_error.h
    link_layer/ll_cmd_lib/inc/ll_intf.h
    link_layer/ll_cmd_lib/inc/mac_host_intf.h
    link_layer/ll_cmd_lib/inc/mem_intf.h
    link_layer/ll_cmd_lib/inc/os_wrapper.h
    link_layer/ll_cmd_lib/inc/platform.h
    link_layer/ll_cmd_lib/inc/power_table.h
    link_layer/ll_cmd_lib/inc/pta.h
    link_layer/ll_cmd_lib/inc/ral.h
    link_layer/ll_cmd_lib/inc/rfd_dev_config.h

    link_layer/ll_sys/inc/ll_sys_sequencer.h
    link_layer/ll_sys/inc/ll_sys_startup.h
    link_layer/ll_sys/inc/ll_sys.h

    link_layer/ll_sys/src/ll_sys_cs.c
    link_layer/ll_sys/src/ll_sys_dp_slp.c
    link_layer/ll_sys/src/ll_sys_intf.c
    link_layer/ll_sys/src/ll_sys_startup.c
)
