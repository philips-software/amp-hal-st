add_library(hal_st.ble_platform STATIC)
emil_build_for(hal_st.ble_platform TARGET_MCU_FAMILY stm32wbaxx PREREQUISITE_BOOL HALST_STANDALONE)

target_include_directories(hal_st.ble_platform PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../../..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(hal_st.ble_platform PUBLIC
    hal_st.stm32_wpan_${TARGET_MCU_FAMILY}
    services.ble
    services.util
    services.tracer
)

target_sources(hal_st.ble_platform PRIVATE
    BlePlatform.cpp
    BlePlatform.hpp
    LinkLayerPlatform.cpp
)
