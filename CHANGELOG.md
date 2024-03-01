# CHANGELOG

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/) and this project adheres to [Semantic Versioning](https://semver.org/).

## [3.0.0](https://github.com/philips-software/amp-hal-st/compare/v2.1.0...v3.0.0) (2024-02-29)


### ⚠ BREAKING CHANGES

* update DmaStm to support new dma peripherals in G0, G4 and WB devices ([#169](https://github.com/philips-software/amp-hal-st/issues/169))

### Features

* Add configuration struct to init ([#262](https://github.com/philips-software/amp-hal-st/issues/262)) ([59d9331](https://github.com/philips-software/amp-hal-st/commit/59d9331329ec7260a18ad24b942e1a3874198f3f))
* Update DmaStm to support new dma peripherals in G0, G4 and WB devices ([#169](https://github.com/philips-software/amp-hal-st/issues/169)) ([d62e03e](https://github.com/philips-software/amp-hal-st/commit/d62e03e2e4eeef6c26dbf6e0f5620617642f443a))
* Update WB55 GAP constructor ([0c8e2e4](https://github.com/philips-software/amp-hal-st/commit/0c8e2e4a89b9cfd34c85f6e4e5ae456bff707c0b))


### Bug Fixes

* Add purl specifier for st middleware ([#228](https://github.com/philips-software/amp-hal-st/issues/228)) ([f3cc25c](https://github.com/philips-software/amp-hal-st/commit/f3cc25cffa1763b6ddff5c384ca15be9ed1cea87))
* Report number of bonds in GapSt ([#263](https://github.com/philips-software/amp-hal-st/issues/263)) ([85dceea](https://github.com/philips-software/amp-hal-st/commit/85dceeabe45f3e3dad178c2a81048874e46e2ce7))
* Zero initialize SynchronousSpiMasterStm::spiHandle ([#245](https://github.com/philips-software/amp-hal-st/issues/245)) ([581db4e](https://github.com/philips-software/amp-hal-st/commit/581db4e7545d343c7fc52eb6913941c737e73a48))

## [2.1.0](https://github.com/philips-software/amp-hal-st/compare/v2.0.0...v2.1.0) (2023-11-09)


### Features

* Add data operations to GattClient ([#132](https://github.com/philips-software/amp-hal-st/issues/132)) ([a1eaaf5](https://github.com/philips-software/amp-hal-st/commit/a1eaaf53fe8cc032da7aec5a3937f5643c9f6590))
* Add GATT Client (discovery attributes) ([#131](https://github.com/philips-software/amp-hal-st/issues/131)) ([121c351](https://github.com/philips-software/amp-hal-st/commit/121c35153e4e131fdab3cce5dd89723defe566e1))
* Add integration tests ([#208](https://github.com/philips-software/amp-hal-st/issues/208)) ([07086ec](https://github.com/philips-software/amp-hal-st/commit/07086eca82d487f4e434f5f086e442e127e0dde2))
* Add services/st_util/StUartProgammer ([#185](https://github.com/philips-software/amp-hal-st/issues/185)) ([8faceb4](https://github.com/philips-software/amp-hal-st/commit/8faceb49c1efbda055661a143ff10d830ccd0c11))
* Add stm32g0 hal drivers ([#118](https://github.com/philips-software/amp-hal-st/issues/118)) ([35dff48](https://github.com/philips-software/amp-hal-st/commit/35dff48cb238a4b0250f90895945205a0446446c))
* Ble central pairing bonding feature ([#163](https://github.com/philips-software/amp-hal-st/issues/163)) ([530b3bc](https://github.com/philips-software/amp-hal-st/commit/530b3bca48b3710dc2ffe4956e451a78968bf330))
* Configurable clock phase and polarity for sync spi ([#200](https://github.com/philips-software/amp-hal-st/issues/200)) ([cbf6d68](https://github.com/philips-software/amp-hal-st/commit/cbf6d688a5d1e2ca05444f8bc86d7cd5215a005b))
* Gap and gatt refactored/improved due to embedded-infra changes ([#136](https://github.com/philips-software/amp-hal-st/issues/136)) ([e6c276b](https://github.com/philips-software/amp-hal-st/commit/e6c276b80075bf53d46ade0da89834712fd28e5d))
* Improvements according to application note AN5289 ([#135](https://github.com/philips-software/amp-hal-st/issues/135)) ([bd95621](https://github.com/philips-software/amp-hal-st/commit/bd956214e617777edc51c248fbde589fa622c099))
* Low power event dispatcher ([#156](https://github.com/philips-software/amp-hal-st/issues/156)) ([a0d1855](https://github.com/philips-software/amp-hal-st/commit/a0d18558fa99fc55f9386d9331856ab4eda730cc))
* Merge upstream changes ([#126](https://github.com/philips-software/amp-hal-st/issues/126)) ([1546f51](https://github.com/philips-software/amp-hal-st/commit/1546f51e706884270073884bd53ec1fb197afe0a))
* Refactor and expand StUartProgrammer ([#202](https://github.com/philips-software/amp-hal-st/issues/202)) ([5c8c213](https://github.com/philips-software/amp-hal-st/commit/5c8c213eb05d506d77d0d86ca862c7974a62fa3f))
* Removed additional EXCLUDE_FROM_ALL property in favour of using emil_build_for() ([8e3766d](https://github.com/philips-software/amp-hal-st/commit/8e3766d8829d0a53ef28d150e0f71bfd8236deb5))
* Update st ble stack ([#165](https://github.com/philips-software/amp-hal-st/issues/165)) ([a6b8827](https://github.com/philips-software/amp-hal-st/commit/a6b8827ad55416a59817b8627785c98626d7aa37))
* Update the st ble stack to version 1.17 ([a6b8827](https://github.com/philips-software/amp-hal-st/commit/a6b8827ad55416a59817b8627785c98626d7aa37))


### Bug Fixes

* Adc on stm32wb ([#162](https://github.com/philips-software/amp-hal-st/issues/162)) ([ee16f2a](https://github.com/philips-software/amp-hal-st/commit/ee16f2a8bb31fedb475b92ab6529bf8b0132a40e))
* Add ble middleware binaries ([#130](https://github.com/philips-software/amp-hal-st/issues/130)) ([201687e](https://github.com/philips-software/amp-hal-st/commit/201687ebdac32493630b76115ed068fd2261e863))
* Add confirmation for gatt indication ([#159](https://github.com/philips-software/amp-hal-st/issues/159)) ([02a98f0](https://github.com/philips-software/amp-hal-st/commit/02a98f071ab9ad8a2663737e8703fa63e02f95c2))
* Communication over ethernet smi ([#129](https://github.com/philips-software/amp-hal-st/issues/129)) ([9935da5](https://github.com/philips-software/amp-hal-st/commit/9935da5cd5ad88318c0bf0ce8825669bd645892d))
* Do not assert if GattCompleteEvent fails ([#167](https://github.com/philips-software/amp-hal-st/issues/167)) ([063daa4](https://github.com/philips-software/amp-hal-st/commit/063daa465045744438e0087dcc384fdaf035fa05))
* Duplicate and misdirected function calls due to listening for events on multiple levels in the inheritance tree of classes using GapSt. ([6f8b7f7](https://github.com/philips-software/amp-hal-st/commit/6f8b7f7f7b2144c79e996fde904aee319b385210))
* Event handling of GapSt and its derived classes. ([#143](https://github.com/philips-software/amp-hal-st/issues/143)) ([6f8b7f7](https://github.com/philips-software/amp-hal-st/commit/6f8b7f7f7b2144c79e996fde904aee319b385210))
* Hal_st/stm32fxxx/EthernetSmiStm: ETH_MACMIIAR_CR_Msk is the mask of MACMII-&gt;AR, not the inverse ([9935da5](https://github.com/philips-software/amp-hal-st/commit/9935da5cd5ad88318c0bf0ce8825669bd645892d))
* Incorrect connection handle in state transitions ([#147](https://github.com/philips-software/amp-hal-st/issues/147)) ([2f99452](https://github.com/philips-software/amp-hal-st/commit/2f994526d59aff59fa80896122762b3a47c347e5))
* Set privacy mode for bonded devices ([#146](https://github.com/philips-software/amp-hal-st/issues/146)) ([88d30f2](https://github.com/philips-software/amp-hal-st/commit/88d30f288cf6c6cb47c10aec0003cabdc0f9d7e8))
* Standalone stm32 build and debug ([#193](https://github.com/philips-software/amp-hal-st/issues/193)) ([71bf177](https://github.com/philips-software/amp-hal-st/commit/71bf177b5f992f4bb9793470cd70136fb60cebcd))
* Systick conflict with invalidIrq ([#161](https://github.com/philips-software/amp-hal-st/issues/161)) ([8ee04e1](https://github.com/philips-software/amp-hal-st/commit/8ee04e17e1c159c2d7e1e5057194dd0603e929ca))

## [2.0.0](https://github.com/philips-software/amp-hal-st/compare/v1.1.0...v2.0.0) (2023-04-14)


### ⚠ BREAKING CHANGES

* update ST hal drivers ([#102](https://github.com/philips-software/amp-hal-st/issues/102))

### Features

* Update ST hal drivers ([#102](https://github.com/philips-software/amp-hal-st/issues/102)) ([6987d83](https://github.com/philips-software/amp-hal-st/commit/6987d83f4e9a78a5fc91b0e93d6788645cb6a49f))

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
