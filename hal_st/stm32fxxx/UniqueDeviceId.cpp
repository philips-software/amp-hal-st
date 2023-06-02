#include "hal_st/stm32fxxx/UniqueDeviceId.hpp"
#include DEVICE_HEADER

namespace hal
{
    infra::ConstByteRange UniqueDeviceId()
    {
        const auto* base = reinterpret_cast<const uint8_t*>(UID_BASE);
        return infra::ConstByteRange(base, base + 12);
    }
}
