#ifndef HAL_ST_TIMER_PWM_ST_HPP
#define HAL_ST_TIMER_PWM_ST_HPP

#include "hal/interfaces/PulseWidthModulation.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/TimerStm.hpp"
#include "infra/util/BoundedDeque.hpp"
#include "infra/util/MemoryRange.hpp"
#include "infra/util/WithStorage.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <utility>

#if defined(HAS_PERIPHERAL_TIMER)

namespace hal
{
    namespace detail
    {
        template<typename T, std::size_t... Is>
        constexpr std::array<T, sizeof...(Is)> CreateArray(uint8_t timerOneBasedIndex, TIM_HandleTypeDef& handle, infra::MemoryRange<GpioPinStm> pins, std::index_sequence<Is...>)
        {
            return { { (Is, T{ timerOneBasedIndex, Is + 1, handle, pins[Is] })... } };
        }
    }

    template<std::size_t N, typename T>
    constexpr std::array<T, N> CreateArray(uint8_t timerOneBasedIndex, TIM_HandleTypeDef& handle, infra::MemoryRange<GpioPinStm> pins)
    {
        return detail::CreateArray<T>(timerOneBasedIndex, handle, pins, std::make_index_sequence<N>());
    }

    class PwmChannelGpio;

    class TimerPwmBaseStm
    {
    public:
        TimerPwmBaseStm(uint8_t oneBasedIndex, TimerBaseStm::Timing timing);

        virtual PwmChannelGpio& Channel(uint8_t channelOneBasedIndex);

        void Start();
        void Stop();

    protected:
        FreeRunningTimerStm timer;
        infra::MemoryRange<PwmChannelGpio> channels;
    };

    template<std::size_t Channels>
    class TimerPwmWithAscendingChannels
        : public TimerPwmBaseStm
    {
    public:
        TimerPwmWithAscendingChannels(uint8_t oneBasedIndex, TimerBaseStm::Timing timing, infra::MemoryRange<GpioPinStm> pins)
            : TimerPwmBaseStm(oneBasedIndex, timing)
            , channelStorage(CreateArray<Channels, PwmChannelGpio>(oneBasedIndex, timer.Handle(), pins))
        {
            channels = channelStorage;
        }

    private:
        std::array<PwmChannelGpio, Channels> channelStorage;
    };

    class TimerPwmLazy
        : public TimerPwmBaseStm
    {
    public:
        template<std::size_t Channels>
        using WithChannels = infra::WithStorage<TimerPwmLazy, infra::BoundedDeque<PwmChannelGpio>::WithMaxSize<Channels>>;

        TimerPwmLazy(infra::BoundedDeque<PwmChannelGpio>& channelStorage, uint8_t oneBasedIndex, TimerBaseStm::Timing timing);

        void ConfigureChannel(uint8_t channelOneBasedIndex, GpioPinStm& pin);
        PwmChannelGpio& Channel(uint8_t channelOneBasedIndex) override;

    private:
        infra::BoundedDeque<PwmChannelGpio>& channelStorage;
        uint8_t oneBasedIndex;
    };

    class PwmChannelGpio
        : public PulseWidthModulation
    {
    public:
        PwmChannelGpio(uint8_t timerOneBasedIndex, uint8_t channelOneBasedIndex, TIM_HandleTypeDef& handle, GpioPinStm& pin);

        void SetDuty(uint8_t dutyPercent) override;
        void SetPulse(uint32_t pulseOn, uint32_t period) override;
        void Start() override;
        void Stop() override;

    private:
        friend class TimerPwmLazy;

        uint8_t timerIndex;
        uint8_t channelIndex;
        TIM_HandleTypeDef& handle;
        PeripheralPinStm pin;
        volatile std::uint32_t* ccr;
    };
}

#endif

#endif // HAL_ST_TIMER_PWM_ST_HPP
