#ifndef SYNCHRONOUS_HAL_SYNCHRONOUS_GPIO_STM_HPP
#define SYNCHRONOUS_HAL_SYNCHRONOUS_GPIO_STM_HPP

#include DEVICE_HEADER
#include "hal/synchronous_interfaces/SynchronousGpio.hpp"
#include <array>
#include <cstdlib>

namespace hal
{
    enum class Port : uint8_t
    {
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I
    };

    enum class WeakPull : uint8_t
    {
        None,
        Up,
        Down,
        Default = None
    };

    enum class Drive : uint8_t
    {
        PushPull,
        OpenDrain,
        Default = PushPull
    };

    enum class Speed : uint8_t
    {
        Low,
        Medium,
        Fast,
        High,
        Default = Low
    };

    class SynchronousOutputPinStm
        : public SynchronousOutputPin
    {
    public:
        SynchronousOutputPinStm(Port port, uint8_t index, Drive drive = Drive::Default, Speed speed = Speed::Default);
        ~SynchronousOutputPinStm();

        void Set(bool value) override;
        bool GetOutputLatch() const override;

    private:
        Port port;
        uint8_t index;
    };

    class SmallPeripheralPinStm
    {
    public:
        template<std::size_t N>
        struct Definition
        {
            struct AssignPositions
            {
                uint8_t peripheralIndex;
                Port port;
                uint8_t pin;
                uint8_t alternateFunction;
            };

            const std::array<AssignPositions, N> positions;

            Drive drive;
            Speed speed;
            WeakPull weakPull;
        };

        template<std::size_t N>
        SmallPeripheralPinStm(const Definition<N>& definition, uint8_t peripheral, Port port, uint8_t index);
        SmallPeripheralPinStm(const Definition<1>& definition);
        SmallPeripheralPinStm(const SmallPeripheralPinStm& other) = delete;
        SmallPeripheralPinStm& operator=(const SmallPeripheralPinStm& other) = delete;
        ~SmallPeripheralPinStm();

    private:
        void ConfigPeripheralPin(uint8_t af, Drive drive, Speed speed, WeakPull weakPull);

    private:
        Port port;
        uint8_t index;
    };

    ////    Implementation    ////

    template<std::size_t N>
    SmallPeripheralPinStm::SmallPeripheralPinStm(const Definition<N>& definition, uint8_t peripheral, Port port, uint8_t index)
        : port(port)
        , index(index)
    {
        for (auto& position : definition.positions)
        {
            if (position.peripheralIndex == peripheral && position.port == port && position.pin == index)
            {
                ConfigPeripheralPin(position.alternateFunction, definition.drive, definition.speed, definition.weakPull);
                return;
            }
        }

        // This function cannot be assigned at the requested port and index.
        std::abort();
    }
}

#endif
