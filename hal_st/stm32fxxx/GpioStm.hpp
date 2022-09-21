#ifndef HAL_GPIO_STM_HPP
#define HAL_GPIO_STM_HPP

#include "hal/interfaces/Gpio.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "infra/util/MemoryRange.hpp"
#include "cmsis_device.h"
#include <cstdint>

namespace hal
{
    enum class PinConfigTypeStm
    {
        i2cScl,
        i2cSda,
        uartTx,
        uartRx,
        uartRts,
        uartCts,
        canTx,
        canRx,
        spiClock,
        spiMiso,
        spiMosi,
        spiSlaveSelect,
        saiSck,
        saiMClock,
        saiSd,
        saiFs,
        quadSpiClock,
        quadSpiSlaveSelect,
        quadSpiData0,
        quadSpiData1,
        quadSpiData2,
        quadSpiData3,
        sdClk,
        sdCmd,
        sdD0,
        sdD1,
        sdD2,
        sdD3,
        ethernet,
        usbFsId,
        usbFsDm,
        usbFsDp,
        fmc,
        lcd,
        timerChannel1,
        timerChannel2,
        timerChannel3,
        timerChannel4
    };

    enum class Type : uint8_t
    {
        adc,
        dac
    };

    enum class Port: uint8_t
    {
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K
    };

    enum class WeakPull: uint8_t
    {
        None,
        Up,
        Down,
        Default = None
    };

    enum class Drive: uint8_t
    {
        PushPull,
        OpenDrain,
        Default = PushPull
    };

    enum class Speed: uint8_t
    {
        Low,
        Medium,
        Fast,
        High,
        Default = Low
    };

    class GpioPinStm
        : public GpioPin
    {
    public:
        GpioPinStm(Port port, uint8_t index, Drive drive = Drive::Default, Speed speed = Speed::Default, WeakPull weakPull = WeakPull::Default);

        virtual bool Get() const override;
        virtual void Set(bool value) override;
        virtual bool GetOutputLatch() const override;
        virtual void SetAsInput() override;
        virtual bool IsInput() const override;
        virtual void Config(PinConfigType config) override;
        virtual void Config(PinConfigType config, bool startOutputState) override;
        virtual void ResetConfig() override;
        virtual void EnableInterrupt(const infra::Function<void()>& action, InterruptTrigger trigger) override;
        virtual void DisableInterrupt() override;

        virtual void ConfigAnalog();
        virtual void ConfigPeripheral(PinConfigTypeStm pinConfigType, uint8_t peripheral);

        uint32_t AdcChannel(uint8_t adc) const;
        uint32_t DacChannel(uint8_t dac) const;

    private:
        Port port;
        uint8_t index;
        Drive drive;
        Speed speed;
        WeakPull weakPull;
    };

    class DummyPinStm
        : public GpioPinStm
    {
    public:
        DummyPinStm();

        virtual bool Get() const override;
        virtual void Set(bool value) override;
        virtual bool GetOutputLatch() const override;
        virtual void SetAsInput() override;
        virtual bool IsInput() const override;
        virtual void Config(PinConfigType config) override;
        virtual void Config(PinConfigType config, bool startOutputState) override;
        virtual void ResetConfig() override;
        virtual void EnableInterrupt(const infra::Function<void()>& action, InterruptTrigger trigger) override;
        virtual void DisableInterrupt() override;
        virtual void ConfigAnalog() override;
        virtual void ConfigPeripheral(PinConfigTypeStm pinConfigType, uint8_t peripheral) override;
    };

    extern DummyPinStm dummyPinStm;

    class PeripheralPinStm
    {
    public:
        PeripheralPinStm(GpioPinStm& pin, PinConfigTypeStm pinConfigType, uint8_t peripheral);
        PeripheralPinStm(const PeripheralPinStm& other) = delete;
        PeripheralPinStm& operator=(const PeripheralPinStm& other) = delete;
        ~PeripheralPinStm();

    private:
        GpioPinStm& pin;
    };

    class AnalogPinStm
    {
    public:
        AnalogPinStm(GpioPinStm& pin);
        AnalogPinStm(const AnalogPinStm& other) = delete;
        AnalogPinStm& operator=(const AnalogPinStm& other) = delete;
        ~AnalogPinStm();

        uint32_t AdcChannel(uint8_t adc) const;
        uint32_t DacChannel(uint8_t dac) const;

    private:
        GpioPinStm& pin;
    };

    class MultiGpioPinStm
    {
    public:
        MultiGpioPinStm(infra::MemoryRange<const std::pair<Port, uint8_t>> table, Drive drive = Drive::Default, Speed speed = Speed::Default, WeakPull weakPull = WeakPull::Default);

        void ResetConfig();
        void ConfigAnalog();
        void ConfigPeripheral(PinConfigTypeStm pinConfigType, uint8_t peripheral);

    private:
        infra::MemoryRange<const std::pair<Port, uint8_t>> table;
        Drive drive;
        Speed speed;
        WeakPull weakPull;
    };

    class MultiPeripheralPinStm
    {
    public:
        MultiPeripheralPinStm(hal::MultiGpioPinStm& pins, PinConfigTypeStm pinConfigType, uint8_t peripheral);
        MultiPeripheralPinStm(const MultiPeripheralPinStm& other) = delete;
        MultiPeripheralPinStm& operator=(const MultiPeripheralPinStm& other) = delete;
        ~MultiPeripheralPinStm();

    private:
        hal::MultiGpioPinStm& pins;
    };

    class GpioStm
        : public infra::InterfaceConnector<GpioStm>
    {
    public:
        struct PinPosition
        {
            uint8_t peripheralIndex;
            Port port;
            uint8_t pin;
            uint8_t alternateFunction;
        };

        struct AnalogPinPosition
        {
            uint8_t instance;
            Type type;
            Port port;
            uint8_t pin;
            uint8_t channel;
        };

        struct PinoutTable
        {
            PinConfigTypeStm pinConfigType;
            infra::MemoryRange<const PinPosition> pinPositions;
            Drive drive;
            Speed speed;
            WeakPull weakPull;
        };

    public:
        GpioStm(infra::MemoryRange<const infra::MemoryRange<const GpioStm::PinoutTable>> pinoutTable, infra::MemoryRange<const GpioStm::AnalogPinPosition> analogTable = infra::MemoryRange<const GpioStm::AnalogPinPosition>());

    public:
        std::pair<const PinPosition&, const PinoutTable&> GetPeripheralPinConfig(Port port, uint8_t index, PinConfigTypeStm pinConfigType, uint8_t peripheral) const;
        uint32_t AdcChannel(Port port, uint8_t index, uint8_t adc) const;
        uint32_t DacChannel(Port port, uint8_t index, uint8_t dac) const;

        void EnableInterrupt(Port port, uint8_t index, const infra::Function<void()>& action, InterruptTrigger trigger);
        void DisableInterrupt(Port port, uint8_t index);

        void ReservePin(Port port, uint8_t index);
        void ClearPinReservation(Port port, uint8_t index);

    private:
        void ExtiInterrupt(IRQn_Type irq, std::size_t from, std::size_t to);

        infra::MemoryRange<const infra::MemoryRange<const GpioStm::PinoutTable>> pinoutTable;
        infra::MemoryRange<const GpioStm::AnalogPinPosition> analogTable;

        std::array<infra::Function<void()>, 16> handlers;
        std::array<uint32_t, 11> assignedPins;

#if defined(STM32F0)
        DispatchedInterruptHandler interruptDispatcher0_1;
        DispatchedInterruptHandler interruptDispatcher2_3;
        DispatchedInterruptHandler interruptDispatcher4_15;
#else
        DispatchedInterruptHandler interruptDispatcher0;
        DispatchedInterruptHandler interruptDispatcher1;
        DispatchedInterruptHandler interruptDispatcher2;
        DispatchedInterruptHandler interruptDispatcher3;
        DispatchedInterruptHandler interruptDispatcher4;
        DispatchedInterruptHandler interruptDispatcher9_5;
        DispatchedInterruptHandler interruptDispatcher15_10;
#endif
    };
}

#endif
