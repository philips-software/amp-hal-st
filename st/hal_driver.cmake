function(add_hal_driver target_name hal_driver cmsis)

    add_library(${target_name} STATIC)

    file(GLOB st_include RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${cmsis}/Device/ST/STM32*)

    target_include_directories(${target_name} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/${hal_driver}/Inc>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/${cmsis}/Core/Include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/${st_include}/Include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/hal_conf>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${hal_driver}/Inc>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${cmsis}/Core/Include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${st_include}/Include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/hal_conf>"
    )

    target_compile_definitions(${target_name} PUBLIC
        USE_HAL_DRIVER=1
        USE_FULL_LL_DRIVER=1
        $<$<NOT:$<CONFIG:MinSizeRel>>:USE_FULL_ASSERT=1>
    )

    # Assembler does not understand -Werror
    set_target_properties(${target_name} PROPERTIES COMPILE_WARNING_AS_ERROR Off)

    file(GLOB sources_in RELATIVE ${CMAKE_CURRENT_LIST_DIR}
        ${hal_driver}/Inc/*.h
        ${hal_driver}/Src/*.c
        ${cmsis}/Core/Include/*.h
        ${cmsis}/Device/ST/*/Include/*.h
        ${cmsis}/Device/ST/*/Source/Templates/system_*.c
        hal_conf/*_hal_conf.h
    )
    set(sources)
    foreach(source ${sources_in})
        if (NOT ${source} MATCHES "_template.")
            list(APPEND sources ${source})
        endif()
    endforeach()

    target_sources(${target_name} PRIVATE
        ${sources}
    )

    file(GLOB startup_source
        ${cmsis}/Device/ST/*/Source/Templates/gcc/startup_${TARGET_MCU}xx.s
        ${cmsis}/Device/ST/*/Source/Templates/gcc/startup_${TARGET_MCU}xx_cm4.s
    )

    if (startup_source)
        set_target_properties(${target_name} PROPERTIES HALST_STARTUP_SOURCE ${startup_source})
    endif()

endfunction()

define_property(TARGET PROPERTY HALST_STARTUP_SOURCE  BRIEF_DOCS "Startup source" FULL_DOCS "The source file that contains the startup and interrupt vector code.")
