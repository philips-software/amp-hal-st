#include "hal_st/stm32fxxx/DigitalToAnalogPinStm.hpp"
#include "stm32g4xx_hal_dac.h"
#include DEVICE_HEADER
#include "infra/util/BitLogic.hpp"

#ifdef HAS_PERIPHERAL_DAC

namespace hal
{
    DigitalToAnalogPinImplStm::DigitalToAnalogPinImplStm(hal::GpioPinStm& pin, hal::DacStm& dac)
        : pin{ pin }
        , dac{ dac }
        , pinChannel{ pin.DacChannel(dac.index + 1) == 1 ? DAC_CHANNEL_1 : DAC_CHANNEL_2 }
    {
        DAC_ChannelConfTypeDef sConfig;
        sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
        sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

#if defined(STM32G4)
        sConfig.DAC_Trigger2 = DAC_TRIGGER_NONE;
        sConfig.DAC_HighFrequency = DAC_HIGH_FREQUENCY_INTERFACE_MODE_AUTOMATIC;
        sConfig.DAC_DMADoubleDataMode = DISABLE;
        sConfig.DAC_SignedFormat = DISABLE;
        sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
        sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_BOTH; // TODO add parameter
        sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
#endif

        HAL_StatusTypeDef result = HAL_DAC_ConfigChannel(&dac.handle, &sConfig, pinChannel);
        assert(result == HAL_OK);

        result = HAL_DAC_Start(&dac.handle, pinChannel);
        assert(result == HAL_OK);
    }

    void DigitalToAnalogPinImplStm::Set(uint16_t value)
    {

        dac.Set(pinChannel, DAC_ALIGN_12B_R, std::min(static_cast<uint32_t>(value), (infra::Bit<uint32_t>(12) - 1)));
    }

    DacStm::DacStm(uint8_t oneBasedIndex)
        : index(oneBasedIndex - 1)
    {
        EnableClockDac(index);

        handle.Instance = peripheralDac[index];
        HAL_StatusTypeDef result = HAL_DAC_Init(&handle);
        assert(result == HAL_OK);
    }

    DacStm::~DacStm()
    {
        DisableClockDac(index);
    }

    void DacStm::Set(uint32_t channel, uint32_t alignment, uint32_t data)
    {
        HAL_StatusTypeDef result = HAL_DAC_SetValue(&handle, channel, alignment, data);
        assert(result == HAL_OK);
    }
}

#endif
