#ifndef SYNCHRONOUS_HAL_SYNCHRONOUS_SPI_MASTER_STM_HPP
#define SYNCHRONOUS_HAL_SYNCHRONOUS_SPI_MASTER_STM_HPP

#include "hal/synchronous_interfaces/SynchronousSpi.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"

namespace hal
{

    class SynchronousSpiMasterStm
        : public SynchronousSpi
    {
    public:
        struct Config
        {
            constexpr Config()
            {}

            bool msbFirst = true;
            bool polarityLow = true;
            bool phase1st = true;
            uint32_t baudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
        };

        SynchronousSpiMasterStm(uint8_t oneBasedSpiIndex, GpioPinStm& clock, GpioPinStm& miso, GpioPinStm& mosi, const Config& config = Config(), GpioPinStm& slaveSelect = dummyPinStm);
        ~SynchronousSpiMasterStm();

        // Implementation of SynchronousSpi
        void SendAndReceive(infra::ConstByteRange sendData, infra::ByteRange receiveData, Action nextAction) override;

    private:
        void HandleInterrupt();

    private:
        uint8_t spiInstance;
        PeripheralPinStm clock;
        PeripheralPinStm miso;
        PeripheralPinStm mosi;
        PeripheralPinStm slaveSelect;

        infra::ConstByteRange sendData;
        infra::ByteRange receiveData;
        bool sending;
        bool receiving;
        uint32_t dummyToSend = 0;
        uint32_t dummyToReceive = 0;
    };

}

#endif
