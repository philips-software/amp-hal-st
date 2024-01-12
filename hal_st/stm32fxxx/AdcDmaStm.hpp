#ifndef HAL_ADC_DMA_STM_HPP
#define HAL_ADC_DMA_STM_HPP

#include "hal/interfaces/AnalogToDigitalPin.hpp"
#include "hal_st/stm32fxxx/AnalogToDigitalPinStm.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/TimerStm.hpp"
#include "infra/util/AutoResetFunction.hpp"
#include "infra/util/InterfaceConnector.hpp"

namespace hal
{
    class AdcTriggeredByTimerWithDma
        : public AnalogToDigitalBulkSamples<uint16_t>
        , public infra::InterfaceConnector<AdcTriggeredByTimerWithDma>
        , private AdcStm
    {
    public:
        explicit AdcTriggeredByTimerWithDma(hal::DmaStm& dma, DmaChannelId dmaChannelId, uint8_t adcIndex, TimerBaseStm::Timing timing, hal::GpioPinStm& pin);
        void Measure(infra::MemoryRange<uint16_t> buffer, const infra::Function<void()>& onDone) override;

    private:
        void TransferDone();
        void Configure();

    private:
        DmaStm::StreamWithConfigurableDataSize<uint16_t> stream;
        AnalogPinStm analogPin;
        TimerBaseStm timer;
        infra::AutoResetFunction<void()> onDone;
    };
}

#endif
