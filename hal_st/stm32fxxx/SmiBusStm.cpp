#include "hal_st/stm32fxxx/SmiBusStm.hpp"
#include DEVICE_HEADER
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include <cstdlib>

#if defined(HAS_PERIPHERAL_ETHERNET)

namespace hal
{
    SmiBusStm::SmiBusStm(GpioPinStm& ethernetMdio, GpioPinStm& ethernetMdc, GpioPinStm& ethernetRmiiRefClk,
        GpioPinStm& ethernetRmiiCrsDv, GpioPinStm& ethernetRmiiRxD0, GpioPinStm& ethernetRmiiRxD1,
        GpioPinStm& ethernetRmiiTxEn, GpioPinStm& ethernetRmiiTxD0, GpioPinStm& ethernetRmiiTxD1)
        : ethernetMdio(ethernetMdio, PinConfigTypeStm::ethernet, 0)
        , ethernetMdc(ethernetMdc, PinConfigTypeStm::ethernet, 0)
        , ethernetRmiiRefClk(ethernetRmiiRefClk, PinConfigTypeStm::ethernet, 0)
        , ethernetRmiiCrsDv(ethernetRmiiCrsDv, PinConfigTypeStm::ethernet, 0)
        , ethernetRmiiRxD0(ethernetRmiiRxD0, PinConfigTypeStm::ethernet, 0)
        , ethernetRmiiRxD1(ethernetRmiiRxD1, PinConfigTypeStm::ethernet, 0)
        , ethernetRmiiTxEn(ethernetRmiiTxEn, PinConfigTypeStm::ethernet, 0)
        , ethernetRmiiTxD0(ethernetRmiiTxD0, PinConfigTypeStm::ethernet, 0)
        , ethernetRmiiTxD1(ethernetRmiiTxD1, PinConfigTypeStm::ethernet, 0)
    {
#if !defined(STM32H5)
        __HAL_RCC_SYSCFG_CLK_ENABLE();
        SYSCFG->PMC |= SYSCFG_PMC_MII_RMII_SEL; // Select RMII Mode
#else
        __HAL_RCC_SBS_CLK_ENABLE();
        SBS->PMCR |= SBS_ETH_RMII; // Select RMII Mode
        (void)SBS->PMCR;           // Dummy read to sync with ETH
#endif

        EnableClockEthernet(0);
        SetMiiClockRange();
    }

    SmiBusStm::~SmiBusStm()
    {
        DisableClockEthernet(0);
    }

    void SmiBusStm::SetMiiClockRange()
    {
        const uint32_t hclk = HAL_RCC_GetHCLKFreq();

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

    uint16_t SmiBusStm::Read(uint8_t phyAddress, uint16_t reg) const
    {
#if defined(STM32H5)
        peripheralEthernet[0]->MACMDIOAR = (peripheralEthernet[0]->MACMDIOAR & ETH_MACMDIOAR_CR) |
                                           ((static_cast<uint32_t>(phyAddress) << 21) & ETH_MACMDIOAR_PA) |
                                           ((static_cast<uint32_t>(reg) << 16) & ETH_MACMDIOAR_RDA) |
                                           ETH_MACMDIOAR_MOC_RD | ETH_MACMDIOAR_MB;

        while ((peripheralEthernet[0]->MACMDIOAR & ETH_MACMDIOAR_MB) != 0)
        {
        }

        return static_cast<uint16_t>(peripheralEthernet[0]->MACMDIODR);
#else
        peripheralEthernet[0]->MACMIIAR = (peripheralEthernet[0]->MACMIIAR & ETH_MACMIIAR_CR) |
                                          ((static_cast<uint32_t>(phyAddress) << 11) & ETH_MACMIIAR_PA) |
                                          ((static_cast<uint32_t>(reg) << 6) & ETH_MACMIIAR_MR) | ETH_MACMIIAR_MB;

        while ((peripheralEthernet[0]->MACMIIAR & ETH_MACMIIAR_MB) != 0)
        {
        }

        return static_cast<uint16_t>(peripheralEthernet[0]->MACMIIDR);
#endif
    }

    void SmiBusStm::Write(uint8_t phyAddress, uint16_t reg, uint16_t value)
    {
#if defined(STM32H5)
        peripheralEthernet[0]->MACMDIODR = value;
        peripheralEthernet[0]->MACMDIOAR = (peripheralEthernet[0]->MACMDIOAR & ETH_MACMDIOAR_CR) |
                                           ((static_cast<uint32_t>(phyAddress) << 21) & ETH_MACMDIOAR_PA) |
                                           ((static_cast<uint32_t>(reg) << 16) & ETH_MACMDIOAR_RDA) |
                                           ETH_MACMDIOAR_MOC_WR | ETH_MACMDIOAR_MB;

        while ((peripheralEthernet[0]->MACMDIOAR & ETH_MACMDIOAR_MB) != 0)
        {
        }
#else
        peripheralEthernet[0]->MACMIIDR = value;
        peripheralEthernet[0]->MACMIIAR = (peripheralEthernet[0]->MACMIIAR & ETH_MACMIIAR_CR) |
                                          ((static_cast<uint32_t>(phyAddress) << 11) & ETH_MACMIIAR_PA) |
                                          ((static_cast<uint32_t>(reg) << 6) & ETH_MACMIIAR_MR) | ETH_MACMIIAR_MW | ETH_MACMIIAR_MB;

        while ((peripheralEthernet[0]->MACMIIAR & ETH_MACMIIAR_MB) != 0)
        {
        }
#endif
    }
}

#endif
