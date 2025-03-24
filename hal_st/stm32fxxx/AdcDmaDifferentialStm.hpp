#ifndef HAL_ADC_DMA_STM_HPP
#define HAL_ADC_DMA_STM_HPP

#include "hal/interfaces/AnalogToDigitalPin.hpp"
#include "hal_st/stm32fxxx/AdcTimerTriggeredBase.hpp"
#include "hal_st/stm32fxxx/AnalogToDigitalPinStm.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/TimerStm.hpp"
#include "infra/util/Function.hpp"
#include <cstdint>

namespace hal
{
    /// @brief This ADC implementation will convert in differential mode with DMA transfer and timer trigger.
    ///        DMA transfer is unlimited mode. This means ADC converion will run continuously until Stop() is called.
    ///        The conversion callback is executed from the interrupt scope. The user can decide to handle it later using the EventDispatcher.
    class AdcDifferentialTriggeredByTimer
        : public AnalogToDigitalPinImplBase<uint16_t>
        , private AdcTimerTriggeredBase
    {
    public:
        using Config = detail::AdcStmChannelConfig;

        AdcDifferentialTriggeredByTimer(AdcStm& adc, DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedTimerIndex, TimerBaseStm::Timing timing, hal::GpioPinStm& positivePin, hal::GpioPinStm& negativePin, const Config& config = {});

        void Measure(std::size_t numberOfSamples, const infra::Function<void(infra::MemoryRange<uint16_t>)>& onDone) override;
        void Stop();

    private:
        void TransferDone();
        void Configure();

    private:
        AdcStm& adc;
        CircularReceiveDmaChannel dmaStream;
        AnalogPinStm analogPinPositive;
        AnalogPinStm analogPinNegative;
        uint16_t buffer{ 0 };
        infra::Function<void(infra::MemoryRange<uint16_t>)> onDone;
    };
}

#endif
