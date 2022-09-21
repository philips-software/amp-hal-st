# amp-hal-st

[![Linting & Formatting](https://github.com/philips-internal/amp-hal-st/actions/workflows/linting-formatting.yml/badge.svg)](https://github.com/philips-internal/amp-hal-st/actions/workflows/linting-formatting.yml)

**Description**: amp-hal-st is a HAL (Hardware Abstraction Layer) implementation for a range of [ST](https://st.com) ARM Cortex-based microcontrollers. amp-hal-st implements the interfaces defined as part of [EmbeddedInfraLib].

## Dependencies

amp-hal-st requires:
- [EmbeddedInfraLib].

## How to build the software

amp-hal-st cannot be built by-itself, it must be built as part of a larger project. This paragraph describes how to add amp-hal-st to a CMake build-system, using [EmbeddedInfraLib].

> CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.21)

project(MyProject VERSION 1.0.0)

include(FetchContent)

FetchContent_Declare(
    emil
    GIT_REPOSITORY https://github.com/philips-software/embeddedinfralib.git
    GIT_TAG        main
)

FetchContent_Declare(
    halst
    GIT_REPOSITORY https://github.com/philips-internal/amp-hal-st.git
    GIT_TAG        main
)

FetchContent_MakeAvailable(emil halst)

add_executable(myprogram Main.cpp)

target_link_libraries(myprogram PUBLIC
    infra.event
    hal_st.stm32fxxx
)
```

## How to test the software

Due to the nature of amp-hal-st: a HAL implementation, there are no included automated tests. Testing should be done in-context on the target hardware.

## Community

This project uses the [CODE_OF_CONDUCT](CODE_OF_CONDUCT.md) to define expected conduct in our community. Instances of abusive, harassing, or otherwise unacceptable behavior may be reported by contacting a project [CODEOWNER](CODEOWNERS)

## Changelog

See [CHANGELOG](CHANGELOG.md) for more info on what's been changed.

## Contributing

Please refer to our [CONTRIBUTING](CONTRIBUTING.md) guide when you want to contribute to this project.

## License

This HalSt Project is in general licensed under the following MIT license except the files named underneath (see corresponding notice files below)

amp-hal-st is licensed under the [MIT](https://choosealicense.com/licenses/mit/) [LICENSE](LICENSE) except the files and/or directories named in the [NOTICE](NOTICE) file.

[EmbeddedInfraLib]: https://github.com/philips-software/embeddedinfralib
