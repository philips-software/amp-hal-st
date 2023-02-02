add_library(hal_st.startup ${HALST_EXCLUDE_FROM_ALL} OBJECT)
install(TARGETS hal_st.startup EXPORT halStTargets)

target_sources(hal_st.startup PRIVATE
    assert.c
    cxx.cpp
    exit.c
    sbrk.c
    startup.c
    syscalls.c
)
