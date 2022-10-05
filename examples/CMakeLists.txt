include(FetchContent)

FetchContent_Declare(
    emil
    GIT_REPOSITORY https://github.com/philips-software/embeddedinfralib.git
    GIT_TAG        modern-cmake
)

FetchContent_MakeAvailable(emil)

add_subdirectory(blink)
