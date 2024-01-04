#ifndef HAL_ADC_DMA_STM_HPP
#define HAL_ADC_DMA_STM_HPP

#include "hal_st/stm32fxxx/AnalogToDigitalPinStm.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/TimerStm.hpp"

namespace hal
{
    class AdcTriggeredByTimerWithDma
        : private AdcStm
    {
    public:
        template<std::size_t Size>
        using WithNumberOfSamples = infra::WithStorage<AdcTriggeredByTimerWithDma, std::array<uint16_t, Size>>;

        AdcTriggeredByTimerWithDma(infra::MemoryRange<uint16_t> buffer, hal::DmaStm& dma, DmaChannelId dmaChannelId, uint8_t adcIndex, TimerBaseStm::Timing timing, hal::GpioPinStm& pin);
        void Measure(const infra::Function<void(infra::MemoryRange<uint16_t>&)>& onDone);

    private:
        void TransferDone();
        void Configure();

    private:
        infra::MemoryRange<uint16_t> buffer;
        DmaStm::StreamGeneric<uint16_t> stream;
        AnalogPinStm pin;
        TimerBaseStm timer;
        infra::Function<void(infra::MemoryRange<uint16_t>&)> onDone;
    };
}

#endif
