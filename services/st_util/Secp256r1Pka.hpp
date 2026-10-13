#ifndef SERVICES_SECP256R1_PKA_HPP
#define SERVICES_SECP256R1_PKA_HPP

#include "hal_st/stm32fxxx/PkaStm.hpp"

namespace services
{
    const hal::PkaStm::Curve& Secp256r1Pka();
}

#endif
