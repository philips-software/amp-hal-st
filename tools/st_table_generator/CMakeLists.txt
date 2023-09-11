add_executable(tools.st_table_generator)
emil_build_for(tools.st_table_generator HOST All)

target_sources(tools.st_table_generator PRIVATE Main.cpp)

target_link_libraries(tools.st_table_generator PRIVATE
    args
    hal.generic
    infra.syntax
    infra.xml
)
