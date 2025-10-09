#ifndef HAL_ST_ETHERNET_SMI_OBSERVER_HPP
#define HAL_ST_ETHERNET_SMI_OBSERVER_HPP

#include "hal/interfaces/Ethernet.hpp"
#include "hal_st/stm32fxxx/EthernetMacStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "lwip/lwip_cpp/LightweightIpOverEthernet.hpp"

namespace main_
{
#ifdef STM32F767xx
    class EthernetStm32f767Workaround
        : public hal::EthernetMac
        , private hal::EthernetMacObserver
    {
    public:
        EthernetStm32f767Workaround(hal::EthernetMac& subject, hal::GpioPinStm& ethernetRmiiRefClk, std::optional<hal::PeripheralPinStm>& ethernetRmiiRefClkPeripheral)
            : hal::EthernetMacObserver(subject)
            , ethernetRmiiRefClk(ethernetRmiiRefClk)
            , ethernetRmiiRefClkPeripheral(ethernetRmiiRefClkPeripheral)
        {}

    public:
        void SendBuffer(infra::ConstByteRange data, bool last) override
        {
            hal::EthernetMacObserver::Subject().SendBuffer(data, last);
        }

        void RetryAllocation() override
        {
            hal::EthernetMacObserver::Subject().RetryAllocation();
        }

        void AddMacAddressFilter(hal::MacAddress address) override
        {
            hal::EthernetMacObserver::Subject().AddMacAddressFilter(address);
        }

        void RemoveMacAddressFilter(hal::MacAddress address) override
        {
            hal::EthernetMacObserver::Subject().RemoveMacAddressFilter(address);
        }

    private:
        infra::ByteRange RequestReceiveBuffer() override
        {
            return GetObserver().RequestReceiveBuffer();
        }

        void ReceivedFrame(uint32_t usedBuffers, uint32_t frameSize) override
        {
            GetObserver().ReceivedFrame(usedBuffers, frameSize);
        }

        void ReceivedErrorFrame(uint32_t usedBuffers, uint32_t frameSize) override
        {
            ethernetRmiiRefClkPeripheral.reset();
            ethernetRmiiRefClkPeripheral.emplace(ethernetRmiiRefClk, hal::PinConfigTypeStm::ethernet, 0);
            GetObserver().ReceivedErrorFrame(usedBuffers, frameSize);
        }

        void SentFrame() override
        {
            GetObserver().SentFrame();
        }

    private:
        hal::GpioPinStm& ethernetRmiiRefClk;
        std::optional<hal::PeripheralPinStm>& ethernetRmiiRefClkPeripheral;
    };
#endif

    class EthernetSmiObserver
        : public hal::EthernetSmiObserver
    {
    public:
        EthernetSmiObserver(hal::EthernetSmi& ethernetSmi, services::LightweightIpOverEthernetFactory& lightweightIpOverEthernetFactory, hal::GpioPinStm& ethernetRmiiRefClk);
        ~EthernetSmiObserver();

        void LinkUp(hal::LinkSpeed linkSpeed) override;
        void LinkDown() override;

    private:
        services::LightweightIpOverEthernetFactory& lightweightIpOverEthernetFactory;
        hal::GpioPinStm& ethernetRmiiRefClk;
        std::optional<hal::PeripheralPinStm> ethernetRmiiRefClkPeripheral;

#ifdef STM32F767xx
        std::optional<EthernetStm32f767Workaround> ethernetStm32f767Workaround;
#endif
        std::optional<hal::EthernetMacStm> ethernetMac;
    };
}

#endif
