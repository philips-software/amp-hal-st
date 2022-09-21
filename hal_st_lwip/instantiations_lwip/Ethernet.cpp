#include "hal_st_lwip/instantiations_lwip/Ethernet.hpp"

namespace main_
{
    const std::array<std::pair<hal::Port, uint8_t>, 9> ethernetPinsNucleoF767 = { {
        { hal::Port::A, 2 },
        { hal::Port::C, 1 },
        { hal::Port::A, 1 },
        { hal::Port::A, 7 },
        { hal::Port::C, 4 },
        { hal::Port::C, 5 },
        { hal::Port::G, 11 },
        { hal::Port::G, 13 },
        { hal::Port::B, 13 }
    } };

    const std::array<std::pair<hal::Port, uint8_t>, 9> ethernetPinsDiscoveryF7 = { {
        { hal::Port::A, 2 },
        { hal::Port::C, 1 },
        { hal::Port::A, 1 },
        { hal::Port::A, 7 },
        { hal::Port::C, 4 },
        { hal::Port::C, 5 },
        { hal::Port::G, 11 },
        { hal::Port::G, 13 },
        { hal::Port::G, 14 }
    } };
}
