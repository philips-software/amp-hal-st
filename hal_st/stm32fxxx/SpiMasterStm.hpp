#ifndef HAL_SPI_MASTER_STM_HPP
#define HAL_SPI_MASTER_STM_HPP

#include "hal/interfaces/Spi.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/util/AutoResetFunction.hpp"

namespace hal
{
    class SpiMasterStm
        : public SpiMaster
    {
    public:
        struct Config
        {
            constexpr Config() {}

            bool msbFirst = true;
            bool polarityLow = true;
            bool phase1st = true;
            uint32_t baudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
        };

        SpiMasterStm(uint8_t oneBasedSpiIndex, GpioPinStm& clock, GpioPinStm& miso, GpioPinStm& mosi, const Config& config = Config(), GpioPinStm& slaveSelect = dummyPinStm);

        virtual void SendAndReceive(infra::ConstByteRange sendData, infra::ByteRange receiveData, SpiAction nextAction, const infra::Function<void()>& onDone) override;
        virtual void SetChipSelectConfigurator(ChipSelectConfigurator& configurator) override;
        virtual void SetCommunicationConfigurator(CommunicationConfigurator& configurator) override;
        virtual void ResetCommunicationConfigurator() override;

    private:
        void HandleInterrupt();

    private:
        uint8_t spiInstance;
        PeripheralPinStm clock;
        PeripheralPinStm miso;
        PeripheralPinStm mosi;
        PeripheralPinStm slaveSelect;

        infra::AutoResetFunction<void()> onDone;
        ChipSelectConfigurator* chipSelectConfigurator = nullptr;
        CommunicationConfigurator* communicationConfigurator = nullptr;
        infra::Optional<DispatchedInterruptHandler> spiInterruptRegistration;
        infra::ConstByteRange sendData;
        infra::ByteRange receiveData;
        bool sending;
        bool receiving;
        uint32_t dummyToSend = 0;
        uint32_t dummyToReceive = 0;
        bool continuedSession = false;
    };
}

#endif
