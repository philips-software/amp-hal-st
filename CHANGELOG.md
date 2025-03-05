# CHANGELOG

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/) and this project adheres to [Semantic Versioning](https://semver.org/).

## [4.0.0](https://github.com/philips-software/amp-hal-st/compare/v3.0.0...v4.0.0) (2025-01-16)


### ⚠ BREAKING CHANGES

* add BLE transport layer for stm32wbaxx ([#356](https://github.com/philips-software/amp-hal-st/issues/356))
* refactor ble middleware ([#346](https://github.com/philips-software/amp-hal-st/issues/346))

### Features

* AdcDmaMultiChannelStm implements new AdcMultiChannel interface ([#462](https://github.com/philips-software/amp-hal-st/issues/462)) ([bdd2507](https://github.com/philips-software/amp-hal-st/commit/bdd2507d3e10875a6e394748d85419e0d64665c5))
* Add ADC and timer for mcu stm32wbaxx ([#328](https://github.com/philips-software/amp-hal-st/issues/328)) ([d413549](https://github.com/philips-software/amp-hal-st/commit/d413549c64e6d24e62a5b23ef02999324a939fdc))
* Add ADC triggered by timer using DMA ([#238](https://github.com/philips-software/amp-hal-st/issues/238)) ([31adf4e](https://github.com/philips-software/amp-hal-st/commit/31adf4ea988ce87f796520c9e3145720244577bc))
* Add BLE transport layer for stm32wbaxx ([#356](https://github.com/philips-software/amp-hal-st/issues/356)) ([f2803c8](https://github.com/philips-software/amp-hal-st/commit/f2803c809d12f2944fb4e0655f5fd41f8216e57e))
* Add calibration for adc when not single differential ([#444](https://github.com/philips-software/amp-hal-st/issues/444)) ([66d25bc](https://github.com/philips-software/amp-hal-st/commit/66d25bc834f3ba2c878f70b4885dacdbb2ca1519))
* Add channel config to stm internal temperature sensor ([#291](https://github.com/philips-software/amp-hal-st/issues/291)) ([401024a](https://github.com/philips-software/amp-hal-st/commit/401024afe3d124036f87cc7e8f2e16d39af4b406))
* Add clangd configuration file ([#351](https://github.com/philips-software/amp-hal-st/issues/351)) ([e86f662](https://github.com/philips-software/amp-hal-st/commit/e86f662237dd687d85467c9a286e1d35c4112f39))
* Add Cucumber integration tests ([#221](https://github.com/philips-software/amp-hal-st/issues/221)) ([75ece18](https://github.com/philips-software/amp-hal-st/commit/75ece182ee8ef45a2d2412e70444f51ec9ff7ba8))
* Add DMA circular mode for mcu stm32wbaxx ([#327](https://github.com/philips-software/amp-hal-st/issues/327)) ([3d46e93](https://github.com/philips-software/amp-hal-st/commit/3d46e931bf27b93a40d6fcf91a6b7935317c3f5f))
* Add hw semaphore sync for rng on stm32wb ([#286](https://github.com/philips-software/amp-hal-st/issues/286)) ([3e4a41d](https://github.com/philips-software/amp-hal-st/commit/3e4a41d3698df6541b0d45497205444c79507251))
* Add Immediate and Dispatched interrupt handling to DmaStm's StreamInterruptHandler via tag dispatching ([#336](https://github.com/philips-software/amp-hal-st/issues/336)) ([736df80](https://github.com/philips-software/amp-hal-st/commit/736df8088f9f9a3916f6af2872b209612e106ff2))
* Add internal flash for mcu stm32wba  ([#348](https://github.com/philips-software/amp-hal-st/issues/348)) ([8629391](https://github.com/philips-software/amp-hal-st/commit/8629391b6fe1159eea8a65a982ded02fcf95e00a))
* Add Multi Channel ADC ([#394](https://github.com/philips-software/amp-hal-st/issues/394)) ([19f2f6b](https://github.com/philips-software/amp-hal-st/commit/19f2f6b312f10918d6e39dc84191b15b124ebb8d))
* Add new family STM32WBA to hal_st ([#314](https://github.com/philips-software/amp-hal-st/issues/314)) ([112eedb](https://github.com/philips-software/amp-hal-st/commit/112eedbebb48186059a00b6464dfc8861f2af375))
* Add new STM32H5 family to hal-st ([#365](https://github.com/philips-software/amp-hal-st/issues/365)) ([ad0cb38](https://github.com/philips-software/amp-hal-st/commit/ad0cb3825151be796253fb3f244d7a237ff38f36))
* Add SPI master for mcu stm32wba ([#343](https://github.com/philips-software/amp-hal-st/issues/343)) ([8115ac1](https://github.com/philips-software/amp-hal-st/commit/8115ac1e7ad530f24a6e9e3610975ce36a0084f4))
* Add ST files for mcu stm32wba52cx ([#315](https://github.com/philips-software/amp-hal-st/issues/315)) ([f545781](https://github.com/philips-software/amp-hal-st/commit/f545781e76bfcdbd247a6d21904f57d6cd7ce273))
* Add support for ADC channel 19 ([#389](https://github.com/philips-software/amp-hal-st/issues/389)) ([114b832](https://github.com/philips-software/amp-hal-st/commit/114b832c9fcb15fe3c8511ed227bb634e2fc66f3))
* Add support for STM32G474VxT ([#372](https://github.com/philips-software/amp-hal-st/issues/372)) ([fe9ee42](https://github.com/philips-software/amp-hal-st/commit/fe9ee426f18c8dc7df9fb04f73c5abf0e5fd3e18))
* Add support for STM32G4xx DigitalToAnalog peripheral ([#375](https://github.com/philips-software/amp-hal-st/issues/375)) ([4f409d0](https://github.com/philips-software/amp-hal-st/commit/4f409d024de94ec7c7534caa1a6f421e39915ae1))
* Add uart pin swap configuration option when supported ([#296](https://github.com/philips-software/amp-hal-st/issues/296)) ([0826e3b](https://github.com/philips-software/amp-hal-st/commit/0826e3b580d3e28f89d389a2bd6743cd8f3c2cea))
* Allow disable NonMaskableInt_IRQn interrupt ([#457](https://github.com/philips-software/amp-hal-st/issues/457)) ([1529067](https://github.com/philips-software/amp-hal-st/commit/1529067b6c1ec7ec40d9fec15f8fb58c9f031b4f))
* Allow more access to SystemTickTimerService ([#393](https://github.com/philips-software/amp-hal-st/issues/393)) ([ecfb4ce](https://github.com/philips-software/amp-hal-st/commit/ecfb4ce214fcc9b41664ecd17d1086205628e926))
* DigitalToAnalogPinImplStm improvements ([#471](https://github.com/philips-software/amp-hal-st/issues/471)) ([cc2855d](https://github.com/philips-software/amp-hal-st/commit/cc2855d44e90aa2318f5e04298a5697d4a389cab))
* Enable internal flash support for STM32H5 series ([#437](https://github.com/philips-software/amp-hal-st/issues/437)) ([97f3005](https://github.com/philips-software/amp-hal-st/commit/97f300519191ab6e8743814243a5b853bc22489d))
* Extend GattClient interface with enable/disable indication/notification ([#447](https://github.com/philips-software/amp-hal-st/issues/447)) ([62be185](https://github.com/philips-software/amp-hal-st/commit/62be185f0789b622112a588b5731c57aa175f9a6))
* Extend gpio interrupt interface to enable immediate interrupt handlers ([#416](https://github.com/philips-software/amp-hal-st/issues/416)) ([d95f21a](https://github.com/philips-software/amp-hal-st/commit/d95f21a206b92e8a2537c2e3ea15d147874d635b))
* Extend I2C for stm32wba family ([#324](https://github.com/philips-software/amp-hal-st/issues/324)) ([6030210](https://github.com/philips-software/amp-hal-st/commit/60302104eb46d6f51ad4229ccd56b1fa8550d678))
* Improve SPI slave support for STM32H5 series ([#390](https://github.com/philips-software/amp-hal-st/issues/390)) ([d17039e](https://github.com/philips-software/amp-hal-st/commit/d17039e548c12f33dd5ba9616ffda18ff21372f6))
* Include UartStmDuplexDma in STM32H5 series build ([#400](https://github.com/philips-software/amp-hal-st/issues/400)) ([0e9533b](https://github.com/philips-software/amp-hal-st/commit/0e9533b134ddefa476856a1d209cd1af9e4feb9d))
* Lpuart with dma ([#340](https://github.com/philips-software/amp-hal-st/issues/340)) ([6577c8e](https://github.com/philips-software/amp-hal-st/commit/6577c8ef73694852becc597e6f4f006685df3218))
* Make hal::Port's contents depend on actual available ports ([#350](https://github.com/philips-software/amp-hal-st/issues/350)) ([da30d1d](https://github.com/philips-software/amp-hal-st/commit/da30d1db334ad9c55b04e8a2c199251c3460b776))
* Move starting the DAC channel to the constructor to avoid a 1ms delay loop every time the Dac value is set ([#387](https://github.com/philips-software/amp-hal-st/issues/387)) ([8211b8c](https://github.com/philips-software/amp-hal-st/commit/8211b8c82ad23b4fa81854b0e7ee4e63c68fc5f4))
* Prevent the CMSIS drivers from providing __cmsis_start function ([#383](https://github.com/philips-software/amp-hal-st/issues/383)) ([5f578c5](https://github.com/philips-software/amp-hal-st/commit/5f578c5d96693277ee31f372e8a69d6d23515b0f))
* Refactor ble middleware ([#346](https://github.com/philips-software/amp-hal-st/issues/346)) ([6703195](https://github.com/philips-software/amp-hal-st/commit/67031950dbfa4b07fc0000baaa2abb6b51e3cb5e))
* Remove HAL_GetTick friendship from SystemTickTimerService ([#359](https://github.com/philips-software/amp-hal-st/issues/359)) ([10e1bd1](https://github.com/philips-software/amp-hal-st/commit/10e1bd1a829ce449d576da54bb1611b8492ed798))
* Removed incorrect using directives for Circular DmaChannels ([#337](https://github.com/philips-software/amp-hal-st/issues/337)) ([c98ba11](https://github.com/philips-software/amp-hal-st/commit/c98ba11b8dbe603b955a50c1375f3156f917afc9))
* Stm32 wpan for stm32wbaxx ([#321](https://github.com/philips-software/amp-hal-st/issues/321)) ([da44299](https://github.com/philips-software/amp-hal-st/commit/da44299e5c1354578329ce6d6a01bd31a1a348b6))
* **stm32g4xx:** Add irq lookup for AnalogToDigitalPinStm based on peripheral number ([#398](https://github.com/philips-software/amp-hal-st/issues/398)) ([7569d4a](https://github.com/philips-software/amp-hal-st/commit/7569d4ae082a8ee057e29ed998c05c4e86ee6958))
* Support for internal high cycle area flash ([#449](https://github.com/philips-software/amp-hal-st/issues/449)) ([2705f02](https://github.com/philips-software/amp-hal-st/commit/2705f02a19882bb9b2a3336ec7084f7b0b9c11e9))
* Update to latest main of amp-cucumber-cpp-runner ([#478](https://github.com/philips-software/amp-hal-st/issues/478)) ([a32ea44](https://github.com/philips-software/amp-hal-st/commit/a32ea4416b7e2131838734e1ec42e075c32183f4))


### Bug Fixes

* Add firmwarebranch info to wireless version info ([#426](https://github.com/philips-software/amp-hal-st/issues/426)) ([0334001](https://github.com/philips-software/amp-hal-st/commit/0334001e081b2f715fa9762435218549270ae901))
* Add missing cmake dependency ([#459](https://github.com/philips-software/amp-hal-st/issues/459)) ([f11d3bf](https://github.com/philips-software/amp-hal-st/commit/f11d3bf7f7071c70fba00f18d0bcd5de65fb0987))
* Ble central requires a timeout when tries to start a connection ([#420](https://github.com/philips-software/amp-hal-st/issues/420)) ([bb209a3](https://github.com/philips-software/amp-hal-st/commit/bb209a3ddb1324463b65509d278c6ed59c077511))
* Circular mode for wba family ([#384](https://github.com/philips-software/amp-hal-st/issues/384)) ([617ab7b](https://github.com/philips-software/amp-hal-st/commit/617ab7bea4b0166f45a90328b20aa7dc2a972bc9))
* Cleanup uartstm destructors ([#441](https://github.com/philips-software/amp-hal-st/issues/441)) ([5042b8d](https://github.com/philips-software/amp-hal-st/commit/5042b8df1fac90b103c2d42a8201c51f8a44cab1))
* Command sequence for flash spi with dma ([#370](https://github.com/philips-software/amp-hal-st/issues/370)) ([edbd1e3](https://github.com/philips-software/amp-hal-st/commit/edbd1e3f4422db4728bf334c1230e31f43543dec))
* Don't disable dma interrupts when stopping a transfer ([#335](https://github.com/philips-software/amp-hal-st/issues/335)) ([bca8097](https://github.com/philips-software/amp-hal-st/commit/bca809731c61eafbc31fd8d7b7b70d2c879d3c97))
* Dont enable UART receiver before actually calling ReceiveData ([#446](https://github.com/philips-software/amp-hal-st/issues/446)) ([1d0b6a2](https://github.com/philips-software/amp-hal-st/commit/1d0b6a2e0968b920563d7f8cf8606caaf3c2b354))
* GAP connection procedure ([#445](https://github.com/philips-software/amp-hal-st/issues/445)) ([e11c17e](https://github.com/philips-software/amp-hal-st/commit/e11c17e681b137e4c71031851dc1cd1d9ef798f0))
* Gpio clock not enabled ([#382](https://github.com/philips-software/amp-hal-st/issues/382)) ([e99f4ab](https://github.com/philips-software/amp-hal-st/commit/e99f4ab91dce810dd97542830ae5b20963a8a31f))
* Handling GPDMA multi byte transfer ([#349](https://github.com/philips-software/amp-hal-st/issues/349)) ([50010c6](https://github.com/philips-software/amp-hal-st/commit/50010c62a3d7a02a8e483d11ebc9db15dbdb919a))
* LpUart peripheral table was not used correctly with UartStmDma ([#430](https://github.com/philips-software/amp-hal-st/issues/430)) ([e65c694](https://github.com/philips-software/amp-hal-st/commit/e65c694e36fb8d6fea4d6e68700b7dc5543cbfd5))
* Prescaler for ADC ([#347](https://github.com/philips-software/amp-hal-st/issues/347)) ([0d083aa](https://github.com/philips-software/amp-hal-st/commit/0d083aabf876fb24839bd2622267695240a92207))
* SPI struct sequence data addition ([#352](https://github.com/philips-software/amp-hal-st/issues/352)) ([a1c28bb](https://github.com/philips-software/amp-hal-st/commit/a1c28bb625a31409c62f43096a0bcae95ce1f656))
* Sync uart and flash for wba ([#453](https://github.com/philips-software/amp-hal-st/issues/453)) ([feb8b53](https://github.com/philips-software/amp-hal-st/commit/feb8b534a449b559c1b8482e505006d612411992))
* Use mcu database to filter non-existing pin ([#305](https://github.com/philips-software/amp-hal-st/issues/305)) ([c704496](https://github.com/philips-software/amp-hal-st/commit/c704496a109f2a5edeb5d35520f29242af3965f9))
* When invoking ReceiveData(nullptr), UartStmDuplexDma now stops reception ([#344](https://github.com/philips-software/amp-hal-st/issues/344)) ([df738e9](https://github.com/philips-software/amp-hal-st/commit/df738e975f334831c5fb521f9e66294ecfd46da7))

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
