#ifndef HAL_ST_ADC_TIMER_TRIGGERED_BASE_HPP
#define HAL_ST_ADC_TIMER_TRIGGERED_BASE_HPP

#include "hal_st/stm32fxxx/AnalogToDigitalPinStm.hpp"
#include "hal_st/stm32fxxx/TimerStm.hpp"
#include <cstdint>

#if defined(HAS_PERIPHERAL_TIMER)

namespace hal
{
    class AdcTimerTriggeredBase
    {
    protected:
        AdcTimerTriggeredBase(AdcStm& adc, uint8_t oneBasedTimerIndex, TimerBaseStm::Timing timing);

        void ReconfigureTrigger();
        void StartTimer();
        void StopTimer();

    private:
        AdcStm& adc;
        FreeRunningTimerStm timer;
        uint8_t timerIndex;
    };
}

#endif

#endif // HAL_ST_ADC_TIMER_TRIGGERED_BASE_HPP
