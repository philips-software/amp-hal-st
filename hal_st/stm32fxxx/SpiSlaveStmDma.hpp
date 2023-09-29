#ifndef HAL_SPI_SLAVE_STM_DMA_HPP
#define HAL_SPI_SLAVE_STM_DMA_HPP

#include "hal/interfaces/Spi.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/util/AutoResetFunction.hpp"

namespace hal
{
    class SpiSlaveStmDma
        : public SpiSlave
    {
    public:
        SpiSlaveStmDma(hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedSpiIndex, GpioPinStm& clock, GpioPinStm& miso, GpioPinStm& mosi, GpioPinStm& slaveSelect);

        virtual void SendAndReceive(infra::ConstByteRange sendData, infra::ByteRange receiveData, const infra::Function<void()>& onDone) override;
        virtual bool CancelTransmission() override;

    private:
        void SendAndReceive(infra::ConstByteRange sendData, infra::ByteRange receiveData);
        void ReceiveDone();
        void SendDone();
        void TransferDone();
        void Configure();
        void EnableDma();
        void DisableDma();

    private:
        uint8_t spiInstance;
        PeripheralPinStm clock;
        PeripheralPinStm miso;
        PeripheralPinStm mosi;
        PeripheralPinStm slaveSelect;

        infra::AutoResetFunction<void()> onDone;

        bool receiveDone = false;
        bool sendDone = false;

        hal::TransmitDmaChannel tx;
        hal::ReceiveDmaChannel rx;
    };
}

#endif
