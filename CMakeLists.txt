cmake_minimum_required(VERSION 3.24)

project(hal_st LANGUAGES C CXX VERSION 1.0.0)

option(CMAKE_COMPILE_WARNING_AS_ERROR "Enable warnings-as-error" ON)
option(HALST_INCLUDE_DEFAULT_INIT "Include default initialization code; turn off when providing custom initialization" ON)
option(HALST_INCLUDE_DEFAULT_LINKER_SCRIPTS "Include default memory-layout and sections linker scripts; turn off when providing custom linker scripts" ON)
option(HALST_BUILD_EXAMPLES "Enable build of the examples" OFF)

set_directory_properties(PROPERTY USE_FOLDERS ON)

if (HALST_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

add_subdirectory(hal_st)
add_subdirectory(hal_st_lwip)
