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

    inline constexpr SdRamStm::Config stm32f7discoverySdRamConfig = { 0xC0000000, 0x800000, 0x0603, 1, 8, 12, 16, 2 };
    inline constexpr std::array<std::pair<hal::Port, uint8_t>, 39> stm32f7discoveryFmcPins = { { { hal::Port::C, 3 }, { hal::Port::D, 0 }, { hal::Port::D, 1 }, { hal::Port::D, 3 },
        { hal::Port::D, 8 }, { hal::Port::D, 9 }, { hal::Port::D, 10 }, { hal::Port::D, 14 },
        { hal::Port::D, 15 }, { hal::Port::E, 0 }, { hal::Port::E, 1 }, { hal::Port::E, 7 },
        { hal::Port::E, 8 }, { hal::Port::E, 9 }, { hal::Port::E, 10 }, { hal::Port::E, 11 },
        { hal::Port::E, 12 }, { hal::Port::E, 13 }, { hal::Port::E, 14 }, { hal::Port::E, 15 },
        { hal::Port::F, 0 }, { hal::Port::F, 1 }, { hal::Port::F, 2 }, { hal::Port::F, 3 },
        { hal::Port::F, 4 }, { hal::Port::F, 5 }, { hal::Port::F, 11 }, { hal::Port::F, 12 },
        { hal::Port::F, 13 }, { hal::Port::F, 14 }, { hal::Port::F, 15 }, { hal::Port::G, 0 },
        { hal::Port::G, 1 }, { hal::Port::G, 4 }, { hal::Port::G, 5 }, { hal::Port::G, 8 },
        { hal::Port::G, 15 }, { hal::Port::H, 3 }, { hal::Port::H, 5 } } };
}

#endif

#endif
