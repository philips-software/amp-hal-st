#ifndef HAL_ST_ADC_DMA_MULTI_CHANNEL_HPP
#define HAL_ST_ADC_DMA_MULTI_CHANNEL_HPP

#include "hal/interfaces/AdcMultiChannel.hpp"
#include "hal_st/stm32fxxx/AnalogToDigitalPinStm.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/util/AutoResetFunction.hpp"
#include <array>
#include <cstddef>

/// @brief This ADC implementation supports multiple channels with DMA transfer and software trigger.
///        DMA transfer is one shot mode.
namespace hal
{
    class AdcDmaMultiChannelStmBase : public AdcMultiChannel
    {
    public:
        AdcDmaMultiChannelStmBase(
            infra::MemoryRange<uint16_t> buffer, infra::MemoryRange<AnalogPinStm> analogPins, AdcStm& adc,
            DmaStm::ReceiveStream& receiveStream);

        void Measure(const infra::Function<void(Samples)>& onDone) override;

        constexpr static std::size_t MaxChannels{ 16 };

    protected:
        void ConfigureChannels();

    private:
        infra::MemoryRange<uint16_t> buffer;
        infra::MemoryRange<AnalogPinStm> analogPins;
        AdcStm& adc;
        ReceiveDmaChannel dmaStream;
        infra::AutoResetFunction<void(Samples)> onDone;

        void TransferDone();
    };

    template<std::size_t Channels>
    class AdcDmaMultiChannelStm : public AdcDmaMultiChannelStmBase
    {
    public:
        static_assert(Channels <= MaxChannels, "Number of Channels not supported");

        template<class... Pins>
        explicit AdcDmaMultiChannelStm(AdcStm& adc, DmaStm::ReceiveStream& receiveStream, Pins&&... pins);

    private:
        std::array<uint16_t, Channels> bufferStorage{};
        std::array<AnalogPinStm, Channels> pinStorage;
    };

    ////    Implementation    ////

    template<std::size_t Channels>
    template<class... Pins>
    AdcDmaMultiChannelStm<Channels>::AdcDmaMultiChannelStm(
        AdcStm& adc, DmaStm::ReceiveStream& receiveStream, Pins&&... pins)
        : AdcDmaMultiChannelStmBase{ bufferStorage, pinStorage, adc, receiveStream }
        , pinStorage{ std::forward<Pins>(pins)... }
    {
        static_assert(sizeof...(Pins) == Channels, "Number of Pins and Channels not matching");
        ConfigureChannels();
    }

}

#endif
