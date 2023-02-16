add_library(hal_st.cmsis_arm ${HALST_EXCLUDE_FROM_ALL} INTERFACE)
install(TARGETS hal_st.cmsis_arm EXPORT halStTargets)

target_include_directories(hal_st.cmsis_arm INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_compile_options(hal_st.cmsis_arm INTERFACE
    $<$<COMPILE_LANG_AND_ID:CXX,GNU>:-Wno-register>
)

target_sources(hal_st.cmsis_arm PRIVATE
    arm_common_tables.h
    arm_const_structs.h
    arm_math.h
    cmsis_armcc.h
    cmsis_armclang_ltm.h
    cmsis_armclang.h
    cmsis_compiler.h
    cmsis_gcc.h
    cmsis_iccarm.h
    cmsis_version.h
    core_armv8mbl.h
    core_armv8mml.h
    core_armv81mml.h
    core_cm0.h
    core_cm0plus.h
    core_cm1.h
    core_cm3.h
    core_cm4.h
    core_cm7.h
    core_cm23.h
    core_cm33.h
    core_cm35p.h
    core_cmFunc.h
    core_cmInstr.h
    core_cmSimd.h
    core_sc000.h
    core_sc300.h
    mpu_armv7.h
    mpu_armv8.h
    tz_context.h
)
