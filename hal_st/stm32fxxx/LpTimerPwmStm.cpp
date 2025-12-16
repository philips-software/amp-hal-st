#include "hal_st/stm32fxxx/LpTimerPwmStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include <array>
#include <cstdint>

#if defined(HAS_PERIPHERAL_LPTIMER)

namespace
{
    constexpr std::array channelPinConfigsLp{
        hal::PinConfigTypeStm::lpTimerChannel1,
        hal::PinConfigTypeStm::lpTimerChannel2
    };

    hal::PinConfigTypeStm GetChannelPinConfig(std::size_t index)
    {
        assert(index < channelPinConfigsLp.size());
        return channelPinConfigsLp[index];
    }

    constexpr std::array lpTimerChannels{
        LPTIM_CHANNEL_1,
        LPTIM_CHANNEL_2,

    };

    uint32_t GetLpTimerChannel(std::size_t index)
    {
        assert(index < lpTimerChannels.size());
        return lpTimerChannels[index];
    }

    const std::array ccr{
        &LPTIM_TypeDef::CCR1,
        &LPTIM_TypeDef::CCR2,
    };

    volatile std::uint32_t* GetCCRegister(LPTIM_TypeDef* tim, std::size_t index)
    {
        assert(index < ccr.size());
        return &(tim->*ccr[index]);
    }
}

namespace hal
{
    LpTimerPwmBaseStm::LpTimerPwmBaseStm(uint8_t oneBasedIndex, LowPowerTimerBaseStm::Timing timing)
        : timer(oneBasedIndex, timing)
    {
        LL_LPTIM_SetWaveform(timer.Handle().Instance, LL_LPTIM_OUTPUT_WAVEFORM_PWM);
    }

    LpPwmChannelGpio& LpTimerPwmBaseStm::Channel(uint8_t channelOneBasedIndex)
    {
        return channels[channelOneBasedIndex - 1];
    }

    void LpTimerPwmBaseStm::StartTimer()
    {
        timer.Start();
    }

    void LpTimerPwmBaseStm::StartChannels()
    {
        for (auto& channel : channels)
            channel.Start();
    }

    void LpTimerPwmBaseStm::Start()
    {
        StartChannels();
        StartTimer();
    }

    void LpTimerPwmBaseStm::StopTimer()
    {
        timer.Stop();
    }

    void LpTimerPwmBaseStm::StopChannels()
    {
        for (auto& channel : channels)
            channel.Stop();
    }

    void LpTimerPwmBaseStm::Stop()
    {
        StopChannels();
        StopTimer();
    }

    LpTimerPwm::LpTimerPwm(infra::BoundedDeque<LpPwmChannelGpio>& channelStorage, uint8_t oneBasedIndex, LowPowerTimerBaseStm::Timing timing)
        : LpTimerPwmBaseStm(oneBasedIndex, timing)
        , channelStorage(channelStorage)
        , oneBasedIndex(oneBasedIndex)
    {}

    LpPwmChannelGpio& LpTimerPwm::Channel(uint8_t channelOneBasedIndex)
    {
        for (auto& channel : channelStorage)
        {
            if (channel.channelIndex == channelOneBasedIndex - 1)
                return channel;
        }
        std::abort();
    }

    void LpTimerPwm::ConfigureChannel(uint8_t channelOneBasedIndex, GpioPinStm& pin)
    {
        channelStorage.emplace_back(oneBasedIndex, channelOneBasedIndex, timer.Handle(), pin);
        channels = channelStorage.contiguous_range(channelStorage.begin());
    }

    LpPwmChannelGpio::LpPwmChannelGpio(uint8_t timerOneBasedIndex, uint8_t channelOneBasedIndex, LPTIM_HandleTypeDef& handle, GpioPinStm& pin, uint32_t polarity)
        : timerIndex(timerOneBasedIndex - 1)
        , channelIndex(channelOneBasedIndex - 1)
        , handle(handle)
        , pin(pin, GetChannelPinConfig(channelIndex), timerOneBasedIndex)
        , ccr(GetCCRegister(handle.Instance, channelIndex))
    {
        ConfigurePolarity(polarity);
    }

    void LpPwmChannelGpio::ConfigurePolarity(uint32_t polarity)
    {
        LPTIM_OC_ConfigTypeDef sConfig = { .OCPolarity = polarity };
        auto result = HAL_LPTIM_OC_ConfigChannel(&handle, &sConfig, GetLpTimerChannel(channelIndex));
        assert(result == HAL_OK);
    }

    void LpPwmChannelGpio::SetDuty(uint8_t dutyPercent)
    {
        assert(dutyPercent >= 0 && dutyPercent <= 100);
        *ccr = handle.Instance->ARR * dutyPercent / 100;
    }

    void LpPwmChannelGpio::SetPulse(uint32_t pulseOn, uint32_t period)
    {
        assert(LL_LPTIM_IsEnabled(handle.Instance));
        *ccr = pulseOn;
        handle.Instance->ARR = period;
    }

    void LpPwmChannelGpio::Start()
    {
        LL_LPTIM_CC_EnableChannel(handle.Instance, GetLpTimerChannel(channelIndex));
    }

    void LpPwmChannelGpio::Stop()
    {
        LL_LPTIM_CC_DisableChannel(handle.Instance, GetLpTimerChannel(channelIndex));
    }
}

#endif
