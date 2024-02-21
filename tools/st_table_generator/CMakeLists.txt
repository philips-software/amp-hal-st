if (NOT CMAKE_CROSSCOMPILING)
    add_executable(tools.st_table_generator)
    install(TARGETS tools.st_table_generator EXPORT halstTargets DESTINATION bin)

    target_sources(tools.st_table_generator PRIVATE Main.cpp)

    target_link_libraries(tools.st_table_generator PRIVATE
        args
        hal.generic
        infra.syntax
    )
endif()
