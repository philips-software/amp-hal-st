#include "hal_st/stm32fxxx/UniqueDeviceId.hpp"

namespace hal
{
#if defined(STM32F373xC)
    infra::ConstByteRange UniqueDeviceId()
    {
        const uint8_t* base = reinterpret_cast<const uint8_t*>(0x1FFFF7AC);
        return infra::ConstByteRange(base, base + 12);
    }
#endif

#if defined(STM32F407xx)
    infra::ConstByteRange UniqueDeviceId()
    {
        const uint8_t* base = reinterpret_cast<const uint8_t*>(0x1FFF7A10);
        return infra::ConstByteRange(base, base + 12);
    }
#endif

#if defined(STM32F7)
    infra::ConstByteRange UniqueDeviceId()
    {
        const uint8_t* base = reinterpret_cast<const uint8_t*>(0x1FF0F420);
        return infra::ConstByteRange(base, base + 12);
    }
#endif

#if defined(STM32WB55xx)
    infra::ConstByteRange UniqueDeviceId()
    {
        const uint8_t* base = reinterpret_cast<const uint8_t*>(0x1FFF7590);
        return infra::ConstByteRange(base, base + 12);
    }
#endif
}
