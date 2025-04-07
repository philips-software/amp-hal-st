#ifndef HAL_ST_LPTIMER_PWM_ST_HPP
#define HAL_ST_LPTIMER_PWM_ST_HPP

#include "hal/interfaces/PulseWidthModulation.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/LpTimerStm.hpp"
#include "infra/util/MemoryRange.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <utility>

#if defined(HAS_PERIPHERAL_LPTIMER)

namespace hal
{

    namespace detail
    {
        template<typename T, std::size_t... Is>
        constexpr std::array<T, sizeof...(Is)> CreateArray(uint8_t timerOneBasedIndex, LPTIM_HandleTypeDef& handle, infra::MemoryRange<GpioPinStm> pins, std::index_sequence<Is...>)
        {
            return {{(Is, T{timerOneBasedIndex, Is + 1, handle, pins[Is]})...}};
        }
    }

    template<std::size_t N, typename T>
    constexpr std::array<T, N> CreateArray(uint8_t timerOneBasedIndex, LPTIM_HandleTypeDef& handle, infra::MemoryRange<GpioPinStm> pins)
    {
        return detail::CreateArray<T>(timerOneBasedIndex, handle, pins, std::make_index_sequence<N>());
    }

    class LpPwmChannelGpio;

    class LpTimerPwmBaseStm
    {
    public:
        LpTimerPwmBaseStm(uint8_t oneBasedIndex, LowPowerTimerBaseStm::Timing timing);

        LpPwmChannelGpio& Channel(uint8_t channelOneBasedIndex);

        void Start();
        void Stop();

    protected:
        FreeRunningLowPowerTimerStm timer;
        infra::MemoryRange<LpPwmChannelGpio> channels;
    };

    template<std::size_t Channels>
    class LpTimerPwmWithChannels
        : public LpTimerPwmBaseStm
    {
    public:
        LpTimerPwmWithChannels(uint8_t oneBasedIndex, LowPowerTimerBaseStm::Timing timing, infra::MemoryRange<GpioPinStm> pins)
            : LpTimerPwmBaseStm(oneBasedIndex, timing)
            , channelStorage(CreateArray<Channels, LpPwmChannelGpio>(oneBasedIndex, timer.Handle(), pins))
        {
            channels = channelStorage;
        }

    private:
        std::array<LpPwmChannelGpio, Channels> channelStorage;
    };

    class LpPwmChannelGpio
        : public PulseWidthModulation
    {
    public:
        LpPwmChannelGpio(uint8_t timerOneBasedIndex, uint8_t channelOneBasedIndex, LPTIM_HandleTypeDef& handle, GpioPinStm& pin);

        void SetDuty(uint8_t dutyPercent) override;
        void SetPulse(uint32_t pulseOn, uint32_t period) override;
        void Start() override;
        void Stop() override;

    private:
        uint8_t timerIndex;
        uint8_t channelIndex;
        LPTIM_HandleTypeDef& handle;
        PeripheralPinStm pin;
        volatile std::uint32_t* ccr;

        void ConfigChannelInit();
    };
}

#endif

#endif // HAL_ST_LPTIMER_PWM_ST_HPP
