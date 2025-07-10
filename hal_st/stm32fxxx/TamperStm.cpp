#include "hal_st/stm32fxxx/TamperStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include <cassert>
#include DEVICE_HEADER

namespace hal
{
    ActiveTamperStm::ActiveTamperStm(RtcBaseStm& rtcBase, uint8_t tampInOneBasedIndex, uint8_t tampOutOneBasedIndex, infra::MemoryRange<uint32_t> seed, const infra::Function<void()>& onTamperIrq)
        : rtc(rtcBase)
        , tamperInIndex(tampInOneBasedIndex - 1)
        , tamperOutIndex(tampOutOneBasedIndex - 1)
        , onTamperIrq(onTamperIrq)
        , interruptHandler(peripheralTampIrq[0], [this]
              {
                  OnInterrupt();
              })
    {
        RTC_PrivilegeStateTypeDef privilegeState{};
        privilegeState.tampPrivilegeFull = TAMP_PRIVILEGE_FULL_YES;
        auto result = HAL_RTCEx_PrivilegeModeSet(&rtc.Handle(), &privilegeState);
        assert(result == HAL_OK);

        RTC_ActiveTampersTypeDef sAllTamper{};
        sAllTamper.ActiveFilter = RTC_ATAMP_FILTER_DISABLE;
        sAllTamper.ActiveAsyncPrescaler = RTC_ATAMP_ASYNCPRES_RTCCLK;
        sAllTamper.TimeStampOnTamperDetection = RTC_TIMESTAMPONTAMPERDETECTION_ENABLE;
        sAllTamper.ActiveOutputChangePeriod = 0;
        for (int i = 0; i != RTC_ATAMP_SEED_NB_UINT32; ++i)
            sAllTamper.Seed[i] = seed[i];
        sAllTamper.TampInput[tamperInIndex].Enable = RTC_ATAMP_ENABLE;
        sAllTamper.TampInput[tamperInIndex].Interrupt = RTC_ATAMP_INTERRUPT_ENABLE;
        sAllTamper.TampInput[tamperInIndex].Output = tamperOutIndex;
        sAllTamper.TampInput[tamperInIndex].NoErase = RTC_TAMPER_ERASE_BACKUP_ENABLE;
        result = HAL_RTCEx_SetActiveTampers(&rtc.Handle(), &sAllTamper);
        assert(result == HAL_OK);
    }

    void ActiveTamperStm::OnInterrupt()
    {
        // let HAL handle clearing all flags
        HAL_RTCEx_TamperIRQHandler(&rtc.Handle());
        onTamperIrq();
    }
}
