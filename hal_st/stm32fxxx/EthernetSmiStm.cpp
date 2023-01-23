#include "hal_st/stm32fxxx/EthernetSmiStm.hpp"
#include "cmsis_device.h"
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
        __HAL_RCC_SYSCFG_CLK_ENABLE();
        SYSCFG->PMC |= SYSCFG_PMC_MII_RMII_SEL; // Select RMII Mode

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
            sequencer.While([]() { return true; });
                DetectLink();
                Delay(std::chrono::milliseconds(200));
            sequencer.EndWhile(); });
    }

    void EthernetSmiStm::SetMiiClockRange()
    {
        uint32_t hclk = HAL_RCC_GetHCLKFreq();

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
    }

    void EthernetSmiStm::ResetPhy()
    {
        sequencer.Execute([this]()
            { WritePhyRegister(phyBasicControlRegister, infra::Bit<uint16_t>(phyBcrReset)); });
        sequencer.DoWhile();
        Delay(std::chrono::milliseconds(1));
        sequencer.Execute([this]()
            { phyRegisterValue = ReadPhyRegister(phyBasicControlRegister); });
        sequencer.EndDoWhile([this]()
            { return infra::IsBitSet(phyRegisterValue, phyBcrReset); });
        sequencer.Execute([this]()
            { WritePhyRegister(phyBasicControlRegister, infra::Bit<uint16_t>(phyBcrAutoNegotiationEnable)); });
    }

    void EthernetSmiStm::DetectLink()
    {
        sequencer.Execute([this]()
            {
            uint16_t status = ReadPhyRegister(phyBasicStatusRegister);
            bool newLinkUp = infra::IsBitSet(status, phyBsrLinkUp) && infra::IsBitSet(status, phyBsrAutoNegotiationComplete);

            if (newLinkUp != linkUp)
            {
                linkUp = newLinkUp;

                if (linkUp)
                {
                    uint16_t linkAbility = ReadPhyRegister(phyAutoNegotiationAdvertisement);
                    uint16_t linkPartnerAbility = ReadPhyRegister(phyAutoNegotiationLinkPartnerAbility);
                    LinkSpeed speed;
                    if (infra::IsBitSet(linkPartnerAbility, phyAnlpaFullDuplex100MHz) && infra::IsBitSet(linkAbility, phyAnlpaFullDuplex100MHz))
                        speed = LinkSpeed::fullDuplex100MHz;
                    else if (infra::IsBitSet(linkPartnerAbility, phyAnlpaHalfDuplex100MHz) && infra::IsBitSet(linkAbility, phyAnlpaHalfDuplex100MHz))
                        speed = LinkSpeed::halfDuplex100MHz;
                    else if (infra::IsBitSet(linkPartnerAbility, phyAnlpaFullDuplex10MHz) && infra::IsBitSet(linkAbility, phyAnlpaFullDuplex10MHz))
                        speed = LinkSpeed::fullDuplex10MHz;
                    else
                        speed = LinkSpeed::halfDuplex10MHz;

                    GetObserver().LinkUp(speed);
                }
                else
                    GetObserver().LinkDown();
            } });
    }

    uint16_t EthernetSmiStm::ReadPhyRegister(uint16_t reg)
    {
        peripheralEthernet[0]->MACMIIAR = (peripheralEthernet[0]->MACMIIAR & ~ETH_MACMIIAR_CR_MASK) | ((static_cast<uint32_t>(phyAddress) << 11) & ETH_MACMIIAR_PA) | ((static_cast<uint32_t>(reg) << 6) & ETH_MACMIIAR_MR) | ETH_MACMIIAR_MB;

        while (peripheralEthernet[0]->MACMIIAR & ETH_MACMIIAR_MB != 0)
        {}

        return peripheralEthernet[0]->MACMIIDR;
    }

    void EthernetSmiStm::WritePhyRegister(uint16_t reg, uint16_t value)
    {
        peripheralEthernet[0]->MACMIIDR = value;
        peripheralEthernet[0]->MACMIIAR = (peripheralEthernet[0]->MACMIIAR & ~ETH_MACMIIAR_CR_MASK) | ((static_cast<uint32_t>(phyAddress) << 11) & ETH_MACMIIAR_PA) | ((static_cast<uint32_t>(reg) << 6) & ETH_MACMIIAR_MR) | ETH_MACMIIAR_MW | ETH_MACMIIAR_MB;

        while (peripheralEthernet[0]->MACMIIAR & ETH_MACMIIAR_MB != 0)
        {}
    }

    void EthernetSmiStm::Delay(infra::Duration duration)
    {
        delay = duration;
        sequencer.Step([this]()
            { delayTimer.Start(delay, [this]()
                  { sequencer.Continue(); }); });
    }
}

#endif
