#ifndef HAL_DIGITAL_TO_ANALOG_PIN_STM_HPP
#define HAL_DIGITAL_TO_ANALOG_PIN_STM_HPP

#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal/interfaces/DigitalToAnalogPin.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/util/Optional.hpp"
#include <cstdint>

#include DEVICE_HEADER

#ifdef HAS_PERIPHERAL_DAC

namespace hal
{
    class DacStm;

    class DigitalToAnalogPinImplStm
        : public DigitalToAnalogPinImplBase
    {
    public:
#if defined(DAC_CHIPCONNECT_BOTH)
        // clang-format off
        static constexpr struct Internal{} internal{};
        static constexpr struct External{} external{};
        static constexpr struct Both{} both{};

        // clang-format on

        enum struct Channel : uint32_t
        {
            channel1 = 1,
            channel2 = 2,
        };

        DigitalToAnalogPinImplStm(Channel channel, hal::DacStm& dac, Internal);
        DigitalToAnalogPinImplStm(hal::GpioPinStm& pin, hal::DacStm& dac, External);
        DigitalToAnalogPinImplStm(hal::GpioPinStm& pin, hal::DacStm& dac, Both);
#else
        DigitalToAnalogPinImplStm(hal::GpioPinStm& pin, hal::DacStm& dac);
#endif

        void Set(uint16_t value) override;

    private:
        void Configure();

        std::optional<AnalogPinStm> pin;
        DacStm& dac;
        uint32_t channel;

#if defined(DAC_CHIPCONNECT_BOTH)
        const bool internallyConnected;
#endif
    };

    class DacStm
    {
    public:
        DacStm(uint8_t oneBasedIndex);
        ~DacStm();

        void Set(uint32_t channel, uint32_t alignment, uint32_t data);

    private:
        friend class DigitalToAnalogPinImplStm;

        uint8_t index;
        DAC_HandleTypeDef handle{};
    };
}

#endif

#endif
