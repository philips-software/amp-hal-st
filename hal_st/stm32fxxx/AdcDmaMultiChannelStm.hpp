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
    namespace detail
    {
        struct AdcChannelConfigExt : AdcStmChannelConfig
        {
            bool differential{ false };
        };

        template<typename T, typename... U>
        concept IsAnyOf = (std::same_as<T, U> || ...);
    }

    class AdcDmaMultiChannelStmBase
        : public AdcMultiChannel
    {
    public:
        static constexpr std::size_t MaxChannels{ 16 };

        struct OneShot
        {};

        struct Unlimited
        {};

        AdcDmaMultiChannelStmBase(infra::MemoryRange<uint16_t> buffer, infra::MemoryRange<AnalogPinStm> analogPins, AdcStm& adc, DmaStm::ReceiveStream& receiveStream, OneShot = {});
        AdcDmaMultiChannelStmBase(infra::MemoryRange<uint16_t> buffer, infra::MemoryRange<AnalogPinStm> analogPins, AdcStm& adc, DmaStm::ReceiveStream& receiveStream, Unlimited);

        void Measure(const infra::Function<void(Samples)>& onDone) override;
        virtual void Stop();

    protected:
        void ConfigureChannels(infra::MemoryRange<const detail::AdcChannelConfigExt> configs);

    private:
        infra::MemoryRange<uint16_t> buffer;
        infra::MemoryRange<AnalogPinStm> analogPins;
        AdcStm& adc;
        infra::Variant<ReceiveDmaChannel, CircularReceiveDmaChannel> dmaStream;
        infra::Function<void(Samples)> onDone;

        void Initialize();
        void TransferDone();
    };

    template<class T>
    concept AdcPin = std::constructible_from<AnalogPinStm, T>;

    template<AdcPin Pin>
    struct AdcPinConfig : detail::AdcChannelConfigExt
    {
        Pin pin;

        AdcPinConfig(Pin pin)
            : AdcChannelConfigExt{}
            , pin(pin)
        {}

        AdcPinConfig(Pin pin, uint32_t samplingTime)
            : AdcChannelConfigExt{ samplingTime }
            , pin(pin)
        {}

        AdcPinConfig(Pin pin, uint32_t samplingTime, bool differential)
            : AdcChannelConfigExt{ samplingTime, differential }
            , pin(pin)
        {}
    };

    template<class T>
    concept PinConfig = detail::IsAnyOf<T, AdcPinConfig<GpioPinStm&>, AdcPinConfig<uint32_t>, AdcPinConfig<unsigned long>>;

    template<class T>
    concept DmaMode = detail::IsAnyOf<T, AdcDmaMultiChannelStmBase::OneShot, AdcDmaMultiChannelStmBase::Unlimited>;

    template<std::size_t Channels, DmaMode Mode = AdcDmaMultiChannelStmBase::OneShot>
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

    template<std::size_t Channels>
    class AdcDmaMultiChannelStmTriggeredByTimer
        : private AdcTimerTriggeredBase
        , public AdcDmaMultiChannelStm<Channels, AdcDmaMultiChannelStmBase::Unlimited>
    {
    public:
        template<AdcPin... Pins>
        explicit AdcDmaMultiChannelStmTriggeredByTimer(AdcStm& adc, DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedTimerIndex, TimerBaseStm::Timing timing, Pins&&... pins);
        template<PinConfig... PinConfigs>
        explicit AdcDmaMultiChannelStmTriggeredByTimer(AdcStm& adc, DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedTimerIndex, TimerBaseStm::Timing timing, PinConfigs&&... pinConfigs);

        void Measure(const infra::Function<void(AdcMultiChannel::Samples)>& onDone) override;
        void Stop() override;
    };

    ////    Implementation    ////

    template<std::size_t Channels, DmaMode Mode>
    template<AdcPin... Pins>
    AdcDmaMultiChannelStm<Channels, Mode>::AdcDmaMultiChannelStm(AdcStm& adc, DmaStm::ReceiveStream& receiveStream, Pins&&... pins)
        : AdcDmaMultiChannelStmBase{ bufferStorage, pinStorage, adc, receiveStream, Mode{} }
        , pinStorage{ std::forward<Pins>(pins)... }
    {
        static_assert(sizeof...(Pins) == Channels, "Number of Pins and Channels not matching");
        Configure();
    }

    template<std::size_t Channels, DmaMode Mode>
    template<PinConfig... PinConfigs>
    AdcDmaMultiChannelStm<Channels, Mode>::AdcDmaMultiChannelStm(AdcStm& adc, DmaStm::ReceiveStream& receiveStream, PinConfigs&&... pinConfigs)
        : AdcDmaMultiChannelStmBase{ bufferStorage, pinStorage, adc, receiveStream, Mode{} }
        , pinStorage{ (AnalogPinStm{ pinConfigs.pin })... }
    {
        static_assert(sizeof...(PinConfigs) == Channels, "Number of Pins and Channels not matching");
        Configure(std::forward<PinConfigs>(pinConfigs)...);
    }

    template<std::size_t Channels, DmaMode Mode>
    template<PinConfig... PinConfigs>
    void AdcDmaMultiChannelStm<Channels, Mode>::Configure(PinConfigs&&... pinConfigs)
    {
        if constexpr (sizeof...(PinConfigs) == 0)
        {
            std::array<detail::AdcChannelConfigExt, Channels> configs{};
            ConfigureChannels(configs);
        }
        else
        {
            std::array configs{ (detail::AdcChannelConfigExt{ pinConfigs.samplingTime, pinConfigs.differential })... };
            ConfigureChannels(configs);
        }
    }

    template<std::size_t Channels>
    template<AdcPin... Pins>
    AdcDmaMultiChannelStmTriggeredByTimer<Channels>::AdcDmaMultiChannelStmTriggeredByTimer(AdcStm& adc, DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedTimerIndex, TimerBaseStm::Timing timing, Pins&&... pins)
        : AdcTimerTriggeredBase(adc, oneBasedTimerIndex, timing)
        , AdcDmaMultiChannelStm<Channels, AdcDmaMultiChannelStmBase::Unlimited>(adc, receiveStream, std::forward<Pins>(pins)...)
    {
        ReconfigureTrigger();
    }

    template<std::size_t Channels>
    template<PinConfig... PinConfigs>
    AdcDmaMultiChannelStmTriggeredByTimer<Channels>::AdcDmaMultiChannelStmTriggeredByTimer(AdcStm& adc, DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedTimerIndex, TimerBaseStm::Timing timing, PinConfigs&&... pinConfigs)
        : AdcTimerTriggeredBase(adc, oneBasedTimerIndex, timing)
        , AdcDmaMultiChannelStm<Channels, AdcDmaMultiChannelStmBase::Unlimited>(adc, receiveStream, std::forward<PinConfigs>(pinConfigs)...)
    {
        ReconfigureTrigger();
    }

    template<std::size_t Channels>
    void AdcDmaMultiChannelStmTriggeredByTimer<Channels>::Measure(const infra::Function<void(AdcMultiChannel::Samples)>& onDone)
    {
        StartTimer();
        AdcDmaMultiChannelStmBase::Measure(onDone);
    }

    template<std::size_t Channels>
    void AdcDmaMultiChannelStmTriggeredByTimer<Channels>::Stop()
    {
        AdcDmaMultiChannelStmBase::Stop();
        StopTimer();
    }
}

#endif
