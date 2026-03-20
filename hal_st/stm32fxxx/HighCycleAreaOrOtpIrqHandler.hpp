#ifndef HALST_HIGH_CYCLE_AREA_OR_OTP_IRQ_HANDLER_HPP
#define HALST_HIGH_CYCLE_AREA_OR_OTP_IRQ_HANDLER_HPP

#include "hal_st/cortex/InterruptCortex.hpp"

namespace hal
{
    // This class is used as a base class for both FlashInternalHighCycleAreaStm::WithIrqHandler and OneTimeProgrammableFlashStm::WithIrqHandler,
    // as they share the same NMI handler which is responsible for clearing the flash end of operation flag.
    class HighCycleAreaOrOtpIrqHandler
    {
    public:
        HighCycleAreaOrOtpIrqHandler();

    private:
        hal::ImmediateInterruptHandler nmi;
    };
}

#endif
