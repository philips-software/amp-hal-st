cmake_minimum_required(VERSION 3.24)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(HALST_STANDALONE On)
endif()

option(CMAKE_COMPILE_WARNING_AS_ERROR "Enable warnings-as-error" ON)
option(HALST_INCLUDE_DEFAULT_INIT "Include default initialization code; turn off when providing custom initialization" ON)
option(HALST_BUILD_EXAMPLES "Enable build of the examples" OFF)
option(HALST_BUILD_EXAMPLES_FREERTOS "Enable build of the FreeRTOS example" OFF)
set(HALST_XML_GPIO "" CACHE STRING "XML GPIO table")
set(HALST_XML_STM32 "" CACHE STRING "XML STM32 table")

if (HALST_STANDALONE)
    set(EMIL_INCLUDE_FREERTOS ${HALST_BUILD_EXAMPLES_FREERTOS})

    include(FetchContent)

    FetchContent_Declare(
        emil
        GIT_REPOSITORY https://github.com/philips-software/amp-embedded-infra-lib.git
        GIT_TAG        16dde0b5e8a10acae790e00e1cbb75e891f5925f # unreleased
    )

    FetchContent_MakeAvailable(emil)
endif()

project(hal_st LANGUAGES C CXX ASM VERSION 3.0.0) # x-release-please-version

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)

set_directory_properties(PROPERTY USE_FOLDERS ON)

# When building HalSt by itself do not exclude any targets from all
# Libraries will only be excluded from all when HalSt is a consumed as a dependency.
if (HALST_STANDALONE)
    set(HALST_EXCLUDE_FROM_ALL "")
else()
    set(HALST_EXCLUDE_FROM_ALL "EXCLUDE_FROM_ALL")
endif()

add_subdirectory(st)
add_subdirectory(hal_st)
add_subdirectory(hal_st_lwip)
add_subdirectory(integration_test)
add_subdirectory(services)
add_subdirectory(examples)

emil_clangformat_directories(hal_st DIRECTORIES .)

if (HALST_STANDALONE)
    emil_folderize_all_targets()
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
