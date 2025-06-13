#ifndef HAL_TAMPER_STM_HPP
#define HAL_TAMPER_STM_HPP
#include "RtcStm.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "infra/util/Function.hpp"
#include DEVICE_HEADER

namespace hal
{
    class TamperStm : public RtcBaseStm
    {
    public:
        explicit TamperStm(const Config& rtcConfig, uint8_t tampInOneBasedIndex, uint8_t tampOutOneBasedIndex, infra::MemoryRange<uint32_t> seed, const infra::Function<void()>& onTamperIrq = nullptr);

    private:
        const uint8_t tamperInIndex_;
        const uint8_t tamperOutIndex_;
        infra::MemoryRange<uint32_t> seed_;
        infra::Function<void()> onTamperIrq_;
        ImmediateInterruptHandler interruptHandler_;
        void ConfigureTamper();
        void OnInterrupt();
    };
}
#endif // HAL_TAMPER_STM_HPP
