#ifndef HAL_SDRAM_STM_HPP
#define HAL_SDRAM_STM_HPP

#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#ifdef STM32F4
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#elif STM32F7
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#endif

#if defined(HAS_PERIPHERAL_SDRAM)

namespace hal
{
    class SdRamStm
    {
    public:
        struct Config
        {
            uint32_t address;
            uint32_t size;
            uint32_t refreshCount;
            uint8_t bank;
            uint8_t columnBits;
            uint8_t rowBits;
            uint8_t busWidth;
            uint8_t casLatency;
        };

    public:
        SdRamStm(hal::MultiGpioPinStm& sdramPins, const Config& config);

        infra::ByteRange Memory() const;
        void SanityCheck();

    private:
        void Delay();
        SDRAM_HandleTypeDef CreateSdRamConfig(const Config& config) const;
        FMC_SDRAM_TimingTypeDef CreateTiming() const;
        void InitSdRam(SDRAM_HandleTypeDef& sdramHandle);
        void EnableClock(SDRAM_HandleTypeDef& sdram);
        void PreChargeAll(SDRAM_HandleTypeDef& sdram);
        void AutoRefresh(SDRAM_HandleTypeDef& sdram);
        void LoadMode(SDRAM_HandleTypeDef& sdram, const Config& config);
        void SetRefreshRate(SDRAM_HandleTypeDef& sdram, const Config& config);

    private:
        hal::MultiPeripheralPinStm sdramPins;
        infra::ByteRange memory;
        const uint32_t sdramTimeout = 0xFFFF;
    };

    static const SdRamStm::Config stm32f7discoverySdRamConfig = { 0xC0000000, 0x800000, 0x0603, 1, 8, 12, 16, 2 };
}

#endif

#endif
