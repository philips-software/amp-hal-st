if (EMIL_HOST_BUILD)
    FetchContent_Declare(
        pugixml
        GIT_REPOSITORY https://github.com/zeux/pugixml
        GIT_TAG a0e064336317c9347a91224112af9933598714e9 # v1.13
    )

    FetchContent_MakeAvailable(pugixml)
endif()

add_library(infra.xml ${HALST_EXCLUDE_FROM_ALL} STATIC)

target_include_directories(infra.xml PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../>"
)

target_link_libraries(infra.xml PUBLIC
    infra.stream
    pugixml
)

if (EMIL_HOST_BUILD)
    target_sources(infra.xml PRIVATE
        XmlNavigator.cpp
        XmlNavigator.hpp
    )
endif()

add_subdirectory(test)
