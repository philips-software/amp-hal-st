# amp-hal-st

[![Linting & Formatting](https://github.com/philips-software/amp-hal-st/actions/workflows/linting-formatting.yml/badge.svg)](https://github.com/philips-software/amp-hal-st/actions/workflows/linting-formatting.yml) [![OpenSSF Scorecard](https://api.securityscorecards.dev/projects/github.com/philips-software/amp-hal-st/badge)](https://api.securityscorecards.dev/projects/github.com/philips-software/amp-hal-st)

**Description**: amp-hal-st is a HAL (Hardware Abstraction Layer) implementation for a range of [ST](https://st.com) ARM Cortex-based micro-controllers. amp-hal-st implements the interfaces defined as part of [EmbeddedInfraLib].

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
    GIT_REPOSITORY https://github.com/philips-software/amp-hal-st.git
    GIT_TAG        main
)

FetchContent_MakeAvailable(emil halst)

add_executable(myprogram Main.cpp)

target_link_libraries(myprogram PUBLIC
    infra.event
    hal_st.stm32fxxx
)

halst_target_default_linker_scripts(myprogram)
halst_default_init(myprogram)

```

## How to test the software

Due to the nature of amp-hal-st: a HAL implementation, there are no included automated tests. Testing should be done in-context on the target hardware.

## Community

This project uses a [code-of-conduct](CODE_OF_CONDUCT.md) to define expected conduct in our community. Instances of abusive, harassing, or otherwise unacceptable behavior may be reported by contacting a project [maintainer](.github/CODEOWNERS)

## Changelog

See the [changelog](CHANGELOG.md) for more info on what's been changed.

## Contributing

Please refer to our [contributing](CONTRIBUTING.md) guide when you want to contribute to this project.

## License

amp-hal-st is licensed under the [MIT](https://choosealicense.com/licenses/mit/) [license](LICENSE) except the files and/or directories named in the [notice](NOTICE) file.

[EmbeddedInfraLib]: https://github.com/philips-software/embeddedinfralib
