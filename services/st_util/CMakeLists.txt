add_library(services.st_util STATIC)

target_include_directories(services.st_util PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_sources(services.st_util PRIVATE
    FlashOnStBootloaderCommunicator.cpp
    FlashOnStBootloaderCommunicator.hpp
    StBootloaderCommunicator.hpp
    StBootloaderCommunicatorUart.cpp
    StBootloaderCommunicatorUart.hpp
)

target_link_libraries(services.st_util PUBLIC
    hal.interfaces
    services.util
)

if (EMIL_BUILD_TESTS)
    add_subdirectory(test_doubles)
    add_subdirectory(test)
endif()
