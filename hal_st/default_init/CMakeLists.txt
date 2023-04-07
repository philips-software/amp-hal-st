add_library(hal_st.default_init ${HALST_EXCLUDE_FROM_ALL} OBJECT)
install(TARGETS hal_st.default_init EXPORT halStTargets)

target_include_directories(hal_st.default_init PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(hal_st.default_init PUBLIC
    st.hal_driver
    hal_st.cortex
)

target_sources(hal_st.default_init PRIVATE
    DefaultInit.cpp
)

function(halst_target_default_init target)
    target_link_libraries(${target} PUBLIC
        $<TARGET_OBJECTS:hal_st.default_init>
    )
endfunction()
