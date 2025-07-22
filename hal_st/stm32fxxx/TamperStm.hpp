#ifndef HAL_TAMPER_STM_HPP
#define HAL_TAMPER_STM_HPP

#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal_st/stm32fxxx/RtcStm.hpp"
#include "infra/util/Function.hpp"
#include DEVICE_HEADER

namespace hal
{
    class ActiveTamperStm
    {
    public:
        explicit ActiveTamperStm(RtcBaseStm& rtcBase, uint8_t tampInOneBasedIndex, uint8_t tampOutOneBasedIndex, infra::MemoryRange<uint32_t> seed, const infra::Function<void()>& onTamperIrq);

    private:
        void OnInterrupt();

    private:
        RtcBaseStm& rtc;
        const uint8_t tamperInIndex;
        const uint8_t tamperOutIndex;
        infra::Function<void()> onTamperIrq;
        ImmediateInterruptHandler interruptHandler;
    };
}
#endif // HAL_TAMPER_STM_HPP
