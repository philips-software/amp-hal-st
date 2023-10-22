add_executable(services.st_util_test)
emil_add_test(services.st_util_test)

target_sources(services.st_util_test PRIVATE
    TestStUartBootloaderCommandHandler.cpp
)

target_link_libraries(services.st_util_test PUBLIC
    gmock_main
    services.st_util
    hal.interfaces_test_doubles
    infra.util_test_helper
    infra.timer_test_helper
)
