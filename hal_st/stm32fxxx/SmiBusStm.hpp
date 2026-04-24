#ifndef HAL_SMI_BUS_STM_HPP
#define HAL_SMI_BUS_STM_HPP

#include "hal/interfaces/SmiBus.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include <cstdint>

namespace hal
{
    class SmiBusStm
        : public SmiBus
    {
    public:
        SmiBusStm(GpioPinStm& ethernetMdio, GpioPinStm& ethernetMdc, GpioPinStm& ethernetRmiiRefClk,
            GpioPinStm& ethernetRmiiCrsDv, GpioPinStm& ethernetRmiiRxD0, GpioPinStm& ethernetRmiiRxD1,
            GpioPinStm& ethernetRmiiTxEn, GpioPinStm& ethernetRmiiTxD0, GpioPinStm& ethernetRmiiTxD1);
        ~SmiBusStm() override;

        uint16_t Read(uint8_t phyAddress, uint16_t reg) const override;
        void Write(uint8_t phyAddress, uint16_t reg, uint16_t value) override;

        void SetMiiClockRange();

    private:
        PeripheralPinStm ethernetMdio;
        PeripheralPinStm ethernetMdc;
        PeripheralPinStm ethernetRmiiRefClk;
        PeripheralPinStm ethernetRmiiCrsDv;
        PeripheralPinStm ethernetRmiiRxD0;
        PeripheralPinStm ethernetRmiiRxD1;
        PeripheralPinStm ethernetRmiiTxEn;
        PeripheralPinStm ethernetRmiiTxD0;
        PeripheralPinStm ethernetRmiiTxD1;
    };
}

#endif
