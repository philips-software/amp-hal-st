add_library(hal_st.synchronous_stm32fxxx ${HALST_EXCLUDE_FROM_ALL} STATIC)

target_include_directories(hal_st.synchronous_stm32fxxx PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(hal_st.synchronous_stm32fxxx PUBLIC
    hal.synchronous_interfaces
    hal_st.stm32fxxx
    services.util
)

target_sources(hal_st.synchronous_stm32fxxx PRIVATE
    SynchronousGpioStm.cpp
    SynchronousGpioStm.hpp
    SynchronousFlashInternalStm.cpp
    SynchronousFlashInternalStm.hpp
    SynchronousRandomDataGeneratorStm.cpp
    SynchronousRandomDataGeneratorStm.hpp
    SynchronousSpiMasterStm.cpp
    SynchronousSpiMasterStm.hpp
    $<$<NOT:$<STREQUAL:${TARGET_MCU_FAMILY},stm32g0xx>>:SynchronousUartStm.cpp>
    $<$<NOT:$<STREQUAL:${TARGET_MCU_FAMILY},stm32g0xx>>:SynchronousUartStm.hpp>
    $<$<STREQUAL:${TARGET_MCU_FAMILY},stm32f7xx>:SynchronousQuadSpiStm.cpp>
    $<$<STREQUAL:${TARGET_MCU_FAMILY},stm32f7xx>:SynchronousQuadSpiStm.hpp>
    TimeKeeperStm.cpp
    TimeKeeperStm.hpp
)
