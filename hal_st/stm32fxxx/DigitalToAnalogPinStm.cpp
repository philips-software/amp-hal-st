#include "hal_st/stm32fxxx/DigitalToAnalogPinStm.hpp"
#include DEVICE_HEADER
#include "infra/util/BitLogic.hpp"

#ifdef HAS_PERIPHERAL_DAC

namespace hal
{
    DigitalToAnalogPinImplStm::DigitalToAnalogPinImplStm(hal::GpioPinStm& pin, hal::DacStm& dac)
        : pin(pin)
        , dac(dac)
    {
        channel = pin.DacChannel(dac.index + 1);
    }

    void DigitalToAnalogPinImplStm::Set(uint16_t value)
    {
        uint32_t pinChannel = channel == 1 ? DAC_CHANNEL_1 : DAC_CHANNEL_2;

        DAC_ChannelConfTypeDef sConfig;
        sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
        sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
        HAL_StatusTypeDef result = HAL_DAC_ConfigChannel(&dac.handle, &sConfig, pinChannel);
        assert(result == HAL_OK);

        result = HAL_DAC_Start(&dac.handle, pinChannel);
        assert(result == HAL_OK);

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
