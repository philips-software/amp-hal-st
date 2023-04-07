# CHANGELOG

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/) and this project adheres to [Semantic Versioning](https://semver.org/).

## [1.1.0](https://github.com/philips-software/amp-hal-st/compare/v1.0.0...v1.1.0) (2023-04-07)


### Features

* Add parity to UartStmDma ([#56](https://github.com/philips-software/amp-hal-st/issues/56)) ([e30d8d9](https://github.com/philips-software/amp-hal-st/commit/e30d8d9749641edc20c822f283ced3a59b071fb7))
* Add release-please workflow ([#65](https://github.com/philips-software/amp-hal-st/issues/65)) ([b39a848](https://github.com/philips-software/amp-hal-st/commit/b39a84822132e00b6d50dd059e16fa4f77e9394b))
* Add stm32f7discoveryFmcPins to SdRamStm ([#78](https://github.com/philips-software/amp-hal-st/issues/78)) ([72df16b](https://github.com/philips-software/amp-hal-st/commit/72df16b5f9142af8e16073af0a9857f95965dfb2))
* Apply clang-format ([#58](https://github.com/philips-software/amp-hal-st/issues/58)) ([714d922](https://github.com/philips-software/amp-hal-st/commit/714d922464168bf334b7616f620a02b93241e540))
* Deregister-interrupt-handler-on-move-assign ([#80](https://github.com/philips-software/amp-hal-st/issues/80)) ([095449d](https://github.com/philips-software/amp-hal-st/commit/095449da9208e5ea8118f3d506a2943284f9786c))
* Merge upstream changes ([#79](https://github.com/philips-software/amp-hal-st/issues/79)) ([8ec3afd](https://github.com/philips-software/amp-hal-st/commit/8ec3afd8bfbdd640d2d0db4875a6e66ab98e0eba))
* **osal:** Add example ([#82](https://github.com/philips-software/amp-hal-st/issues/82)) ([b1539ca](https://github.com/philips-software/amp-hal-st/commit/b1539cafb23ab99fe65f0fb403adbbad5f2debbe))


### Bug Fixes

* Avoid using generator expressions for linking to targets ([#92](https://github.com/philips-software/amp-hal-st/issues/92)) ([441abe5](https://github.com/philips-software/amp-hal-st/commit/441abe5c58c0891f55f744a358b89a823e195b4b))
* Correct dependencies of synchronous_stm32fxxx and ble_middleware ([#85](https://github.com/philips-software/amp-hal-st/issues/85)) ([2ffaf23](https://github.com/philips-software/amp-hal-st/commit/2ffaf237fb9b1cb5db01802687b100065ed72af8))
* **hal_st/stm32fxxx/UartStm & UartDma:** Registers for stm32f407 ([#59](https://github.com/philips-software/amp-hal-st/issues/59)) ([98ceaf3](https://github.com/philips-software/amp-hal-st/commit/98ceaf33b855cd1021dee622b4e15e4c97693845))
* HALST_EXCLUDE_FROM_ALL for instantiations_lwip ([#94](https://github.com/philips-software/amp-hal-st/issues/94)) ([f80f6ee](https://github.com/philips-software/amp-hal-st/commit/f80f6ee1d840534610f357777275030220bdd3bf))
* Link stm32f0xx to correct std_periph ([#93](https://github.com/philips-software/amp-hal-st/issues/93)) ([31cdcbb](https://github.com/philips-software/amp-hal-st/commit/31cdcbb16a0132daba2647762128cb8f5df8de16))
* Set correct mcpu mfpu options for different target cpus ([#75](https://github.com/philips-software/amp-hal-st/issues/75)) ([a799834](https://github.com/philips-software/amp-hal-st/commit/a799834fb8a167053297ba7a71e3f7beb5c048c1))
* **UartStm:** Use correct registers & fix overrun assert; enable FIFO ([#68](https://github.com/philips-software/amp-hal-st/issues/68)) ([60efc65](https://github.com/philips-software/amp-hal-st/commit/60efc6527afd5bd25b005bc5735ad4b399c9c21c))

## [Unreleased]

### Added
- Add examples; added blink example and build the examples as part of the continuous integration build.

### Changed
- Enabled warnings as errors by using CMAKE_COMPILE_WARNING_AS_ERROR; requires CMake 3.24.

## [v1.0.0] - 2022-Oct-10

- Initial release of amp-hal-st on GitHub.
