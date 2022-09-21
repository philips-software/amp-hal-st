add_library(hal_st.atomic EXCLUDE_FROM_ALL STATIC)
install(TARGETS hal_st.atomic EXPORT halStAtomicExports)

target_link_libraries(
    hal_st.std_periph_stm32f0xx
)

target_sources(hal_st.atomic PRIVATE
    atomic.cpp
)
