#include "hal_st/stm32fxxx/AdcTimerTriggeredBase.hpp"
#include "hal_st/stm32fxxx/AnalogToDigitalPinStm.hpp"
#include DEVICE_HEADER

#if defined(STM32F7)
#include "stm32f7xx_ll_adc.h"
#endif

#if defined(HAS_PERIPHERAL_TIMER)

namespace
{
    constexpr unsigned long TriggerUnsupported{ 255 };

    constexpr std::array triggers = {
#if defined(ADC_EXTERNALTRIGCONV_T1_TRGO)
        ADC_EXTERNALTRIGCONV_T1_TRGO,
#elif defined(ADC_EXTERNALTRIG_T1_TRGO)
        ADC_EXTERNALTRIG_T1_TRGO,
#elif defined(ADC_EXTERNALTRIG_T1_TRGO2)
        ADC_EXTERNALTRIG_T1_TRGO2,
#else
        TriggerUnsupported,
#endif
#if defined(ADC_EXTERNALTRIGCONV_T2_TRGO)
        ADC_EXTERNALTRIGCONV_T2_TRGO,
#elif defined(ADC_EXTERNALTRIG_T2_TRGO)
        ADC_EXTERNALTRIG_T2_TRGO,
#else
        TriggerUnsupported,
#endif
#if defined(ADC_EXTERNALTRIGCONV_T3_TRGO)
        ADC_EXTERNALTRIGCONV_T3_TRGO,
#elif defined(ADC_EXTERNALTRIG_T3_TRGO)
        ADC_EXTERNALTRIG_T3_TRGO,
#else
        TriggerUnsupported,
#endif
#if defined(ADC_EXTERNALTRIGCONV_T4_TRGO)
        ADC_EXTERNALTRIGCONV_T4_TRGO,
#elif defined(ADC_EXTERNALTRIG_T4_TRGO)
        ADC_EXTERNALTRIG_T4_TRGO,
#else
        TriggerUnsupported,
#endif
#if defined(ADC_EXTERNALTRIGCONV_T5_TRGO)
        ADC_EXTERNALTRIGCONV_T5_TRGO,
#elif defined(ADC_EXTERNALTRIG_T5_TRGO)
        ADC_EXTERNALTRIG_T5_TRGO,
#else
        TriggerUnsupported,
#endif
#if defined(ADC_EXTERNALTRIGCONV_T6_TRGO)
        ADC_EXTERNALTRIGCONV_T6_TRGO,
#elif defined(ADC_EXTERNALTRIG_T6_TRGO)
        ADC_EXTERNALTRIG_T6_TRGO,
#else
        TriggerUnsupported,
#endif
#if defined(ADC_EXTERNALTRIGCONV_T7_TRGO)
        ADC_EXTERNALTRIGCONV_T7_TRGO,
#elif defined(ADC_EXTERNALTRIG_T7_TRGO)
        ADC_EXTERNALTRIG_T7_TRGO,
#else
        TriggerUnsupported,
#endif
#if defined(ADC_EXTERNALTRIGCONV_T8_TRGO)
        ADC_EXTERNALTRIGCONV_T8_TRGO,
#elif defined(ADC_EXTERNALTRIG_T8_TRGO)
        ADC_EXTERNALTRIG_T8_TRGO,
#else
        TriggerUnsupported,
#endif
#if defined(ADC_EXTERNALTRIGCONV_T9_TRGO)
        ADC_EXTERNALTRIGCONV_T9_TRGO,
#elif defined(ADC_EXTERNALTRIG_T9_TRGO)
        ADC_EXTERNALTRIG_T9_TRGO,
#else
        TriggerUnsupported,
#endif
#if defined(ADC_EXTERNALTRIGCONV_T11_TRGO)
        ADC_EXTERNALTRIGCONV_T11_TRGO,
#elif defined(ADC_EXTERNALTRIG_T11_TRGO)
        ADC_EXTERNALTRIG_T11_TRGO,
#else
        TriggerUnsupported,
#endif
#if defined(ADC_EXTERNALTRIGCONV_T12_TRGO)
        ADC_EXTERNALTRIGCONV_T12_TRGO,
#elif defined(ADC_EXTERNALTRIG_T12_TRGO)
        ADC_EXTERNALTRIG_T12_TRGO,
#else
        TriggerUnsupported,
#endif
#if defined(ADC_EXTERNALTRIGCONV_T13_TRGO)
        ADC_EXTERNALTRIGCONV_T13_TRGO,
#elif defined(ADC_EXTERNALTRIG_T13_TRGO)
        ADC_EXTERNALTRIG_T13_TRGO,
#else
        TriggerUnsupported,
#endif
#if defined(ADC_EXTERNALTRIGCONV_T14_TRGO)
        ADC_EXTERNALTRIGCONV_T14_TRGO,
#elif defined(ADC_EXTERNALTRIG_T14_TRGO)
        ADC_EXTERNALTRIG_T14_TRGO,
#else
        TriggerUnsupported,
#endif
#if defined(ADC_EXTERNALTRIGCONV_T15_TRGO)
        ADC_EXTERNALTRIGCONV_T15_TRGO,
#elif defined(ADC_EXTERNALTRIG_T15_TRGO)
        ADC_EXTERNALTRIG_T15_TRGO,
#else
        TriggerUnsupported,
#endif
    };

    uint32_t FindTrigger(uint8_t index)
    {
        assert(index < triggers.size());
        assert(triggers[index] != TriggerUnsupported);
        return triggers[index];
    }
}

namespace hal
{
    AdcTimerTriggeredBase::AdcTimerTriggeredBase(AdcStm& adc, uint8_t oneBasedTimerIndex, TimerBaseStm::Timing timing)
        : adc(adc)
        , timer(oneBasedTimerIndex, timing, { TimerBaseStm::CounterMode::up, infra::MakeOptional<TimerBaseStm::Trigger>({ TimerBaseStm::Trigger::TriggerOutput::update, false }) })
        , timerIndex(oneBasedTimerIndex - 1)
    {}

    void AdcTimerTriggeredBase::ReconfigureTrigger()
    {
        HAL_ADC_Stop(&adc.Handle());

        LL_ADC_REG_SetTriggerSource(adc.Handle().Instance, FindTrigger(timerIndex));
#if defined(ADC_CFGR1_EXTEN) || defined(ADC_CFGR_EXTEN)
        LL_ADC_REG_SetTriggerEdge(adc.Handle().Instance, ADC_EXTERNALTRIGCONVEDGE_RISING);
#endif
    }

    void AdcTimerTriggeredBase::StartTimer()
    {
        timer.Start();
    }

    void AdcTimerTriggeredBase::StopTimer()
    {
        timer.Stop();
    }
}

#endif
