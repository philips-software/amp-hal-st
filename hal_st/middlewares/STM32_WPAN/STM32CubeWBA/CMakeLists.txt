# cmake-dependency-scan pkg:github/STMicroelectronics/STM32CubeWBA@502fbc072efc4d253459d82ba77dcd4c434837f7#Middlewares/ST/STM32_WPAN

add_library(hal_st.stm32_wpan_stm32wbaxx STATIC)
emil_build_for(hal_st.stm32_wpan_stm32wbaxx TARGET_MCU_FAMILY stm32wbaxx PREREQUISITE_BOOL HALST_STANDALONE)

set_target_properties(hal_st.stm32_wpan_stm32wbaxx PROPERTIES COMPILE_DEFINITIONS "BLE")

target_include_directories(hal_st.stm32_wpan_stm32wbaxx PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/app_specific>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ble>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ble/stack/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ble/stack/include/auto>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ble/svc/Inc>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/link_layer/ll_cmd_lib/config/ble_basic>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/link_layer/ll_cmd_lib/inc>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/link_layer/ll_cmd_lib/inc/ot_inc>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/link_layer/ll_sys/inc>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(hal_st.stm32_wpan_stm32wbaxx PUBLIC
    st.hal_driver
)

target_sources(hal_st.stm32_wpan_stm32wbaxx PRIVATE
    app_specific/power_table.c
    app_specific/scm.c
    app_specific/scm.h

    ble/svc/Inc/common_blesvc.h
    ble/svc/Src/svc_ctl.c

    link_layer/ll_sys/inc/ll_sys_sequencer.h
    link_layer/ll_sys/inc/ll_sys_startup.h
    link_layer/ll_sys/inc/ll_sys.h

    link_layer/ll_sys/src/ll_sys_cs.c
    link_layer/ll_sys/src/ll_sys_dp_slp.c
    link_layer/ll_sys/src/ll_sys_intf.c
    link_layer/ll_sys/src/ll_sys_startup.c
)
