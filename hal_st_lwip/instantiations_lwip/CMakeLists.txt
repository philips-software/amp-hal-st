add_library(hal_st.instantiations_lwip STATIC)
emil_build_for(hal_st.instantiations_lwip TARGET_MCU_FAMILY stm32f4xx
                                          TARGET_MCU_FAMILY stm32f7xx
                                          TARGET_MCU_FAMILY stm32h5xx
                                          PREREQUISITE_BOOL HALST_STANDALONE)

target_include_directories(hal_st.instantiations_lwip PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(hal_st.instantiations_lwip PUBLIC
    infra.event
    infra.timer
    lwip.lwip_cpp
    services.network
    hal_st.stm32fxxx
)

target_sources(hal_st.instantiations_lwip PRIVATE
    EthernetSmiObserver.cpp
    EthernetSmiObserver.hpp
)

list(APPEND EthernetFxxxFamilies stm32f4xx stm32f7xx)

if (TARGET_MCU_FAMILY IN_LIST EthernetFxxxFamilies)
    target_sources(hal_st.instantiations_lwip PRIVATE
        Ethernet.cpp
        Ethernet.hpp
    )
endif()
