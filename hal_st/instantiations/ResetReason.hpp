#ifndef HAL_ST_INSTANTIATIONS_RESETREASON_HPP
#define HAL_ST_INSTANTIATIONS_RESETREASON_HPP

#include "infra/util/BoundedString.hpp"
#include DEVICE_HEADER

namespace hal
{
    // Note: This function clears the reset flags after reading them
    infra::BoundedConstString GetResetReason()
    {
        const char* resetReason;

        if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST))
            resetReason = "LowPower";
        else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST))
            resetReason = "PowerOn";
        else if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST))
            resetReason = "Window WatchDog";
        else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST))
            resetReason = "Independent WatchDog";
        else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST))
            resetReason = "Software";
        else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST))
            resetReason = "Reset Pin";
        else if (__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST))
            resetReason = "BrownOut(BOR)";
        else
            resetReason = "Unknown";

        __HAL_RCC_CLEAR_RESET_FLAGS();
        return resetReason;
    }
}

#endif // HAL_ST_INSTANTIATIONS_RESETREASON_HPP
