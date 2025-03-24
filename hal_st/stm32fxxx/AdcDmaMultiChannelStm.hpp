#ifndef HAL_ST_ADC_DMA_MULTI_CHANNEL_HPP
#define HAL_ST_ADC_DMA_MULTI_CHANNEL_HPP

#include "hal/interfaces/AdcMultiChannel.hpp"
#include "hal_st/stm32fxxx/AdcTimerTriggeredBase.hpp"
#include "hal_st/stm32fxxx/AnalogToDigitalPinStm.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/TimerStm.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/MemoryRange.hpp"
#include "infra/util/Variant.hpp"
#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <utility>

/// @brief This ADC implementation supports multiple channels with DMA transfer and software trigger.
///        DMA transfer is either one shot or unlimited mode. In unlimited mode, ADC converion will run continuously until Stop() is called.
///        The conversion callback is executed from the interrupt scope. The user can decide to handle it later using the EventDispatcher.
namespace hal
{
    class AdcDmaMultiChannelStmBase
        : public AdcMultiChannel
    {
    public:
        AdcDmaMultiChannelStmBase(infra::MemoryRange<uint16_t> buffer, infra::MemoryRange<AnalogPinStm> analogPins, AdcStm& adc, DmaStm::ReceiveStream& receiveStream);

        void Measure(const infra::Function<void(Samples)>& onDone) override;

        constexpr static std::size_t MaxChannels{ 16 };

    protected:
        void ConfigureChannels(infra::MemoryRange<const detail::AdcStmChannelConfig> configs);

    private:
        infra::MemoryRange<uint16_t> buffer;
        infra::MemoryRange<AnalogPinStm> analogPins;
        AdcStm& adc;
        ReceiveDmaChannel dmaStream;
        infra::AutoResetFunction<void(Samples)> onDone;

        void TransferDone();
    };

    template<class T>
    concept AdcPin = std::constructible_from<AnalogPinStm, T>;

    template<AdcPin Pin>
    struct AdcPinConfig
        : detail::AdcStmChannelConfig
    {
        Pin pin;

        AdcPinConfig(Pin pin)
            : AdcStmChannelConfig{}
            , pin(pin)
        {}

        AdcPinConfig(Pin pin, uint32_t samplingTime)
            : AdcStmChannelConfig{ samplingTime }
            , pin(pin)
        {}

        AdcPinConfig(Pin pin, uint32_t samplingTime, bool differential)
            : AdcStmChannelConfig{ samplingTime, differential }
            , pin(pin)
        {}
    };

    AdcPinConfig(GpioPinStm pin, uint32_t samplingTime, bool differential) -> AdcPinConfig<GpioPinStm&>;
    AdcPinConfig(GpioPinStm pin, uint32_t samplingTime) -> AdcPinConfig<GpioPinStm&>;
    AdcPinConfig(GpioPinStm pin) -> AdcPinConfig<GpioPinStm&>;

    template<typename T, typename... U>
    concept IsAnyOf = (std::same_as<T, U> || ...);

    template<class T>
    concept PinConfig = IsAnyOf<T, AdcPinConfig<GpioPinStm&>, AdcPinConfig<uint32_t>, AdcPinConfig<unsigned long>>;

    template<std::size_t Channels>
    class AdcDmaMultiChannelStm : public AdcDmaMultiChannelStmBase
    {
    public:
        static_assert(Channels <= MaxChannels, "Number of Channels not supported");

        template<AdcPin... Pins>
        explicit AdcDmaMultiChannelStm(AdcStm& adc, DmaStm::ReceiveStream& receiveStream, Pins&&... pins);

        template<PinConfig... PinConfigs>
        explicit AdcDmaMultiChannelStm(AdcStm& adc, DmaStm::ReceiveStream& receiveStream, PinConfigs&&... pinConfigs);

    private:
        std::array<uint16_t, Channels> bufferStorage{};
        std::array<AnalogPinStm, Channels> pinStorage;

        template<PinConfig... PinConfigs>
        void Configure(PinConfigs&&... pinConfigs);
    };

    ////    Implementation    ////

    template<std::size_t Channels>
    template<AdcPin... Pins>
    AdcDmaMultiChannelStm<Channels>::AdcDmaMultiChannelStm(AdcStm& adc, DmaStm::ReceiveStream& receiveStream, Pins&&... pins)
        : AdcDmaMultiChannelStmBase{ bufferStorage, pinStorage, adc, receiveStream }
        , pinStorage{ std::forward<Pins>(pins)... }
    {
        static_assert(sizeof...(Pins) == Channels, "Number of Pins and Channels not matching");
        Configure();
    }

    template<std::size_t Channels>
    template<PinConfig... PinConfigs>
    AdcDmaMultiChannelStm<Channels>::AdcDmaMultiChannelStm(AdcStm& adc, DmaStm::ReceiveStream& receiveStream, PinConfigs&&... pinConfigs)
        : AdcDmaMultiChannelStmBase{ bufferStorage, pinStorage, adc, receiveStream }
        , pinStorage{ (AnalogPinStm{ pinConfigs.pin })... }
    {
        static_assert(sizeof...(PinConfigs) == Channels, "Number of Pins and Channels not matching");
        Configure(std::forward<PinConfigs>(pinConfigs)...);
    }

    template<std::size_t Channels>
    template<PinConfig... PinConfigs>
    void AdcDmaMultiChannelStm<Channels>::Configure(PinConfigs&&... pinConfigs)
    {
        if constexpr (sizeof...(PinConfigs) == 0)
        {
            std::array<detail::AdcStmChannelConfig, Channels> configs{};
            ConfigureChannels(configs);
        }
        else
        {
            std::array configs{ (detail::AdcStmChannelConfig{ pinConfigs.samplingTime, pinConfigs.differential })... };
            ConfigureChannels(configs);
        }
    }
}

#endif
