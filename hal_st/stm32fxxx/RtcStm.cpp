#include "hal_st/stm32fxxx/RtcStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include <cassert>
#include DEVICE_HEADER

namespace hal
{
    RtcBaseStm::RtcBaseStm(const Config& config)
    {
        EnableClockRtc(rtcIndex);

        handle.Instance = peripheralRtc[rtcIndex];
        handle.Init.HourFormat = config.hourFormat;
        handle.Init.AsynchPrediv = config.asynchPrediv;
        handle.Init.SynchPrediv = config.asynchPrediv;
        handle.Init.OutPut = config.output;
        handle.Init.OutPutPolarity = config.outputPolarity;
        handle.Init.OutPutType = config.outputType;
        handle.Init.BinMode = RTC_BINARY_NONE;
        auto result = HAL_RTC_Init(&handle);
        assert(result == HAL_OK);

        RTC_PrivilegeStateTypeDef privilegeState{};
        privilegeState.rtcPrivilegeFull = RTC_PRIVILEGE_FULL_NO;
        privilegeState.backupRegisterPrivZone = RTC_PRIVILEGE_BKUP_ZONE_NONE;
        privilegeState.backupRegisterStartZone2 = RTC_BKP_DR0;
        privilegeState.backupRegisterStartZone3 = RTC_BKP_DR0;
        result = HAL_RTCEx_PrivilegeModeSet(&handle, &privilegeState);
        assert(result == HAL_OK);
    }
}
