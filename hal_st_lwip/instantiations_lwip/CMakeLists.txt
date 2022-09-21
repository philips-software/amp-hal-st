add_library(hal_st.instantiations_lwip EXCLUDE_FROM_ALL STATIC)
install(TARGETS hal_st.instantiations_lwip EXPORT halStLwipTargets)

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
    Ethernet.cpp
    Ethernet.hpp
    EthernetSmiObserver.cpp
    EthernetSmiObserver.hpp
)
