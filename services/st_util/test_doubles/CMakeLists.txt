add_library(services.st_util_doubles INTERFACE)
emil_build_for(services.st_util_doubles BOOL BUILD_TESTING)

target_sources(services.st_util_doubles PRIVATE
    StBootloaderCommunicatorMock.hpp
)

target_link_libraries(services.st_util_doubles INTERFACE
    gmock
)
