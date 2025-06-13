
#include "TamperStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include <cassert>
#include DEVICE_HEADER

namespace hal
{
    TamperStm::TamperStm(const Config& rtcConfig, uint8_t tampInOneBasedIndex, uint8_t tampOutOneBasedIndex, infra::MemoryRange<uint32_t> seed, const infra::Function<void()>& onTamperIrq)
        : RtcBaseStm(rtcConfig)
        , tamperInIndex_(tampInOneBasedIndex - 1)
        , tamperOutIndex_(tampOutOneBasedIndex - 1)
        , seed_(seed)
        , onTamperIrq_(onTamperIrq)
        , interruptHandler_(peripheralTampIrq[0], [this] { OnInterrupt(); })
    {
        ConfigureTamper();
    }

    void TamperStm::ConfigureTamper()
    {
        RTC_PrivilegeStateTypeDef privilegeState{};
        privilegeState.tampPrivilegeFull = TAMP_PRIVILEGE_FULL_YES;
        if (HAL_RTCEx_PrivilegeModeSet(&handle_, &privilegeState) != HAL_OK)
        {
            std::abort();
        }
        RTC_ActiveTampersTypeDef sAllTamper{};
        sAllTamper.ActiveFilter = RTC_ATAMP_FILTER_DISABLE;
        sAllTamper.ActiveAsyncPrescaler = RTC_ATAMP_ASYNCPRES_RTCCLK;
        sAllTamper.TimeStampOnTamperDetection = RTC_TIMESTAMPONTAMPERDETECTION_ENABLE;
        sAllTamper.ActiveOutputChangePeriod = 0;
        for (int i = 0; i < 4; ++i)
        {
            sAllTamper.Seed[i] = seed_[i];
        }
        sAllTamper.TampInput[tamperInIndex_].Enable = RTC_ATAMP_ENABLE;
        sAllTamper.TampInput[tamperInIndex_].Interrupt = RTC_ATAMP_INTERRUPT_ENABLE;
        sAllTamper.TampInput[tamperInIndex_].Output = tamperOutIndex_;
        sAllTamper.TampInput[tamperInIndex_].NoErase = RTC_TAMPER_ERASE_BACKUP_ENABLE;
        if (HAL_RTCEx_SetActiveTampers(&handle_, &sAllTamper) != HAL_OK)
        {
            std::abort();
        }
    }

    void TamperStm::OnInterrupt()
    {
        HAL_RTCEx_TamperIRQHandler(&handle_);
        onTamperIrq_();
    }
}
