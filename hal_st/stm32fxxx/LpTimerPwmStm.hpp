#ifndef HAL_ST_LPTIMER_PWM_ST_HPP
#define HAL_ST_LPTIMER_PWM_ST_HPP

#include "hal/interfaces/PulseWidthModulation.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/LpTimerStm.hpp"
#include "infra/util/BoundedDeque.hpp"
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
            return { { (Is, T{ timerOneBasedIndex, Is + 1, handle, pins[Is] })... } };
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

        virtual LpPwmChannelGpio& Channel(uint8_t channelOneBasedIndex);

        // Timer and configured channels can either be started independently from each other by invoking StartTimer() and StartChannels(), or together by invoking Start()
        void StartTimer();
        void StartChannels();
        void Start();
        // Timer and configured channels can either be stopped independently from each other by invoking StopTimer() and StopChannels(), or together by invoking Stop()
        void StopTimer();
        void StopChannels();
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

    class LpTimerPwm
        : public LpTimerPwmBaseStm
    {
    public:
        template<std::size_t Channels>
        using WithChannels = infra::WithStorage<LpTimerPwm, infra::BoundedDeque<LpPwmChannelGpio>::WithMaxSize<Channels>>;

        LpTimerPwm(infra::BoundedDeque<LpPwmChannelGpio>& channelStorage, uint8_t oneBasedIndex, LowPowerTimerBaseStm::Timing timing);

        void ConfigureChannel(uint8_t channelOneBasedIndex, GpioPinStm& pin);
        LpPwmChannelGpio& Channel(uint8_t channelOneBasedIndex) override;

    private:
        infra::BoundedDeque<LpPwmChannelGpio>& channelStorage;
        uint8_t oneBasedIndex;
    };

    class LpPwmChannelGpio
        : public PulseWidthModulation
    {
    public:
        LpPwmChannelGpio(uint8_t timerOneBasedIndex, uint8_t channelOneBasedIndex, LPTIM_HandleTypeDef& handle, GpioPinStm& pin, uint32_t polarity = LPTIM_OCPOLARITY_HIGH);

        // Implementation of PulseWidthModulation
        void SetDuty(uint8_t dutyPercent) override;
        void SetPulse(uint32_t pulseOn, uint32_t period) override;
        void Start() override;
        void Stop() override;

        void ConfigurePolarity(uint32_t polarity);

    private:
        friend LpTimerPwm;

        uint8_t timerIndex;
        uint8_t channelIndex;
        LPTIM_HandleTypeDef& handle;
        PeripheralPinStm pin;
        volatile std::uint32_t* ccr;
    };
}

#endif

#endif
