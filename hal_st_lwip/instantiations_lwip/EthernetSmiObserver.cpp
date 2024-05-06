#include "hal_st_lwip/instantiations_lwip/EthernetSmiObserver.hpp"

namespace main_
{
    EthernetSmiObserver::EthernetSmiObserver(hal::EthernetSmi& ethernetSmi, services::LightweightIpOverEthernetFactory& lightweightIpOverEthernetFactory, hal::GpioPinStm& ethernetRmiiRefClk)
        : hal::EthernetSmiObserver(ethernetSmi)
        , lightweightIpOverEthernetFactory(lightweightIpOverEthernetFactory)
        , ethernetRmiiRefClk(ethernetRmiiRefClk)
        , ethernetRmiiRefClkPeripheral(std::in_place, ethernetRmiiRefClk, hal::PinConfigTypeStm::ethernet, 0)
    {}

    EthernetSmiObserver::~EthernetSmiObserver()
    {
        if (ethernetMac)
            LinkDown();
    }

    void EthernetSmiObserver::LinkUp(hal::LinkSpeed linkSpeed)
    {
        ethernetMac.emplace(Subject(), linkSpeed, lightweightIpOverEthernetFactory.MacAddress());
#if STM32F767xx
        ethernetStm32f767Workaround.emplace(*ethernetMac, ethernetRmiiRefClk, ethernetRmiiRefClkPeripheral);
        lightweightIpOverEthernetFactory.Create(*ethernetStm32f767Workaround);
#else
        lightweightIpOverEthernetFactory.Create(*ethernetMac);
#endif
    }

    void EthernetSmiObserver::LinkDown()
    {
        lightweightIpOverEthernetFactory.Destroy();
#if STM32F767xx
        ethernetStm32f767Workaround = std::nullopt;
#endif
        ethernetMac = std::nullopt;
    }
}
