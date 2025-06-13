
#include "RtcStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include <cassert>
#include DEVICE_HEADER

namespace hal
{
    RtcBaseStm::RtcBaseStm(const Config& config)
    {
        EnableClockRtc(rtcIndex_);

        handle_.Instance = peripheralRtc[rtcIndex_];
        handle_.Init.HourFormat = config.hourFormat;
        handle_.Init.AsynchPrediv = config.asynchPrediv;
        handle_.Init.SynchPrediv = config.asynchPrediv;
        handle_.Init.OutPut = config.output;
        handle_.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
        handle_.Init.OutPutPolarity = config.outputPolarity;
        handle_.Init.OutPutType = config.outputType;
        handle_.Init.OutPutPullUp = config.outputPullUp;
        handle_.Init.BinMode = config.binMode;
        if (HAL_RTC_Init(&handle_) != HAL_OK)
        {
            std::abort();
        }
        RTC_PrivilegeStateTypeDef privilegeState{};
        privilegeState.rtcPrivilegeFull = RTC_PRIVILEGE_FULL_NO;
        privilegeState.backupRegisterPrivZone = RTC_PRIVILEGE_BKUP_ZONE_NONE;
        privilegeState.backupRegisterStartZone2 = RTC_BKP_DR0;
        privilegeState.backupRegisterStartZone3 = RTC_BKP_DR0;
        if (HAL_RTCEx_PrivilegeModeSet(&handle_, &privilegeState) != HAL_OK)
        {
            std::abort();
        }
    }
}
