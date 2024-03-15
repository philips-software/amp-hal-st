#ifndef HAL_ADC_DMA_STM_HPP
#define HAL_ADC_DMA_STM_HPP

#include "hal/interfaces/AnalogToDigitalPin.hpp"
#include "hal_st/stm32fxxx/AnalogToDigitalPinStm.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/TimerStm.hpp"
#include "infra/util/AutoResetFunction.hpp"

namespace hal
{
    class AdcTriggeredByTimerWithDma
        : public AnalogToDigitalPinImplBase<uint16_t>
    {
    public:
        template<std::size_t Max>
        using WithNumberOfSamples = infra::WithStorage<AdcTriggeredByTimerWithDma, std::array<uint16_t, Max>>;

        explicit AdcTriggeredByTimerWithDma(infra::MemoryRange<uint16_t> buffer, AdcStm& adc, DmaStm::ReceiveStream& receiveStream, TimerBaseStm::Timing timing, hal::GpioPinStm& pin);

        void Measure(std::size_t numberOfSamples, const infra::Function<void(infra::MemoryRange<uint16_t>)>& onDone) override;

    private:
        void ReconfigureTrigger();
        void TransferDone();
        void Configure();

    private:
        AdcStm& adc;
        infra::MemoryRange<uint16_t> buffer;
        ReceiveDmaChannel dmaStream;
        AnalogPinStm analogPin;
        TimerBaseStm timer;
        infra::AutoResetFunction<void(infra::MemoryRange<uint16_t>)> onDone;
    };
}

#endif
