#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/BitLogic.hpp"
#include "infra/util/ReallyAssert.hpp"

namespace hal
{
    namespace
    {
        GPIO_TypeDef* const portToGPIOPort[] = {
            GPIOA,
            GPIOB,
            GPIOC,
            GPIOD,
            GPIOE,
#if !defined(STM32WB)
            GPIOF,
#endif
#if defined(STM32F2) || defined(STM32F4) || defined(STM32F7)
            GPIOG,
            GPIOH,
            GPIOI,
#endif
#if defined(STM32F7)
            GPIOJ,
            GPIOK
#endif
        };

        const uint16_t pinToGPIOPin[16] = {
            GPIO_PIN_0,
            GPIO_PIN_1,
            GPIO_PIN_2,
            GPIO_PIN_3,
            GPIO_PIN_4,
            GPIO_PIN_5,
            GPIO_PIN_6,
            GPIO_PIN_7,
            GPIO_PIN_8,
            GPIO_PIN_9,
            GPIO_PIN_10,
            GPIO_PIN_11,
            GPIO_PIN_12,
            GPIO_PIN_13,
            GPIO_PIN_14,
            GPIO_PIN_15,
        };

        const uint32_t weakPullToPuPd[3] = {
            GPIO_NOPULL,
            GPIO_PULLUP,
            GPIO_PULLDOWN
        };

        const uint32_t speedToSpeed[] = {
            GPIO_SPEED_LOW,
            GPIO_SPEED_MEDIUM,
#if defined(STM32F2) || defined(STM32F4) || defined(STM32F7)
            GPIO_SPEED_FAST,
#endif
            GPIO_SPEED_HIGH
        };

        const uint32_t driveToAFMode[2] = {
            GPIO_MODE_AF_PP,
            GPIO_MODE_AF_OD
        };
    }

    DummyPinStm dummyPinStm;

    GpioPinStm::GpioPinStm(Port port, uint8_t index, Drive drive, Speed speed, WeakPull weakPull)
        : port(port)
        , index(index)
        , drive(drive)
        , speed(speed)
        , weakPull(weakPull)
    {}

    bool GpioPinStm::Get() const
    {
        return infra::IsBitSet(portToGPIOPort[static_cast<uint8_t>(port)]->IDR, index);
    }

    void GpioPinStm::Set(bool value)
    {
        portToGPIOPort[static_cast<uint8_t>(port)]->BSRR = (1 << index) << (value ? 0 : 16);
        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->MODER, 2, 1, index);
    }

    bool GpioPinStm::GetOutputLatch() const
    {
        return infra::IsBitSet(portToGPIOPort[static_cast<uint8_t>(port)]->ODR, index);
    }

    void GpioPinStm::SetAsInput()
    {
        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->MODER, 2, 0, index);
    }

    bool GpioPinStm::IsInput() const
    {
        return infra::GetBits(portToGPIOPort[static_cast<uint8_t>(port)]->MODER, 2, index) == 0;
    }

    void GpioPinStm::Config(PinConfigType config)
    {
        assert(config != PinConfigType::output); // Use Config(_, startOutputState) for outputs

        GpioStm::Instance().ReservePin(port, index);

        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->PUPDR, 2, static_cast<uint8_t>(weakPull), index);
        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->OTYPER, 1, static_cast<uint16_t>(drive), index);
        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->OSPEEDR, 2, static_cast<uint8_t>(speed), index);

        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->AFR[index >> 3], 4, 0, index & 7);
        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->MODER, 2, 0, index);
    }

    void GpioPinStm::Config(PinConfigType config, bool startOutputState)
    {
        assert(config != PinConfigType::input);

        GpioStm::Instance().ReservePin(port, index);

        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->PUPDR, 2, static_cast<uint8_t>(weakPull), index);
        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->OTYPER, 1, static_cast<uint16_t>(drive), index);
        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->OSPEEDR, 2, static_cast<uint8_t>(speed), index);

        portToGPIOPort[static_cast<uint8_t>(port)]->BSRR = (1 << index) << (startOutputState ? 0 : 16);
        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->AFR[index >> 3], 4, 0, index & 7);
        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->MODER, 2, 1, index);
    }

    void GpioPinStm::ResetConfig()
    {
        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->PUPDR, 2, static_cast<uint8_t>(weakPull), index);
        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->OTYPER, 1, static_cast<uint16_t>(drive), index);
        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->OSPEEDR, 2, static_cast<uint8_t>(speed), index);

        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->AFR[index >> 3], 4, 0, index & 7);
        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->MODER, 2, 0, index);

        GpioStm::Instance().ClearPinReservation(port, index);
    }

    void GpioPinStm::EnableInterrupt(const infra::Function<void()>& action, InterruptTrigger trigger)
    {
        GpioStm::Instance().EnableInterrupt(port, index, action, trigger);
    }

    void GpioPinStm::DisableInterrupt()
    {
        GpioStm::Instance().DisableInterrupt(port, index);
    }

    void GpioPinStm::ConfigAnalog()
    {
        GpioStm::Instance().ReservePin(port, index);

        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->PUPDR, 2, static_cast<uint8_t>(weakPull), index);
        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->OTYPER, 1, static_cast<uint16_t>(drive), index);
        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->OSPEEDR, 2, static_cast<uint8_t>(speed), index);

        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->MODER, 2, 3, index);
    }

    void GpioPinStm::ConfigPeripheral(PinConfigTypeStm pinConfigType, uint8_t peripheral)
    {
        std::pair<const GpioStm::PinPosition&, const GpioStm::PinoutTable&> peripheralPinConfig = GpioStm::Instance().GetPeripheralPinConfig(port, index, pinConfigType, peripheral);

        GpioStm::Instance().ReservePin(port, index);

        uint8_t alternateFunction = peripheralPinConfig.first.alternateFunction;
        Drive drive = peripheralPinConfig.second.drive;
        Speed speed = peripheralPinConfig.second.speed;
        WeakPull weakPull = peripheralPinConfig.second.weakPull;

        // Peripherals use the weakpull, drive, and speed from the peripheral's definition
        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->PUPDR, 2, static_cast<uint8_t>(weakPull), index);
        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->OTYPER, 1, static_cast<uint16_t>(drive), index);
        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->OSPEEDR, 2, static_cast<uint8_t>(speed), index);

        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->AFR[index >> 3], 4, alternateFunction, index & 7);
        infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(port)]->MODER, 2, 2, index);
    }

    uint32_t GpioPinStm::AdcChannel(uint8_t adc) const
    {
        return GpioStm::Instance().AdcChannel(port, index, adc);
    }

    uint32_t GpioPinStm::DacChannel(uint8_t dac) const
    {
        return GpioStm::Instance().DacChannel(port, index, dac);
    }

    DummyPinStm::DummyPinStm()
        : GpioPinStm(Port::A, 0)
    {}

    bool DummyPinStm::Get() const
    {
        return false;
    }

    void DummyPinStm::Set(bool value)
    {}

    bool DummyPinStm::GetOutputLatch() const
    {
        return false;
    }

    void DummyPinStm::SetAsInput()
    {}

    bool DummyPinStm::IsInput() const
    {
        return false;
    }

    void DummyPinStm::Config(PinConfigType config)
    {}

    void DummyPinStm::Config(PinConfigType config, bool startOutputState)
    {}

    void DummyPinStm::ResetConfig()
    {}

    void DummyPinStm::EnableInterrupt(const infra::Function<void()>& action, InterruptTrigger trigger)
    {}

    void DummyPinStm::DisableInterrupt()
    {}

    void DummyPinStm::ConfigAnalog()
    {}

    void DummyPinStm::ConfigPeripheral(PinConfigTypeStm pinConfigType, uint8_t peripheral)
    {}

    PeripheralPinStm::PeripheralPinStm(GpioPinStm& pin, PinConfigTypeStm pinConfigType, uint8_t peripheral)
        : pin(pin)
    {
        pin.ConfigPeripheral(pinConfigType, peripheral);
    }

    PeripheralPinStm::~PeripheralPinStm()
    {
        pin.ResetConfig();
    }

    AnalogPinStm::AnalogPinStm(GpioPinStm& pin)
        : pin(pin)
    {
        pin.ConfigAnalog();
    }

    AnalogPinStm::~AnalogPinStm()
    {
        pin.ResetConfig();
    }

    uint32_t AnalogPinStm::AdcChannel(uint8_t adc) const
    {
        return pin.AdcChannel(adc);
    }

    uint32_t AnalogPinStm::DacChannel(uint8_t dac) const
    {
        return pin.DacChannel(dac);
    }

    MultiGpioPinStm::MultiGpioPinStm(infra::MemoryRange<const std::pair<Port, uint8_t>> table, Drive drive, Speed speed, WeakPull weakPull)
        : table(table)
        , drive(drive)
        , speed(speed)
        , weakPull(weakPull)
    {}

    void MultiGpioPinStm::ResetConfig()
    {
        for (const std::pair<Port, uint8_t>& portAndIndex : table)
        {
            infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(portAndIndex.first)]->PUPDR, 2, 0, portAndIndex.second);
            infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(portAndIndex.first)]->OTYPER, 1, 0, portAndIndex.second);
            infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(portAndIndex.first)]->OSPEEDR, 2, 0, portAndIndex.second);

            infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(portAndIndex.first)]->AFR[portAndIndex.second >> 3], 4, 0, portAndIndex.second & 7);
            infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(portAndIndex.first)]->MODER, 2, 0, portAndIndex.second);

            GpioStm::Instance().ClearPinReservation(portAndIndex.first, portAndIndex.second);
        }
    }

    void MultiGpioPinStm::ConfigAnalog()
    {
        for (const std::pair<Port, uint8_t>& portAndIndex : table)
        {
            GpioStm::Instance().ReservePin(portAndIndex.first, portAndIndex.second);

            infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(portAndIndex.first)]->PUPDR, 2, static_cast<uint8_t>(weakPull), portAndIndex.second);
            infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(portAndIndex.first)]->OTYPER, 1, static_cast<uint16_t>(drive), portAndIndex.second);
            infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(portAndIndex.first)]->OSPEEDR, 2, static_cast<uint8_t>(speed), portAndIndex.second);

            infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(portAndIndex.first)]->MODER, 2, 3, portAndIndex.second);
        }
    }

    void MultiGpioPinStm::ConfigPeripheral(PinConfigTypeStm pinConfigType, uint8_t peripheral)
    {
        for (const std::pair<Port, uint8_t>& portAndIndex : table)
        {
            std::pair<const GpioStm::PinPosition&, const GpioStm::PinoutTable&> peripheralPinConfig = GpioStm::Instance().GetPeripheralPinConfig(portAndIndex.first, portAndIndex.second, pinConfigType, peripheral);

            GpioStm::Instance().ReservePin(portAndIndex.first, portAndIndex.second);

            infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(portAndIndex.first)]->PUPDR, 2, static_cast<uint8_t>(peripheralPinConfig.second.weakPull), portAndIndex.second);
            infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(portAndIndex.first)]->OTYPER, 1, static_cast<uint16_t>(peripheralPinConfig.second.drive), portAndIndex.second);
            infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(portAndIndex.first)]->OSPEEDR, 2, static_cast<uint8_t>(peripheralPinConfig.second.speed), portAndIndex.second);

            infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(portAndIndex.first)]->AFR[portAndIndex.second >> 3], 4, peripheralPinConfig.first.alternateFunction, portAndIndex.second & 7);
            infra::ReplaceBits(portToGPIOPort[static_cast<uint8_t>(portAndIndex.first)]->MODER, 2, 2, portAndIndex.second);
        }
    }

    MultiPeripheralPinStm::MultiPeripheralPinStm(hal::MultiGpioPinStm& pins, PinConfigTypeStm pinConfigType, uint8_t peripheral)
        : pins(pins)
    {
        pins.ConfigPeripheral(pinConfigType, peripheral);
    }

    MultiPeripheralPinStm::~MultiPeripheralPinStm()
    {
        pins.ResetConfig();
    }

    GpioStm::GpioStm(infra::MemoryRange<const infra::MemoryRange<const GpioStm::PinoutTable>> pinoutTable, infra::MemoryRange<const GpioStm::AnalogPinPosition> analogTable)
        : pinoutTable(pinoutTable)
        , analogTable(analogTable)
        , assignedPins()
#if defined(STM32F0)
        , interruptDispatcher0_1(EXTI0_1_IRQn, [this]()
              { ExtiInterrupt(EXTI0_1_IRQn, 0, 2); })
        , interruptDispatcher2_3(EXTI2_3_IRQn, [this]()
              { ExtiInterrupt(EXTI2_3_IRQn, 2, 4); })
        , interruptDispatcher4_15(EXTI4_15_IRQn, [this]()
              { ExtiInterrupt(EXTI4_15_IRQn, 4, 16); })
#else
        , interruptDispatcher0(EXTI0_IRQn, [this]()
              { ExtiInterrupt(EXTI0_IRQn, 0, 1); })
        , interruptDispatcher1(EXTI1_IRQn, [this]()
              { ExtiInterrupt(EXTI1_IRQn, 1, 2); })
#if defined(STM32F3)
        , interruptDispatcher2(EXTI2_TSC_IRQn, [this]()
              { ExtiInterrupt(EXTI2_TSC_IRQn, 2, 3); })
#else
        , interruptDispatcher2(EXTI2_IRQn, [this]()
              { ExtiInterrupt(EXTI2_IRQn, 2, 3); })
#endif
        , interruptDispatcher3(EXTI3_IRQn, [this]()
              { ExtiInterrupt(EXTI3_IRQn, 3, 4); })
        , interruptDispatcher4(EXTI4_IRQn, [this]()
              { ExtiInterrupt(EXTI4_IRQn, 4, 5); })
        , interruptDispatcher9_5(EXTI9_5_IRQn, [this]()
              { ExtiInterrupt(EXTI9_5_IRQn, 5, 10); })
        , interruptDispatcher15_10(EXTI15_10_IRQn, [this]()
              { ExtiInterrupt(EXTI15_10_IRQn, 10, 16); })
#endif
    {
#if !defined(STM32WB)
        __SYSCFG_CLK_ENABLE();
#endif
        __GPIOA_CLK_ENABLE();
        __GPIOB_CLK_ENABLE();
        __GPIOC_CLK_ENABLE();
        __GPIOD_CLK_ENABLE();
        __GPIOE_CLK_ENABLE();
#if !defined(STM32WB)
        __GPIOF_CLK_ENABLE();
#endif
#if defined(STM32F2) || defined(STM32F4) || defined(STM32F7)
        __GPIOG_CLK_ENABLE();
        __GPIOH_CLK_ENABLE();
        __GPIOI_CLK_ENABLE();
#endif
#if defined(STM32F7)
        __GPIOJ_CLK_ENABLE();
        __GPIOK_CLK_ENABLE();
#endif
    }

    std::pair<const GpioStm::PinPosition&, const GpioStm::PinoutTable&> GpioStm::GetPeripheralPinConfig(Port port, uint8_t index, PinConfigTypeStm pinConfigType, uint8_t peripheral) const
    {
        for (infra::MemoryRange<const GpioStm::PinoutTable> subTable : pinoutTable)
            for (const PinoutTable& table : subTable)
                if (table.pinConfigType == pinConfigType)
                    for (const PinPosition& position : table.pinPositions)
                        if (position.port == port && position.pin == index && position.peripheralIndex == peripheral)
                            return std::pair<const GpioStm::PinPosition&, const GpioStm::PinoutTable&>(position, table);

        abort();
    }

    uint32_t GpioStm::AdcChannel(Port port, uint8_t pin, uint8_t instance) const
    {
        for (const GpioStm::AnalogPinPosition position : analogTable)
            if (position.type == Type::adc && position.instance == instance && position.port == port && position.pin == pin)
                return position.channel;

        abort();
    }

    uint32_t GpioStm::DacChannel(Port port, uint8_t pin, uint8_t instance) const
    {
        for (const GpioStm::AnalogPinPosition position : analogTable)
            if (position.type == Type::dac && position.instance == instance && position.port == port && position.pin == pin)
                return position.channel;

        abort();
    }

    void GpioStm::EnableInterrupt(Port port, uint8_t index, const infra::Function<void()>& action, InterruptTrigger trigger)
    {
        uint32_t extiMask = 0xf << ((index & 0x03) << 2);
        uint32_t extiValue = static_cast<uint8_t>(port) << ((index & 0x03) << 2);
        SYSCFG->EXTICR[index >> 2] = (SYSCFG->EXTICR[index >> 2] & ~extiMask) | extiValue;

#if defined(STM32WB) || defined(STM32G4)
        if (trigger != InterruptTrigger::fallingEdge)
            EXTI->RTSR1 |= 1 << index;
        else
            EXTI->RTSR1 &= ~(1 << index);

        if (trigger != InterruptTrigger::risingEdge)
            EXTI->FTSR1 |= 1 << index;
        else
            EXTI->FTSR1 &= ~(1 << index);

        EXTI->IMR1 |= 1 << index;
#else
        if (trigger != InterruptTrigger::fallingEdge)
            EXTI->RTSR |= 1 << index;
        else
            EXTI->RTSR &= ~(1 << index);

        if (trigger != InterruptTrigger::risingEdge)
            EXTI->FTSR |= 1 << index;
        else
            EXTI->FTSR &= ~(1 << index);

        EXTI->IMR |= 1 << index;
#endif
        really_assert(!handlers[index]);
        handlers[index] = action;
    }

    void GpioStm::DisableInterrupt(Port port, uint8_t index)
    {
#if defined(STM32WB) || defined(STM32G4)
        EXTI->IMR1 &= ~(1 << index);
#else
        EXTI->IMR &= ~(1 << index);
#endif
        handlers[index] = nullptr;
    }

    void GpioStm::ExtiInterrupt(IRQn_Type irq, std::size_t from, std::size_t to)
    {
        for (std::size_t line = from; line != to; ++line)
        {
#if defined(STM32WB) || defined(STM32G4)
            if (EXTI->PR1 & (1 << line))
            {
                EXTI->PR1 &= (1 << line); // Interrupt pending is cleared by writing a 1 to it
#else
            if (EXTI->PR & (1 << line))
            {
                EXTI->PR &= (1 << line); // Interrupt pending is cleared by writing a 1 to it
#endif
                if (handlers[line])
                    infra::EventDispatcher::Instance().Schedule(handlers[line]);
                NVIC_ClearPendingIRQ(irq);
            }
        }
    }

    void GpioStm::ReservePin(Port port, uint8_t index)
    {
        assert(static_cast<uint8_t>(port) < assignedPins.size());
        assert(index < 32 && index >= 0);
        assert((assignedPins[static_cast<uint8_t>(port)] & (1 << index)) == 0);
        assignedPins[static_cast<uint8_t>(port)] |= 1 << index;
    }

    void GpioStm::ClearPinReservation(Port port, uint8_t index)
    {
        assert(static_cast<uint8_t>(port) < assignedPins.size());
        assert(index < 32 && index >= 0);
        assignedPins[static_cast<uint8_t>(port)] &= ~(1 << index);
    }
}
