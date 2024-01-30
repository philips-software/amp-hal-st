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
        struct Config
        {
            constexpr Config()
            {}

            infra::Optional<DmaChannelId> dmaChannelTx;
            infra::Optional<DmaChannelId> dmaChannelRx;
        };

        SpiSlaveStmDma(hal::DmaStm& dmaStm, uint8_t oneBasedSpiIndex, GpioPinStm& clock, GpioPinStm& miso, GpioPinStm& mosi, GpioPinStm& slaveSelect, const Config& config = Config());

        void SendAndReceive(infra::ConstByteRange sendData, infra::ByteRange receiveData, const infra::Function<void()>& onDone) override;
        bool CancelTransmission() override;

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

        hal::DmaStm::Stream tx;
        hal::DmaStm::Stream rx;
    };
}

#endif
