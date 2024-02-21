function(hal_st_fetch_table_generator)
    # This function will first try to use `find_package` to import
    # amp-hal-st table generators. If there is an installed version
    # of amp-hal-st present those table generators will be used.
    # Otherwise the latest hal-st release will be downloaded and those
    # table generators will be used instead.
    #
    # (See: https://cmake.org/cmake/help/latest/module/FetchContent.html#commands)

    if (EMIL_HOST_BUILD AND NOT CMAKE_CROSSCOMPILING)
        # In a host build where we are not cross-compiling we use the built table generators
        return()
    endif()

    FetchContent_GetProperties(table_generator)
    if (table_generator_POPULATED)
        return()
    endif()

    set(hal_st_version "2.1.0") # x-release-please-version

    if (CMAKE_HOST_WIN32)
        set(os_postfix "win64")
        set(host_executable_postfix ".exe")
    elseif (CMAKE_HOST_APPLE)
        set(os_postfix "Darwin")
    elseif (CMAKE_HOST_UNIX)
        set(os_postfix "Linux")
    else()
        message(FATAL_ERROR "No suitable table generators found for ${CMAKE_HOST_SYSTEM_NAME} (${CMAKE_HOST_SYSTEM_PROCESSOR})")
    endif()

    FetchContent_Declare(table_generator
        URL https://github.com/philips-software/amp-hal-st/releases/download/v${hal_st_version}/hal_st-${hal_st_version}-${os_postfix}.zip
        FIND_PACKAGE_ARGS NAMES hal_st GLOBAL
    )
    FetchContent_MakeAvailable(table_generator)

    if (NOT ${table_generators_FOUND})
        if (NOT TARGET tools.st_table_generator)
            add_executable(tools.st_table_generator IMPORTED GLOBAL)
            set_target_properties(tools.st_table_generator PROPERTIES
                IMPORTED_LOCATION "${table_generators_SOURCE_DIR}/bin/tools.st_table_generator${host_executable_postfix}"
            )
        endif()
    else()
        message(STATUS "Using table generator from installed location")
    endif()
endfunction()

function(st_table_generator target mcu_xml)
    hal_st_fetch_table_generator()

    cmake_path(SET generated_dir_tables "generated/stm32fxxx")
    cmake_path(ABSOLUTE_PATH generated_dir_tables BASE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} NORMALIZE OUTPUT_VARIABLE generated_dir_tables)

    set(generated_files "${generated_dir_tables}/PeripheralTable.cpp" "${generated_dir_tables}/PeripheralTable.hpp")

    add_custom_command(
        OUTPUT ${generated_files}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${generated_dir_tables}
        COMMAND tools.st_table_generator "${mcu_xml}" ${generated_dir_tables}
        MAIN_DEPENDENCY "${mcu_xml}"
        DEPENDS tools.st_table_generator
    )

    target_sources(${target} PRIVATE
        ${generated_files}
    )

    target_include_directories(${target} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )
endfunction()
