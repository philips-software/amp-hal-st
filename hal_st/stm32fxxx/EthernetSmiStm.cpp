#include "hal_st/stm32fxxx/EthernetSmiStm.hpp"
#include DEVICE_HEADER
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/util/BitLogic.hpp"

#if defined(HAS_PERIPHERAL_ETHERNET)

namespace hal
{
    EthernetSmiStm::EthernetSmiStm(hal::GpioPinStm& ethernetMdio, hal::GpioPinStm& ethernetMdc, hal::GpioPinStm& ethernetRmiiRefClk,
        hal::GpioPinStm& ethernetRmiiCrsDv, hal::GpioPinStm& ethernetRmiiRxD0, hal::GpioPinStm& ethernetRmiiRxD1,
        hal::GpioPinStm& ethernetRmiiTxEn, hal::GpioPinStm& ethernetRmiiTxD0, hal::GpioPinStm& ethernetRmiiTxD1, uint8_t phyAddress)
        : smiBus(ethernetMdio, ethernetMdc, ethernetRmiiRefClk, ethernetRmiiCrsDv, ethernetRmiiRxD0, ethernetRmiiRxD1, ethernetRmiiTxEn, ethernetRmiiTxD0, ethernetRmiiTxD1)
        , phyAddress(phyAddress)
        , phy_(smiBus, phyAddress)
    {
        RunPhy();
    }

    uint16_t EthernetSmiStm::PhyAddress() const
    {
        return phyAddress;
    }

    void EthernetSmiStm::RunPhy()
    {
        sequencer.Load([this]()
            {
                ResetPhy();
                sequencer.While([]()
                    {
                        return true;
                    });
                DetectLink();
                Delay(std::chrono::milliseconds(200));
                sequencer.EndWhile();
            });
    }

    void EthernetSmiStm::ResetPhy()
    {
        sequencer.Execute([this]()
            {
                phy_.WriteBcr(infra::Bit<uint16_t>(services::SmiPhy::BasicControlRegister::ResetBit));
            });
        sequencer.DoWhile();
        Delay(std::chrono::milliseconds(1));
        sequencer.Execute([this]()
            {
                savedBcr_ = phy_.ReadBcr();
            });
        sequencer.EndDoWhile([this]()
            {
                return services::SmiPhy::BasicControlRegister::IsResetting(savedBcr_);
            });
        sequencer.Execute([this]()
            {
                phy_.EnableAutoNegIfCapable();
            });
    }

    void EthernetSmiStm::DetectLink()
    {
        sequencer.Execute([this]()
            {
                const auto linkState = phy_.ReadLinkState();
                if (linkState == services::SmiPhy::LinkState::Up)
                    GetObserver().LinkUp(phy_.LinkSpeed());
                else if (linkState == services::SmiPhy::LinkState::Down)
                {
                    GetObserver().LinkDown();
                    smiBus.SetMiiClockRange();
                }
            });
    }

    void EthernetSmiStm::Delay(infra::Duration duration)
    {
        delay = duration;
        sequencer.Step([this]()
            {
                delayTimer.Start(delay, [this]()
                    {
                        sequencer.Continue();
                    });
            });
    }
}

#endif
