#include "hal_st/stm32fxxx/LpTimerPwmStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/util/ReallyAssert.hpp"
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
    {}

    LpPwmChannelGpio& LpTimerPwmBaseStm::Channel(uint8_t channelOneBasedIndex)
    {
        return channels[channelOneBasedIndex - 1];
    }

    void LpTimerPwmBaseStm::Start()
    {
        for (auto& channel : channels)
            channel.Start();

        timer.Start();
    }

    void LpTimerPwmBaseStm::Stop()
    {
        timer.Stop();

        for (auto& channel : channels)
            channel.Stop();
    }

    LpPwmChannelGpio::LpPwmChannelGpio(uint8_t timerOneBasedIndex, uint8_t channelOneBasedIndex, LPTIM_HandleTypeDef& handle, GpioPinStm& pin)
        : timerIndex(timerOneBasedIndex - 1)
        , channelIndex(channelOneBasedIndex - 1)
        , handle(handle)
        , pin(pin, GetChannelPinConfig(channelIndex), timerOneBasedIndex)
        , ccr(GetCCRegister(handle.Instance, channelIndex))
    {
        ConfigChannelInit();
    }

    void LpPwmChannelGpio::ConfigChannelInit()
    {
        LPTIM_OC_ConfigTypeDef sConfig = { .OCPolarity = LPTIM_OCPOLARITY_HIGH };
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
        *ccr = pulseOn;
        handle.Instance->ARR = period;
    }

    void LpPwmChannelGpio::Start()
    {
        auto result = HAL_LPTIM_PWM_Start(&handle, GetLpTimerChannel(channelIndex));
        really_assert(result == HAL_OK);
    }

    void LpPwmChannelGpio::Stop()
    {
        auto result = HAL_LPTIM_PWM_Stop(&handle, GetLpTimerChannel(channelIndex));
        really_assert(result == HAL_OK);
    }
}

#endif
