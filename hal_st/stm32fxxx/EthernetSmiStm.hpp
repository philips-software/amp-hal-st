#ifndef HAL_ETHERNET_SMI_STM_HPP
#define HAL_ETHERNET_SMI_STM_HPP

#include "hal/interfaces/Ethernet.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/SmiBusStm.hpp"
#include "infra/timer/Timer.hpp"
#include "infra/util/Sequencer.hpp"
#include "services/util/SmiPhy.hpp"

namespace hal
{
    class EthernetSmiStm
        : public EthernetSmi
    {
    public:
        EthernetSmiStm(hal::GpioPinStm& ethernetMdio, hal::GpioPinStm& ethernetMdc, hal::GpioPinStm& ethernetRmiiRefClk, hal::GpioPinStm& ethernetRmiiCrsDv,
            hal::GpioPinStm& ethernetRmiiRxD0, hal::GpioPinStm& ethernetRmiiRxD1, hal::GpioPinStm& ethernetRmiiTxEn,
            hal::GpioPinStm& ethernetRmiiTxD0, hal::GpioPinStm& ethernetRmiiTxD1, uint8_t phyAddress = 0);

    public:
        uint16_t PhyAddress() const override;

    private:
        void RunPhy();
        void ResetPhy();
        void DetectLink();
        void Delay(infra::Duration duration);

    private:
        SmiBusStm smiBus;
        uint8_t phyAddress;
        services::SmiPhy phy_;

        infra::Sequencer sequencer;
        uint16_t savedBcr_ = 0;
        infra::TimerSingleShot delayTimer;
        infra::Duration delay;
    };
}

#endif
