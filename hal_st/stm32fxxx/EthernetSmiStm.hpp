#ifndef HAL_ETHERNET_SMI_STM_HPP
#define HAL_ETHERNET_SMI_STM_HPP

#include "hal/interfaces/Ethernet.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/timer/Timer.hpp"
#include "infra/util/Sequencer.hpp"

namespace hal
{
    class EthernetSmiStm
        : public EthernetSmi
    {
    public:
        EthernetSmiStm(hal::GpioPinStm& ethernetMdio, hal::GpioPinStm& ethernetMdc, hal::GpioPinStm& ethernetRmiiRefClk, hal::GpioPinStm& ethernetRmiiCrsDv,
            hal::GpioPinStm& ethernetRmiiRxD0, hal::GpioPinStm& ethernetRmiiRxD1, hal::GpioPinStm& ethernetRmiiTxEn,
            hal::GpioPinStm& ethernetRmiiTxD0, hal::GpioPinStm& ethernetRmiiTxD1, uint16_t phyAddress = 0);
        ~EthernetSmiStm();

    public:
        uint16_t PhyAddress() const override;

    private:
        void RunPhy();
        void SetMiiClockRange();
        void ResetPhy();
        void DetectLink();
        uint16_t ReadPhyRegister(uint16_t reg);
        void WritePhyRegister(uint16_t reg, uint16_t value);
        void Delay(infra::Duration duration);

    private:
        hal::PeripheralPinStm ethernetMdio;
        hal::PeripheralPinStm ethernetMdc;
        hal::PeripheralPinStm ethernetRmiiRefClk;
        hal::PeripheralPinStm ethernetRmiiCrsDv;
        hal::PeripheralPinStm ethernetRmiiRxD0;
        hal::PeripheralPinStm ethernetRmiiRxD1;
        hal::PeripheralPinStm ethernetRmiiTxEn;
        hal::PeripheralPinStm ethernetRmiiTxD0;
        hal::PeripheralPinStm ethernetRmiiTxD1;
        uint16_t phyAddress;

        infra::Sequencer sequencer;
        uint32_t phyRegisterValue;
        infra::TimerSingleShot delayTimer;
        infra::Duration delay;

        bool linkUp = false;

        static const uint16_t phyBasicControlRegister = 0;
        static const uint16_t phyBasicStatusRegister = 1;
        static const uint16_t phyAutoNegotiationAdvertisement = 4;
        static const uint16_t phyAutoNegotiationLinkPartnerAbility = 5;

        static const uint16_t phyBcrDuplexMode = 8;
        static const uint16_t phyBcrRestartAutoNegotiation = 9;
        static const uint16_t phyBcrAutoNegotiationEnable = 12;
        static const uint16_t phyBcrSpeedSelect = 13;
        static const uint16_t phyBcrReset = 15;

        static const uint16_t phyBsrLinkUp = 2;
        static const uint16_t phyBsrAutoNegotiationComplete = 5;

        static const uint16_t phyAnlpaHalfDuplex10MHz = 5;
        static const uint16_t phyAnlpaFullDuplex10MHz = 6;
        static const uint16_t phyAnlpaHalfDuplex100MHz = 7;
        static const uint16_t phyAnlpaFullDuplex100MHz = 8;
    };
}

#endif
