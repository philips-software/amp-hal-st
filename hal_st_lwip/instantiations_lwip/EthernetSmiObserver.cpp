#include "hal_st_lwip/instantiations_lwip/EthernetSmiObserver.hpp"

namespace main_
{
    EthernetSmiObserver::EthernetSmiObserver(hal::EthernetSmi& ethernetSmi, services::LightweightIpOverEthernetFactory& lightweightIpOverEthernetFactory, hal::GpioPinStm& ethernetRmiiRefClk)
        : hal::EthernetSmiObserver(ethernetSmi)
        , lightweightIpOverEthernetFactory(lightweightIpOverEthernetFactory)
        , ethernetRmiiRefClk(ethernetRmiiRefClk)
        , ethernetRmiiRefClkPeripheral(infra::inPlace, ethernetRmiiRefClk, hal::PinConfigTypeStm::ethernet, 0)
    {}

    EthernetSmiObserver::~EthernetSmiObserver()
    {
        if (ethernetMac)
            LinkDown();
    }

    void EthernetSmiObserver::LinkUp(hal::LinkSpeed linkSpeed)
    {
        ethernetMac.Emplace(Subject(), linkSpeed, lightweightIpOverEthernetFactory.MacAddress());
#if STM32F767xx
        ethernetStm32f767Workaround.Emplace(*ethernetMac, ethernetRmiiRefClk, ethernetRmiiRefClkPeripheral);
        lightweightIpOverEthernetFactory.Create(*ethernetStm32f767Workaround);
#else
        lightweightIpOverEthernetFactory.Create(*ethernetMac);
#endif
    }

    void EthernetSmiObserver::LinkDown()
    {
        lightweightIpOverEthernetFactory.Destroy();
#if STM32F767xx
        ethernetStm32f767Workaround = infra::none;
#endif
        ethernetMac = infra::none;
    }
}
