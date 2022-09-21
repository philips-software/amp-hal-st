#include "hal_st/stm32fxxx/SdRamStm.hpp"
#include <cassert>

#if defined(HAS_PERIPHERAL_SDRAM)

namespace hal
{
    SdRamStm::SdRamStm(hal::MultiGpioPinStm& sdramPins, const Config& config)
        : sdramPins(sdramPins, hal::PinConfigTypeStm::fmc, 0)
        , memory(reinterpret_cast<uint8_t*>(config.address), reinterpret_cast<uint8_t*>(config.address + config.size))
    {
        __HAL_RCC_FMC_CLK_ENABLE();

        SDRAM_HandleTypeDef sdramHandle = CreateSdRamConfig(config);
        InitSdRam(sdramHandle);
        EnableClock(sdramHandle);
        PreChargeAll(sdramHandle);
        AutoRefresh(sdramHandle);
        LoadMode(sdramHandle, config);
        SetRefreshRate(sdramHandle, config);
    }

    infra::ByteRange SdRamStm::Memory() const
    {
        return memory;
    }

    void SdRamStm::SanityCheck()
    {
        const_cast<volatile uint8_t&>(memory[0x50]) = 0x45;
        assert(const_cast<volatile uint8_t&>(memory[0x50]) == 0x45);
    }

    void SdRamStm::Delay()
    {
        volatile uint32_t timeout = sdramTimeout * 0xF;
        while (timeout--)
        {}
    }

    SDRAM_HandleTypeDef SdRamStm::CreateSdRamConfig(const Config& config) const
    {
        static const uint32_t columnBits[] = { 0, 0, 0, 0, 0, 0, 0, 0, FMC_SDRAM_COLUMN_BITS_NUM_8, FMC_SDRAM_COLUMN_BITS_NUM_9, FMC_SDRAM_COLUMN_BITS_NUM_10, FMC_SDRAM_COLUMN_BITS_NUM_11 };
        static const uint32_t rowBits[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, FMC_SDRAM_ROW_BITS_NUM_11, FMC_SDRAM_ROW_BITS_NUM_12, FMC_SDRAM_ROW_BITS_NUM_13 };
        static const uint32_t casLatency[] = { 0, FMC_SDRAM_CAS_LATENCY_1, FMC_SDRAM_CAS_LATENCY_2, FMC_SDRAM_CAS_LATENCY_3 };

        SDRAM_HandleTypeDef result = {};

        result.Instance = FMC_SDRAM_DEVICE;
        result.Init.SDBank = config.bank == 1 ? FMC_SDRAM_BANK1 : FMC_SDRAM_BANK2;
        result.Init.ColumnBitsNumber = columnBits[config.columnBits];
        result.Init.RowBitsNumber = rowBits[config.rowBits];
        result.Init.MemoryDataWidth = config.busWidth == 8 ? FMC_SDRAM_MEM_BUS_WIDTH_8 : config.busWidth == 16 ? FMC_SDRAM_MEM_BUS_WIDTH_16 : FMC_SDRAM_MEM_BUS_WIDTH_32;
        result.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
        result.Init.CASLatency = casLatency[config.casLatency];
        result.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
        result.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
        result.Init.ReadBurst = FMC_SDRAM_RBURST_DISABLE;
        result.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;

        return result;
    }

    FMC_SDRAM_TimingTypeDef SdRamStm::CreateTiming() const
    {
        FMC_SDRAM_TimingTypeDef result = {};

        // timing configuration for 90 Mhz of SD clock frequency (180Mhz/2)
        // TMRD: 2 Clock cycles
        // 1 clock cycle = 1 / 90MHz = 11.1ns
        result.LoadToActiveDelay = 2;
        result.ExitSelfRefreshDelay = 7;
        result.SelfRefreshTime = 4;
        result.RowCycleDelay = 7;
        result.WriteRecoveryTime = 2;
        result.RPDelay = 2;
        result.RCDDelay = 2;

        return result;
    }

    void SdRamStm::InitSdRam(SDRAM_HandleTypeDef& sdramHandle)
    {
        FMC_SDRAM_TimingTypeDef timing = CreateTiming();
        HAL_SDRAM_Init(&sdramHandle, &timing);
    }

    void SdRamStm::EnableClock(SDRAM_HandleTypeDef& sdramHandle)
    {
        FMC_SDRAM_CommandTypeDef command = {};
        command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
        command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
        command.AutoRefreshNumber = 1;
        command.ModeRegisterDefinition = 0;

        HAL_SDRAM_SendCommand(&sdramHandle, &command, sdramTimeout);
        Delay();
    }

    void SdRamStm::PreChargeAll(SDRAM_HandleTypeDef& sdramHandle)
    {
        FMC_SDRAM_CommandTypeDef command = {};
        command.CommandMode = FMC_SDRAM_CMD_PALL;
        command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
        command.AutoRefreshNumber = 1;
        command.ModeRegisterDefinition = 0;

        HAL_SDRAM_SendCommand(&sdramHandle, &command, sdramTimeout);
    }

    void SdRamStm::AutoRefresh(SDRAM_HandleTypeDef& sdramHandle)
    {
        FMC_SDRAM_CommandTypeDef command = {};
        command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
        command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
        command.AutoRefreshNumber = 8;
        command.ModeRegisterDefinition = 0;

        HAL_SDRAM_SendCommand(&sdramHandle, &command, sdramTimeout);
    }

    void SdRamStm::LoadMode(SDRAM_HandleTypeDef& sdramHandle, const Config& config)
    {
        FMC_SDRAM_CommandTypeDef command = {};
        command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
        command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
        command.AutoRefreshNumber = 1;
        command.ModeRegisterDefinition = 0x0200 | (config.casLatency << 4);

        HAL_SDRAM_SendCommand(&sdramHandle, &command, sdramTimeout);
    }

    void SdRamStm::SetRefreshRate(SDRAM_HandleTypeDef& sdramHandle, const Config& config)
    {
        HAL_SDRAM_ProgramRefreshRate(&sdramHandle, config.refreshCount);
        Delay();
    }
}

#endif
