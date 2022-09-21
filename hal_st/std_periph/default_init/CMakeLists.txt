add_library(hal_st.default_init EXCLUDE_FROM_ALL OBJECT)
install(TARGETS hal_st.default_init EXPORT halStTargets)

target_include_directories(hal_st.default_init PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(hal_st.default_init PUBLIC
    hal_st.std_periph
)

target_sources(hal_st.default_init PRIVATE
    DefaultClock.h
    DefaultInit.c
    $<$<STREQUAL:${TARGET_MCU},stm32f372>:DefaultClockSTM32F372.c>
    $<$<STREQUAL:${TARGET_MCU},stm32f407>:DefaultClockSTM32F407.c>
    $<$<STREQUAL:${TARGET_MCU},stm32f429>:DefaultClockSTM32F429.c>
    $<$<STREQUAL:${TARGET_MCU},stm32f746>:DefaultClockSTM32F746.c>
    $<$<STREQUAL:${TARGET_MCU},stm32f767>:DefaultClockSTM32F767.c>
    $<$<STREQUAL:${TARGET_MCU},stm32wb55>:DefaultClockSTM32WB55.c>
)
