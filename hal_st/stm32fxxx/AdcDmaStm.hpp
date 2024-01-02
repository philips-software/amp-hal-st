#ifndef HAL_ADC_DMA_STM_HPP
#define HAL_ADC_DMA_STM_HPP

#include "hal_st/stm32fxxx/AnalogToDigitalPinStm.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/TimerStm.hpp"
#include "infra/util/BoundedVector.hpp"
#include "infra/util/Optional.hpp"

namespace hal
{
    class AdcTriggeredByTimerWithDma
        : private AdcStm
    {
    public:
        template<std::size_t Max>
        using WithNumberOfConversions = infra::WithStorage<AdcTriggeredByTimerWithDma, infra::BoundedVector<int16_t>::WithMaxSize<Max>>;

        explicit AdcTriggeredByTimerWithDma(infra::BoundedVector<int16_t>& conversionBuffer, hal::DmaStm& dma, DmaChannelId dmaChannelId, uint8_t adcIndex, hal::GpioPinStm& pin);

        void Measure(const infra::Function<void(infra::BoundedVector<int16_t>&)>& onDone);

    private:
        void TransferDone();

    private:
        infra::BoundedVector<int16_t>& conversionBuffer;
        DmaStm::StreamGeneric<int16_t> stream;
        AnalogPinStm pin;
        TimerBaseStm::Config timerConfig{ 1, 39999, TimerBaseStm::CounterMode::up, infra::MakeOptional<TimerBaseStm::Trigger>({ TimerBaseStm::Trigger::TriggerOutput::update, false }) };
        AdcStm::Config adcConfig{ AdcStm::TriggerSource::timer, AdcStm::TriggerEdge::rising, true };
        TimerBaseStm timer;
        infra::Function<void(infra::BoundedVector<int16_t>&)> onDone;
    };
}

#endif
