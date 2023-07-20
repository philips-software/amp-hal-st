#ifndef HAL_SPI_MASTER_STM_DMA_HPP
#define HAL_SPI_MASTER_STM_DMA_HPP

#include "hal/interfaces/Spi.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/util/AutoResetFunction.hpp"

namespace hal
{
    class SpiMasterStmDma
        : public SpiMaster
    {
    public:
        struct Config
        {
            constexpr Config()
            {}

            bool msbFirst = true;
            uint32_t baudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
            infra::Optional<DmaChannelId> dmaChannelTx;
            infra::Optional<DmaChannelId> dmaChannelRx;
        };

        SpiMasterStmDma(hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedSpiIndex, GpioPinStm& clock, GpioPinStm& miso, GpioPinStm& mosi, const Config& config = Config(), GpioPinStm& slaveSelect = dummyPinStm);

        virtual void SendAndReceive(infra::ConstByteRange sendData, infra::ByteRange receiveData, SpiAction nextAction, const infra::Function<void()>& onDone) override;
        virtual void SetChipSelectConfigurator(ChipSelectConfigurator& configurator) override;
        virtual void SetCommunicationConfigurator(CommunicationConfigurator& configurator) override;
        virtual void ResetCommunicationConfigurator() override;

        void SetDataSize(uint8_t dataSizeInBits);
        uint8_t DataSize() const;

    private:
        void SendAndReceive(infra::ConstByteRange sendData, infra::ByteRange receiveData);
        void ReceiveDone();
        void SendDone();
        void TransferDone();
        void EnableDma();
        void DisableDma();

    private:
        uint8_t spiInstance;
        PeripheralPinStm clock;
        PeripheralPinStm miso;
        PeripheralPinStm mosi;
        PeripheralPinStm slaveSelect;

        ChipSelectConfigurator* chipSelectConfigurator = nullptr;
        CommunicationConfigurator* communicationConfigurator = nullptr;
        infra::AutoResetFunction<void()> onDone;
        bool continuedSession = false;

        infra::ConstByteRange sendBuffer;
        infra::ByteRange receiveBuffer;

        bool receiveDone = false;
        bool sendDone = false;

        hal::TransmitDmaChannel tx;
        hal::ReceiveDmaChannel rx;
    };
}

#endif
