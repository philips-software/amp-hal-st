#include "hal_st/stm32fxxx/TimerPwmStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/TimerStm.hpp"
#include "infra/util/ReallyAssert.hpp"
#include <array>
#include <cstdint>

#if defined(HAS_PERIPHERAL_TIMER)

namespace
{
    constexpr std::array channelPinConfigs{
        hal::PinConfigTypeStm::timerChannel1,
        hal::PinConfigTypeStm::timerChannel2,
        hal::PinConfigTypeStm::timerChannel3,
        hal::PinConfigTypeStm::timerChannel4
    };

    hal::PinConfigTypeStm GetChannelPinConfig(std::size_t index)
    {
        assert(index < channelPinConfigs.size());
        return channelPinConfigs[index];
    }

    constexpr std::array timerChannels{
        TIM_CHANNEL_1,
        TIM_CHANNEL_2,
        TIM_CHANNEL_3,
        TIM_CHANNEL_4,
#if defined(TIM_CHANNEL_5)
        TIM_CHANNEL_5,
#endif
#if defined(TIM_CHANNEL_6)
        TIM_CHANNEL_6
#endif
    };

    uint32_t GetTimerChannel(std::size_t index)
    {
        assert(index < timerChannels.size());
        return timerChannels[index];
    }

    const std::array ccr{
        &TIM_TypeDef::CCR1,
        &TIM_TypeDef::CCR2,
        &TIM_TypeDef::CCR3,
        &TIM_TypeDef::CCR4,
#if defined(TIM_CHANNEL_5)
        &TIM_TypeDef::CCR5,
#endif
#if defined(TIM_CHANNEL_6)
        &TIM_TypeDef::CCR6,
#endif
    };

    volatile std::uint32_t* GetCCRegister(TIM_TypeDef* tim, std::size_t index)
    {
        assert(index < ccr.size());
        return &(tim->*ccr[index]);
    }
}

namespace hal
{
    TimerPwmBaseStm::TimerPwmBaseStm(uint8_t oneBasedIndex, TimerBaseStm::Timing timing)
        : timer(oneBasedIndex, timing, { TimerBaseStm::CounterMode::up })
    {}

    PwmChannelGpio& TimerPwmBaseStm::Channel(uint8_t channelOneBasedIndex)
    {
        return channels[channelOneBasedIndex - 1];
    }

    void TimerPwmBaseStm::Start()
    {
        for (auto& channel : channels)
            channel.Start();

        timer.Start();
    }

    void TimerPwmBaseStm::Stop()
    {
        timer.Stop();

        for (auto& channel : channels)
            channel.Stop();
    }

    TimerPwmLazy::TimerPwmLazy(infra::BoundedDeque<PwmChannelGpio>& channelStorage, uint8_t oneBasedIndex, TimerBaseStm::Timing timing)
        : TimerPwmBaseStm(oneBasedIndex, timing)
        , channelStorage(channelStorage)
        , oneBasedIndex(oneBasedIndex)
    {}

    void TimerPwmLazy::ConfigureChannel(uint8_t channelOneBasedIndex, GpioPinStm& pin)
    {
        channelStorage.emplace_back(oneBasedIndex, channelOneBasedIndex, timer.Handle(), pin);
        channels = channelStorage.contiguous_range(channelStorage.begin());
    }

    PwmChannelGpio& TimerPwmLazy::Channel(uint8_t channelOneBasedIndex)
    {
        for (auto& channel : channelStorage)
        {
            if (channel.channelIndex == channelOneBasedIndex - 1)
                return channel;
        }
        std::abort();
    }

    PwmChannelGpio::PwmChannelGpio(uint8_t timerOneBasedIndex, uint8_t channelOneBasedIndex, TIM_HandleTypeDef& handle, GpioPinStm& pin)
        : timerIndex(timerOneBasedIndex - 1)
        , channelIndex(channelOneBasedIndex - 1)
        , handle(handle)
        , pin(pin, GetChannelPinConfig(channelIndex), timerOneBasedIndex)
        , ccr(GetCCRegister(handle.Instance, channelIndex))
    {
        TIM_OC_InitTypeDef init{};
        init.OCMode = TIM_OCMODE_PWM1;
        init.Pulse = 0;
        init.OCPolarity = TIM_OCPOLARITY_HIGH;
        init.OCFastMode = TIM_OCFAST_DISABLE;
        auto result = HAL_TIM_PWM_ConfigChannel(&handle, &init, GetTimerChannel(channelIndex));
        assert(result == HAL_OK);
    }

    void PwmChannelGpio::SetDuty(uint8_t dutyPercent)
    {
        assert(dutyPercent >= 0 && dutyPercent <= 100);
        *ccr = handle.Instance->ARR * dutyPercent / 100;
    }

    void PwmChannelGpio::SetPulse(uint32_t pulseOn, uint32_t period)
    {
        *ccr = pulseOn;
        handle.Instance->ARR = period;
    }

    void PwmChannelGpio::Start()
    {
        auto result = HAL_TIM_PWM_Start(&handle, GetTimerChannel(channelIndex));
        really_assert(result == HAL_OK);
    }

    void PwmChannelGpio::Stop()
    {
        auto result = HAL_TIM_PWM_Stop(&handle, GetTimerChannel(channelIndex));
        really_assert(result == HAL_OK);
    }
}

#endif
