#include "hal_st/stm32fxxx/EthernetSmiStm.hpp"
#include DEVICE_HEADER
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/util/BitLogic.hpp"

#if defined(HAS_PERIPHERAL_ETHERNET)

namespace hal
{
    EthernetSmiStm::EthernetSmiStm(hal::GpioPinStm& ethernetMdio, hal::GpioPinStm& ethernetMdc, hal::GpioPinStm& ethernetRmiiRefClk,
        hal::GpioPinStm& ethernetRmiiCrsDv, hal::GpioPinStm& ethernetRmiiRxD0, hal::GpioPinStm& ethernetRmiiRxD1,
        hal::GpioPinStm& ethernetRmiiTxEn, hal::GpioPinStm& ethernetRmiiTxD0, hal::GpioPinStm& ethernetRmiiTxD1, uint16_t phyAddress)
        : smiBus(ethernetMdio, ethernetMdc, ethernetRmiiRefClk, ethernetRmiiCrsDv, ethernetRmiiRxD0, ethernetRmiiRxD1, ethernetRmiiTxEn, ethernetRmiiTxD0, ethernetRmiiTxD1)
        , phyAddress(phyAddress)
    {
        RunPhy();
    }

    EthernetSmiStm::~EthernetSmiStm() = default;

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
                WritePhyRegister(phyBasicControlRegister, infra::Bit<uint16_t>(phyBcrReset));
            });
        sequencer.DoWhile();
        Delay(std::chrono::milliseconds(1));
        sequencer.Execute([this]()
            {
                phyControlRegisterValue = ReadPhyRegister(phyBasicControlRegister);
            });
        sequencer.EndDoWhile([this]()
            {
                return infra::IsBitSet(phyControlRegisterValue, phyBcrReset);
            });
        sequencer.Execute([this]()
            {
                uint16_t status = ReadPhyRegister(phyBasicStatusRegister);
                autoNegotiation = infra::IsBitSet(status, phyBsrAutoNegotiationAbility);
                if (autoNegotiation)
                    WritePhyRegister(phyBasicControlRegister, infra::Bit<uint16_t>(phyBcrAutoNegotiationEnable));
            });
    }

    void EthernetSmiStm::DetectLink()
    {
        sequencer.Execute([this]()
            {
                uint16_t status = ReadPhyRegister(phyBasicStatusRegister);
                bool autoNegotiationComplete = autoNegotiation ? infra::IsBitSet(status, phyBsrAutoNegotiationComplete) : true;
                bool newLinkUp = infra::IsBitSet(status, phyBsrLinkUp) && autoNegotiationComplete;

                if (newLinkUp != linkUp)
                {
                    linkUp = newLinkUp;

                    if (linkUp)
                    {
                        LinkSpeed speed = autoNegotiation ? GetLinkSpeedNegotiated() : GetLinkSpeedLocal();
                        GetObserver().LinkUp(speed);
                    }
                    else
                    {
                        GetObserver().LinkDown();
                        smiBus.SetMiiClockRange();
                    }
                }
            });
    }

    LinkSpeed EthernetSmiStm::GetLinkSpeedNegotiated() const
    {
        uint16_t linkAbility = ReadPhyRegister(phyAutoNegotiationAdvertisement);
        uint16_t linkPartnerAbility = ReadPhyRegister(phyAutoNegotiationLinkPartnerAbility);
        if (infra::IsBitSet(linkPartnerAbility, phyAnlpaFullDuplex100MHz) && infra::IsBitSet(linkAbility, phyAnlpaFullDuplex100MHz))
            return LinkSpeed::fullDuplex100MHz;
        else if (infra::IsBitSet(linkPartnerAbility, phyAnlpaHalfDuplex100MHz) && infra::IsBitSet(linkAbility, phyAnlpaHalfDuplex100MHz))
            return LinkSpeed::halfDuplex100MHz;
        else if (infra::IsBitSet(linkPartnerAbility, phyAnlpaFullDuplex10MHz) && infra::IsBitSet(linkAbility, phyAnlpaFullDuplex10MHz))
            return LinkSpeed::fullDuplex10MHz;
        else
            return LinkSpeed::halfDuplex10MHz;
    }

    LinkSpeed EthernetSmiStm::GetLinkSpeedLocal() const
    {
        bool duplexMode = infra::IsBitSet(phyControlRegisterValue, phyBcrDuplexMode);
        if (infra::IsBitSet(phyControlRegisterValue, phyBcrSpeedSelectLsb) && duplexMode)
            return LinkSpeed::fullDuplex100MHz;
        else if (infra::IsBitSet(phyControlRegisterValue, phyBcrSpeedSelectLsb) && !duplexMode)
            return LinkSpeed::halfDuplex100MHz;
        else if (!infra::IsBitSet(phyControlRegisterValue, phyBcrSpeedSelectLsb) && duplexMode)
            return LinkSpeed::fullDuplex10MHz;
        else
            return LinkSpeed::halfDuplex10MHz;
    }

    uint16_t EthernetSmiStm::ReadPhyRegister(uint16_t reg) const
    {
        return smiBus.Read(static_cast<uint8_t>(phyAddress), reg);
    }

    void EthernetSmiStm::WritePhyRegister(uint16_t reg, uint16_t value)
    {
        smiBus.Write(static_cast<uint8_t>(phyAddress), reg, value);
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
