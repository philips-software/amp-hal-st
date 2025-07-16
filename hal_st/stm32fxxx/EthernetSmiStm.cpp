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
        : ethernetMdio(ethernetMdio, hal::PinConfigTypeStm::ethernet, 0)
        , ethernetMdc(ethernetMdc, hal::PinConfigTypeStm::ethernet, 0)
        , ethernetRmiiRefClk(ethernetRmiiRefClk, hal::PinConfigTypeStm::ethernet, 0)
        , ethernetRmiiCrsDv(ethernetRmiiCrsDv, hal::PinConfigTypeStm::ethernet, 0)
        , ethernetRmiiRxD0(ethernetRmiiRxD0, hal::PinConfigTypeStm::ethernet, 0)
        , ethernetRmiiRxD1(ethernetRmiiRxD1, hal::PinConfigTypeStm::ethernet, 0)
        , ethernetRmiiTxEn(ethernetRmiiTxEn, hal::PinConfigTypeStm::ethernet, 0)
        , ethernetRmiiTxD0(ethernetRmiiTxD0, hal::PinConfigTypeStm::ethernet, 0)
        , ethernetRmiiTxD1(ethernetRmiiTxD1, hal::PinConfigTypeStm::ethernet, 0)
        , phyAddress(phyAddress)
    {
#if !defined(STM32H5)
        __HAL_RCC_SYSCFG_CLK_ENABLE();
        SYSCFG->PMC |= SYSCFG_PMC_MII_RMII_SEL; // Select RMII Mode
#endif

        EnableClockEthernet(0);

        SetMiiClockRange();
        RunPhy();
    }

    EthernetSmiStm::~EthernetSmiStm()
    {
        DisableClockEthernet(0);
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

    void EthernetSmiStm::SetMiiClockRange()
    {
        uint32_t hclk = HAL_RCC_GetHCLKFreq();

#if defined(STM32H5)
        if (hclk >= 20000000 && hclk < 35000000)
            peripheralEthernet[0]->MACMDIOAR = ETH_MACMDIOAR_CR_DIV16;
        else if (hclk >= 35000000 && hclk < 60000000)
            peripheralEthernet[0]->MACMDIOAR = ETH_MACMDIOAR_CR_DIV26;
        else if (hclk >= 60000000 && hclk < 100000000)
            peripheralEthernet[0]->MACMDIOAR = ETH_MACMDIOAR_CR_DIV42;
        else if (hclk >= 100000000 && hclk < 150000000)
            peripheralEthernet[0]->MACMDIOAR = ETH_MACMDIOAR_CR_DIV62;
        else if (hclk >= 150000000 && hclk <= 250000000)
            peripheralEthernet[0]->MACMDIOAR = ETH_MACMDIOAR_CR_DIV102;
        else
            std::abort();
#else
        if (hclk >= 20000000 && hclk < 35000000)
            peripheralEthernet[0]->MACMIIAR = ETH_MACMIIAR_CR_Div16;
        else if (hclk >= 35000000 && hclk < 60000000)
            peripheralEthernet[0]->MACMIIAR = ETH_MACMIIAR_CR_Div26;
        else if (hclk >= 60000000 && hclk < 100000000)
            peripheralEthernet[0]->MACMIIAR = ETH_MACMIIAR_CR_Div42;
        else if (hclk >= 100000000 && hclk < 150000000)
            peripheralEthernet[0]->MACMIIAR = ETH_MACMIIAR_CR_Div62;
        else if (hclk >= 150000000 && hclk <= 216000000)
            peripheralEthernet[0]->MACMIIAR = ETH_MACMIIAR_CR_Div102;
        else
            std::abort();
#endif
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
                autoNego = infra::IsBitSet(status, phyBsrAutoNegotiationAbility);
                if (autoNego)
                    WritePhyRegister(phyBasicControlRegister, infra::Bit<uint16_t>(phyBcrAutoNegotiationEnable));
            });
    }

    void EthernetSmiStm::DetectLink()
    {
        sequencer.Execute([this]()
            {
                uint16_t status = ReadPhyRegister(phyBasicStatusRegister);
                bool autoNegoComplete = autoNego ? infra::IsBitSet(status, phyBsrAutoNegotiationComplete) : true;
                bool newLinkUp = infra::IsBitSet(status, phyBsrLinkUp) && autoNegoComplete;

                if (newLinkUp != linkUp)
                {
                    linkUp = newLinkUp;

                    if (linkUp)
                    {
                        LinkSpeed speed = autoNego ? GetLinkSpeedNegotiated() : GetLinkSpeedLocal();
                        GetObserver().LinkUp(speed);
                    }
                    else
                        GetObserver().LinkDown();
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
#if defined(STM32H5)
        peripheralEthernet[0]->MACMDIOAR = (peripheralEthernet[0]->MACMDIOAR & ETH_MACMDIOAR_CR) | ((static_cast<uint32_t>(phyAddress) << 21) & ETH_MACMDIOAR_PA) | ((static_cast<uint32_t>(reg) << 16) & ETH_MACMDIOAR_RDA) | ETH_MACMDIOAR_MOC_RD | ETH_MACMDIOAR_MB;

        while ((peripheralEthernet[0]->MACMDIOAR & ETH_MACMDIOAR_MB) != 0)
        {
        }

        return static_cast<uint16_t>(peripheralEthernet[0]->MACMDIODR);
#else
        peripheralEthernet[0]->MACMIIAR = (peripheralEthernet[0]->MACMIIAR & ETH_MACMIIAR_CR) | ((static_cast<uint32_t>(phyAddress) << 11) & ETH_MACMIIAR_PA) | ((static_cast<uint32_t>(reg) << 6) & ETH_MACMIIAR_MR) | ETH_MACMIIAR_MB;

        while (peripheralEthernet[0]->MACMIIAR & ETH_MACMIIAR_MB != 0)
        {
        }

        return peripheralEthernet[0]->MACMIIDR;
#endif
    }

    void EthernetSmiStm::WritePhyRegister(uint16_t reg, uint16_t value)
    {
#if defined(STM32H5)
        peripheralEthernet[0]->MACMDIODR = value;
        peripheralEthernet[0]->MACMDIOAR = (peripheralEthernet[0]->MACMDIOAR & ETH_MACMDIOAR_CR) | ((static_cast<uint32_t>(phyAddress) << 21) & ETH_MACMDIOAR_PA) | ((static_cast<uint32_t>(reg) << 16) & ETH_MACMDIOAR_RDA) | ETH_MACMDIOAR_MOC_WR | ETH_MACMDIOAR_MB;

        while ((peripheralEthernet[0]->MACMDIOAR & ETH_MACMDIOAR_MB) != 0)
        {
        }
#else
        peripheralEthernet[0]->MACMIIDR = value;
        peripheralEthernet[0]->MACMIIAR = (peripheralEthernet[0]->MACMIIAR & ETH_MACMIIAR_CR) | ((static_cast<uint32_t>(phyAddress) << 11) & ETH_MACMIIAR_PA) | ((static_cast<uint32_t>(reg) << 6) & ETH_MACMIIAR_MR) | ETH_MACMIIAR_MW | ETH_MACMIIAR_MB;

        while (peripheralEthernet[0]->MACMIIAR & ETH_MACMIIAR_MB != 0)
        {
        }
#endif
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
