#ifndef HAL_ST_TIMER_PWM_ST_HPP
#define HAL_ST_TIMER_PWM_ST_HPP

#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/TimerStm.hpp"
#include "infra/util/MemoryRange.hpp"
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

        PwmChannelGpio& Channel(uint8_t channelOneBasedIndex);

        void Start();
        void Stop();

    private:
        FreeRunningTimerStm timer;

    protected:
        infra::MemoryRange<PwmChannelGpio> channels;

        auto& Handle()
        {
            return timer.handle;
        }
    };

    template<std::size_t Channels>
    class TimerPwmWithChannels
        : public TimerPwmBaseStm
    {
    public:
        TimerPwmWithChannels(uint8_t oneBasedIndex, TimerBaseStm::Timing timing, infra::MemoryRange<GpioPinStm> pins)
            : TimerPwmBaseStm(oneBasedIndex, timing)
            , channelStorage(CreateArray<Channels, PwmChannelGpio>(oneBasedIndex, Handle(), pins))
        {
            channels = channelStorage;
        }

    private:
        std::array<PwmChannelGpio, Channels> channelStorage;
    };

    class PwmChannelGpio
    {
    public:
        PwmChannelGpio(uint8_t timerOneBasedIndex, uint8_t channelOneBasedIndex, TIM_HandleTypeDef& handle, GpioPinStm& pin);

        void Set(uint8_t dutyPercent);
        void Start();
        void Stop();

    private:
        uint8_t timerIndex;
        uint8_t channelIndex;
        TIM_HandleTypeDef& handle;
        PeripheralPinStm pin;
        volatile std::uint32_t* ccr;
    };
}

#endif

#endif // HAL_ST_TIMER_PWM_ST_HPP
