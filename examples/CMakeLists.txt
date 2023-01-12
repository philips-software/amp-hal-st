include(FetchContent)

if (HALST_STANDALONE)
    FetchContent_Declare(
        emil
        GIT_REPOSITORY https://github.com/philips-software/embeddedinfralib.git
        GIT_TAG        modern-cmake
    )

    FetchContent_MakeAvailable(emil)
endif()

add_subdirectory(blink)
