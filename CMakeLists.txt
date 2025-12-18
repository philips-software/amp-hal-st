cmake_minimum_required(VERSION 3.24)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(HALST_STANDALONE On)
endif()

option(CMAKE_COMPILE_WARNING_AS_ERROR "Enable warnings-as-error" ON)
option(HALST_BUILD_TESTS "Enable building the tests" Off)
option(HALST_INCLUDE_DEFAULT_INIT "Include default initialization code; turn off when providing custom initialization" ON)
option(HALST_BUILD_EXAMPLES "Enable build of the examples" OFF)
option(HALST_BUILD_EXAMPLES_FREERTOS "Enable build of the FreeRTOS example" OFF)
set(HALST_XML_GPIO "" CACHE STRING "Optional: path to a GPIO-STM32xxx_gpio_vx_x_Modes.xml file that is not already supported by amp-hal-st. Usually found 'db/mcu/ip/GPIO-*' of CubeIDE/CubeMX.")
set(HALST_XML_STM32 "" CACHE STRING "Optional: path to a STM32xxxx.xml file that is not already supported by amp-hal-st. Usually found 'db/mcu/' of CubeIDE/CubeMX.")

if (HALST_BUILD_TESTS)
    # CTest cannot be included before the first project() statement, but amp-embedded-infa-lib
    # needs to see that test utilities need to be built.
    set(BUILD_TESTING On)
endif()

if (HALST_STANDALONE)
    set(EMIL_INCLUDE_FREERTOS ${HALST_BUILD_EXAMPLES_FREERTOS})

    include(FetchContent)

    FetchContent_Declare(
        emil
        GIT_REPOSITORY https://github.com/philips-software/amp-embedded-infra-lib.git
        GIT_TAG        4494b45f8c157964346a60a45e76a83e48640e44 # unreleased
    )
    FetchContent_MakeAvailable(emil)

    if (EMIL_HOST_BUILD)
        FetchContent_Declare(
            cucumber-cpp-runner
            GIT_REPOSITORY https://github.com/philips-software/amp-cucumber-cpp-runner.git
            GIT_TAG        1c7fcde3dd140ce3f9c7327e66e2a73344625328 # unreleased
        )

        FetchContent_MakeAvailable(cucumber-cpp-runner)
    endif()
endif()

project(hal_st LANGUAGES C CXX ASM VERSION 4.0.0) # x-release-please-version

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

set_directory_properties(PROPERTY USE_FOLDERS ON)

# When building HalSt by itself do not exclude any targets from all
# Libraries will only be excluded from all when HalSt is a consumed as a dependency.
if (HALST_STANDALONE)
    set(HALST_EXCLUDE_FROM_ALL "")
else()
    set(HALST_EXCLUDE_FROM_ALL "EXCLUDE_FROM_ALL")
endif()

if (HALST_BUILD_TESTS)
    include(CTest)
    emil_enable_testing()
endif()

add_subdirectory(st)
add_subdirectory(hal_st)
add_subdirectory(hal_st_lwip)
if (HALST_STANDALONE)
    add_subdirectory(integration_test)
endif()
add_subdirectory(services)
add_subdirectory(examples)

emil_clangformat_directories(hal_st DIRECTORIES .)

if (HALST_STANDALONE)
    emil_folderize_all_targets()
endif()

if (HALST_STANDALONE AND NOT CMAKE_CROSSCOMPILING)
    function(generate_export_targets name)
        install(EXPORT halst${name}Targets
            FILE halst${name}Targets.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/halst
        )
    endfunction()

    foreach(target IN ITEMS IntegrationTest)
        generate_export_targets(${target})
    endforeach()

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}ConfigVersion.cmake"
        COMPATIBILITY SameMajorVersion
        # When cross-compiling for a 32-bit architecture and re-using host tooling from a 64-bit architecture
        # ARCH_INDEPENDENT is necessary here. See: https://cmake.org/cmake/help/latest/module/CMakePackageConfigHelpers.html.
        ARCH_INDEPENDENT
    )

    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )
endif()

set(CPACK_GENERATOR "ZIP;TGZ")
set(CPACK_SOURCE_IGNORE_FILES ".vs/;.git/;build/")
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "Koninklijke Philips N.V")
set(CPACK_PACKAGE_DESCRIPTION "${PROJECT_DESCRIPTION}")
set(CPACK_DEBIAN_PACKAGE_NAME "${CPACK_PACKAGE_NAME}")
set(CPACK_RPM_PACKAGE_NAME "${CPACK_PACKAGE_NAME}")
set(CPACK_PACKAGE_HOMEPAGE_URL "${PROJECT_HOMEPAGE_URL}")
set(CPACK_PACKAGE_MAINTAINER "${CPACK_PACKAGE_VENDOR}")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_VENDOR}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

include(CPack)
