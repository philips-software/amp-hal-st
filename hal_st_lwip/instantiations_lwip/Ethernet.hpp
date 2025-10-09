#ifndef HAL_ST_ETHERNET_HPP
#define HAL_ST_ETHERNET_HPP

#include "hal_st/stm32fxxx/EthernetSmiStm.hpp"
#include "hal_st_lwip/instantiations_lwip/EthernetSmiObserver.hpp"
#include "lwip/lwip_cpp/LightweightIp.hpp"
#include "services/network/LlmnrResponder.hpp"

namespace
{
    services::LightweightIpOverEthernetFactory::Config ConvertConfig(infra::BoundedString hostName)
    {
        services::LightweightIpOverEthernetFactory::Config config;
        config.hostName = hostName;
        return config;
    }
}

namespace main_
{
    template<std::size_t MaxListeners = 1, std::size_t MaxConnectors = 1, std::size_t MaxConnections = 5>
    struct Ethernet
    {
        Ethernet(infra::MemoryRange<const std::pair<hal::Port, uint8_t>> pins, hal::MacAddress macAddress, infra::BoundedString hostName, hal::SynchronousRandomDataGenerator& randomDataGenerator);
        Ethernet(infra::MemoryRange<const std::pair<hal::Port, uint8_t>> pins, hal::MacAddress macAddress, infra::BoundedString hostName, hal::SynchronousRandomDataGenerator& randomDataGenerator, infra::CreatorBase<services::Stoppable, void(services::LightweightIp& lightweightIp)>& connectedCreator);

        services::LightweightIp::WithFixedAllocator<MaxListeners, MaxConnectors, MaxConnections> lightweightIp;
        services::LightweightIpOverEthernetFactory::Config lightweightIpConfig;
        services::LightweightIpOverEthernetFactory lightweightIpOverEthernetFactory;
        infra::Execute setSysConfig;
        hal::GpioPinStm mdio;
        hal::GpioPinStm mdc;
        hal::GpioPinStm rmiiRefClk;
        hal::GpioPinStm rmiiCrsDv;
        hal::GpioPinStm rmiiRxD0;
        hal::GpioPinStm rmiiRxD1;
        hal::GpioPinStm rmiiTxEn;
        hal::GpioPinStm rmiiTxD0;
        hal::GpioPinStm rmiiTxD1;
        hal::EthernetSmiStm ethernetSmi;
        main_::EthernetSmiObserver ethernetSmiObserver;
        infra::BoundedString hostName;

        struct Connected
            : public services::Stoppable
        {
            Connected(services::LightweightIp& lightweightIp, infra::BoundedConstString hostName, infra::CreatorBase<services::Stoppable, void(services::LightweightIp& lightweightIp)>* connectedCreator)
                : llmnrResponder(lightweightIp, lightweightIp, lightweightIp, hostName)
            {
                if (connectedCreator != nullptr)
                    connected.emplace(*connectedCreator, lightweightIp);
            }

            void Stop(const infra::Function<void()>& onDone) override
            {
                if (connected != std::nullopt)
                    (*connected)->Stop(onDone);
                else
                    onDone;
            }

            services::LlmnrResponder llmnrResponder;
            std::optional<infra::ProxyCreator<services::Stoppable, void(services::LightweightIp& lightweightIp)>> connected;
        };

        infra::CreatorBase<services::Stoppable, void(services::LightweightIp& lightweightIp)>* connectedCreator = nullptr;
        infra::Creator<services::Stoppable, Connected, void(services::LightweightIp& lightweightIp)> connected{ [this](std::optional<Connected>& value, services::LightweightIp& lightweightIp)
            {
                value.emplace(lightweightIp, hostName, connectedCreator);
            } };
    };

    template<std::size_t MaxListeners, std::size_t MaxConnectors, std::size_t MaxConnections>
    Ethernet<MaxListeners, MaxConnectors, MaxConnections>::Ethernet(infra::MemoryRange<const std::pair<hal::Port, uint8_t>> pins, hal::MacAddress macAddress, infra::BoundedString hostName, hal::SynchronousRandomDataGenerator& randomDataGenerator)
        : lightweightIp(randomDataGenerator, connected)
        , lightweightIpConfig(ConvertConfig(hostName))
        , lightweightIpOverEthernetFactory(macAddress, lightweightIpConfig)
        , setSysConfig([]()
              {
                  SYSCFG->CMPCR |= SYSCFG_CMPCR_CMP_PD;
                  while ((SYSCFG->CMPCR & SYSCFG_CMPCR_READY) == 0)
                  {
                  }
              })
        , mdio(pins[0].first, pins[0].second)
        , mdc(pins[1].first, pins[1].second)
        , rmiiRefClk(pins[2].first, pins[2].second)
        , rmiiCrsDv(pins[3].first, pins[3].second)
        , rmiiRxD0(pins[4].first, pins[4].second)
        , rmiiRxD1(pins[5].first, pins[5].second)
        , rmiiTxEn(pins[6].first, pins[6].second)
        , rmiiTxD0(pins[7].first, pins[7].second)
        , rmiiTxD1(pins[8].first, pins[8].second)
        , ethernetSmi(mdio, mdc, hal::dummyPinStm, rmiiCrsDv, rmiiRxD0, rmiiRxD1, rmiiTxEn, rmiiTxD0, rmiiTxD1)
        , ethernetSmiObserver(ethernetSmi, lightweightIpOverEthernetFactory, rmiiRefClk)
        , hostName(hostName)
    {}

    template<std::size_t MaxListeners, std::size_t MaxConnectors, std::size_t MaxConnections>
    Ethernet<MaxListeners, MaxConnectors, MaxConnections>::Ethernet(infra::MemoryRange<const std::pair<hal::Port, uint8_t>> pins, hal::MacAddress macAddress, infra::BoundedString hostName,
        hal::SynchronousRandomDataGenerator& randomDataGenerator, infra::CreatorBase<services::Stoppable, void(services::LightweightIp& lightweightIp)>& connectedCreator)
        : lightweightIp(randomDataGenerator, connected)
        , lightweightIpConfig(ConvertConfig(hostName))
        , lightweightIpOverEthernetFactory(macAddress, lightweightIpConfig)
        , setSysConfig([]()
              {
                  SYSCFG->CMPCR |= SYSCFG_CMPCR_CMP_PD;
                  while ((SYSCFG->CMPCR & SYSCFG_CMPCR_READY) == 0)
                  {
                  }
              })
        , mdio(pins[0].first, pins[0].second)
        , mdc(pins[1].first, pins[1].second)
        , rmiiRefClk(pins[2].first, pins[2].second)
        , rmiiCrsDv(pins[3].first, pins[3].second)
        , rmiiRxD0(pins[4].first, pins[4].second)
        , rmiiRxD1(pins[5].first, pins[5].second)
        , rmiiTxEn(pins[6].first, pins[6].second)
        , rmiiTxD0(pins[7].first, pins[7].second)
        , rmiiTxD1(pins[8].first, pins[8].second)
        , ethernetSmi(mdio, mdc, hal::dummyPinStm, rmiiCrsDv, rmiiRxD0, rmiiRxD1, rmiiTxEn, rmiiTxD0, rmiiTxD1)
        , ethernetSmiObserver(ethernetSmi, lightweightIpOverEthernetFactory, rmiiRefClk)
        , connectedCreator(&connectedCreator)
        , hostName(hostName)
    {}

    extern const std::array<std::pair<hal::Port, uint8_t>, 9> ethernetPinsNucleoF767;
    extern const std::array<std::pair<hal::Port, uint8_t>, 9> ethernetPinsDiscoveryF7;
}

#endif
