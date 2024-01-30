#ifndef HAL_SINGLE_SPEED_QUAD_SPI_STM_DMA_HPP
#define HAL_SINGLE_SPEED_QUAD_SPI_STM_DMA_HPP

#include "hal/interfaces/Spi.hpp"
#include "hal_st/stm32fxxx/QuadSpiStmDma.hpp"

#if defined(HAS_PERIPHERAL_QUADSPI)

namespace hal
{
    class SingleSpeedQuadSpiStmDma
        : public SpiMaster
    {
    public:
        SingleSpeedQuadSpiStmDma(QuadSpiStmDma& quadSpi);

        void SendAndReceive(infra::ConstByteRange sendData, infra::ByteRange receiveData, SpiAction nextAction, const infra::Function<void()>& onDone) override;
        void SetChipSelectConfigurator(ChipSelectConfigurator& configurator) override;
        void SetCommunicationConfigurator(CommunicationConfigurator& configurator) override;
        void ResetCommunicationConfigurator() override;

    private:
        void Done();

    private:
        QuadSpiStmDma& quadSpi;
        ChipSelectConfigurator* chipSelectConfigurator = nullptr;
        CommunicationConfigurator* communicationConfigurator = nullptr;
        bool continuedSession = false;
        infra::AutoResetFunction<void()> onDone;
    };
}

#endif

#endif
