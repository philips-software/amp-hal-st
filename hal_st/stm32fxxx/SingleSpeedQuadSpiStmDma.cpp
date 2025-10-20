#include "hal_st/stm32fxxx/SingleSpeedQuadSpiStmDma.hpp"

#if defined(HAS_PERIPHERAL_QUADSPI)

namespace hal
{
    SingleSpeedQuadSpiStmDma::SingleSpeedQuadSpiStmDma(QuadSpiStmDma& quadSpi)
        : quadSpi(quadSpi)
    {}

    void SingleSpeedQuadSpiStmDma::SendAndReceive(infra::ConstByteRange sendData, infra::ByteRange receiveData, SpiAction nextAction, const infra::Function<void()>& onDone)
    {
        assert(sendData.empty() || receiveData.empty());

        this->onDone = onDone;
        if (chipSelectConfigurator && !continuedSession)
            chipSelectConfigurator->StartSession();
        continuedSession = nextAction == SpiAction::continueSession;

        if (!sendData.empty())
            quadSpi.SendData(QuadSpi::Header{ std::nullopt, {}, {}, 0 }, sendData, QuadSpi::Lines::SingleSpeed(), [this]()
                {
                    Done();
                });
        else
            quadSpi.ReceiveData(QuadSpi::Header{ std::nullopt, {}, {}, 0 }, receiveData, QuadSpi::Lines::SingleSpeed(), [this]()
                {
                    Done();
                });
    }

    void SingleSpeedQuadSpiStmDma::SetChipSelectConfigurator(ChipSelectConfigurator& configurator)
    {
        chipSelectConfigurator = &configurator;
    }

    void SingleSpeedQuadSpiStmDma::SetCommunicationConfigurator(CommunicationConfigurator& configurator)
    {
        communicationConfigurator = &configurator;
        communicationConfigurator->ActivateConfiguration();
    }

    void SingleSpeedQuadSpiStmDma::ResetCommunicationConfigurator()
    {
        if (communicationConfigurator)
            communicationConfigurator->DeactivateConfiguration();

        communicationConfigurator = nullptr;
    }

    void SingleSpeedQuadSpiStmDma::Done()
    {
        if (chipSelectConfigurator && !continuedSession)
            chipSelectConfigurator->EndSession();
        onDone();
    }
}

#endif
