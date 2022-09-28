cmake_minimum_required(VERSION 3.20)

project(hal_st LANGUAGES C CXX VERSION 1.0.0)

option(HALST_INCLUDE_DEFAULT_INIT "Include default initialization code; turn off when providing custom initialization" ON)
option(HALST_INCLUDE_DEFAULT_LINKER_SCRIPTS "Include default memory-layout and sections linker scripts; turn off when providing custom linker scripts" ON)

list(APPEND CMAKE_MODULE_PATH "${EMIL_SOURCE_DIR}/external/xslt")

set_directory_properties(PROPERTY USE_FOLDERS ON)

add_subdirectory(hal_st)
add_subdirectory(hal_st_lwip)
