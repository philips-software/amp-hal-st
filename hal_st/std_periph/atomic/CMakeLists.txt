add_library(hal_st.atomic STATIC)
if (NOT TARGET_MCU_FAMILY STREQUAL stm32f0xx)
    set_property(TARGET hal_st.atomic PROPERTY EXCLUDE_FROM_ALL TRUE)
endif()
install(TARGETS hal_st.atomic EXPORT halStAtomicExports)

target_link_libraries(
    hal_st.std_periph_stm32f0xx
)

target_sources(hal_st.atomic PRIVATE
    atomic.cpp
)
