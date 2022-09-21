#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal_st/stm32fxxx/AnalogToDigitalPinStm.hpp"
#include <cassert>

namespace hal
{
    AnalogToDigitalPinImplStm::AnalogToDigitalPinImplStm(hal::GpioPinStm& pin, AdcStm& adc)
        : pin(pin)
        , adc(adc)
    {}

    void AnalogToDigitalPinImplStm::Measure(const infra::Function<void(int32_t value)>& onDone)
    {
        ADC_ChannelConfTypeDef channelConfig;
        channelConfig.Channel = pin.AdcChannel(adc.index + 1);
        channelConfig.Rank = 1;
#if defined(STM32F0) || defined(STM32F3)
        channelConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
#else
        channelConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
        channelConfig.Offset = 0;
#endif
        HAL_StatusTypeDef result = HAL_ADC_ConfigChannel(&adc.handle, &channelConfig);
        assert(result == HAL_OK);

        adc.Measure(pin, onDone);
    }

    AdcStm::AdcStm(uint8_t oneBasedIndex)
        : index(oneBasedIndex - 1)
        , handle()
        , interruptHandler(ADC_IRQn, [this]() { MeasurementDone(); })
    {
        EnableClockAdc(index);

        handle.Instance = peripheralAdc[index];
#if !defined(STM32F3)
        handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
        handle.Init.Resolution = ADC_RESOLUTION_12B;
#endif
        handle.Init.ScanConvMode = DISABLE;
        handle.Init.ContinuousConvMode = DISABLE;
        handle.Init.DiscontinuousConvMode = DISABLE;
        handle.Init.NbrOfDiscConversion = 0;
#if !defined(STM32F3)
        handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
#endif
        handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
        handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
        handle.Init.NbrOfConversion = 1;
#if !defined(STM32F3)
        handle.Init.DMAContinuousRequests = DISABLE;
        handle.Init.EOCSelection = DISABLE;
#endif
        HAL_StatusTypeDef result = HAL_ADC_Init(&handle);
        assert(result == HAL_OK);
    }

    AdcStm::~AdcStm()
    {
        DisableClockAdc(index);
    }

    void AdcStm::Measure(AnalogPinStm& pin, const infra::Function<void(int32_t value)>& onDone)
    {
        this->onDone = onDone;

        HAL_StatusTypeDef result = HAL_ADC_Start_IT(&handle);
        assert(result == HAL_OK);
    }

    void AdcStm::MeasurementDone()
    {
        assert(onDone != nullptr);
        handle.Instance->SR &= ~ADC_SR_EOC;
        interruptHandler.ClearPending();
        onDone(HAL_ADC_GetValue(&handle));
    }
}
