add_library(hal_st.synchronous_stm32fxxx ${HALST_EXCLUDE_FROM_ALL} STATIC)
install(TARGETS hal_st.synchronous_stm32fxxx EXPORT halStTargets)

target_include_directories(hal_st.synchronous_stm32fxxx PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(hal_st.synchronous_stm32fxxx PUBLIC
    hal.synchronous_interfaces
    hal_st.stm32fxxx
)

target_sources(hal_st.synchronous_stm32fxxx PRIVATE
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:SynchronousGpioStm.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:SynchronousGpioStm.hpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:SynchronousFlashInternalStm.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:SynchronousFlashInternalStm.hpp>
    SynchronousRandomDataGeneratorStm.cpp
    SynchronousRandomDataGeneratorStm.hpp
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:SynchronousSpiMasterStm.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU},stm32wb55>>:SynchronousSpiMasterStm.hpp>
    SynchronousUartStm.cpp
    SynchronousUartStm.hpp
    $<$<STREQUAL:${TARGET_MCU_FAMILY},stm32f7xx>:SynchronousQuadSpiStm.cpp>
    $<$<STREQUAL:${TARGET_MCU_FAMILY},stm32f7xx>:SynchronousQuadSpiStm.hpp>
    TimeKeeperStm.cpp
    TimeKeeperStm.hpp
)
