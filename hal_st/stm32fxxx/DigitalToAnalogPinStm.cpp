#include "hal_st/stm32fxxx/DigitalToAnalogPinStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/util/BitLogic.hpp"
#include "infra/util/EnumCast.hpp"
#include "infra/util/Optional.hpp"
#include <algorithm>
#include <cassert>
#include <cstdint>

#include DEVICE_HEADER

#ifdef HAS_PERIPHERAL_DAC

namespace hal
{
#if defined(DAC_CHIPCONNECT_BOTH)
    DigitalToAnalogPinImplStm::DigitalToAnalogPinImplStm(Channel channel, hal::DacStm& dac, Internal)
        : dac{ dac }
        , channel{ infra::enum_cast(channel) == 1 ? DAC_CHANNEL_1 : DAC_CHANNEL_2 }
        , internallyConnected{ true }
    {
        Configure();
    }

    DigitalToAnalogPinImplStm::DigitalToAnalogPinImplStm(hal::GpioPinStm& pin, hal::DacStm& dac, External)
        : pin{ std::in_place, pin }
        , dac{ dac }
        , channel{ pin.DacChannel(dac.index + 1) == 1 ? DAC_CHANNEL_1 : DAC_CHANNEL_2 }
        , internallyConnected{ false }
    {
        Configure();
    }

    DigitalToAnalogPinImplStm::DigitalToAnalogPinImplStm(hal::GpioPinStm& pin, hal::DacStm& dac, Both)
        : pin{ std::in_place, pin }
        , dac{ dac }
        , channel{ pin.DacChannel(dac.index + 1) == 1 ? DAC_CHANNEL_1 : DAC_CHANNEL_2 }
        , internallyConnected{ true }

    {
        Configure();
    }
#else
    DigitalToAnalogPinImplStm::DigitalToAnalogPinImplStm(hal::GpioPinStm& pin, hal::DacStm& dac)
        : pin{ std::in_place, pin }
        , dac(dac)
        , channel{ pin.DacChannel(dac.index + 1) == 1 ? DAC_CHANNEL_1 : DAC_CHANNEL_2 }
    {
        Configure();
    }
#endif

    void DigitalToAnalogPinImplStm::Set(uint16_t value)
    {
        dac.Set(channel, DAC_ALIGN_12B_R, std::min(static_cast<uint32_t>(value), (infra::Bit<uint32_t>(12) - 1)));
    }

    void DigitalToAnalogPinImplStm::Configure()
    {
        DAC_ChannelConfTypeDef sConfig{ 0 };
        sConfig.DAC_OutputBuffer = pin ? DAC_OUTPUTBUFFER_ENABLE : DAC_OUTPUTBUFFER_DISABLE;

#if defined(DAC_HIGH_FREQUENCY_INTERFACE_MODE_AUTOMATIC)
        sConfig.DAC_HighFrequency = DAC_HIGH_FREQUENCY_INTERFACE_MODE_AUTOMATIC;
#endif

#if defined(DAC_CHIPCONNECT_BOTH)
        if (pin)
            sConfig.DAC_ConnectOnChipPeripheral = internallyConnected ? DAC_CHIPCONNECT_BOTH : DAC_CHIPCONNECT_EXTERNAL;
        else
            sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_INTERNAL;
#endif
        HAL_StatusTypeDef result = HAL_DAC_ConfigChannel(&dac.handle, &sConfig, channel);
        assert(result == HAL_OK);

        result = HAL_DAC_Start(&dac.handle, channel);
        assert(result == HAL_OK);
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
