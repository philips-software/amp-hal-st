#include "hal_st/cortex/FaultTracer.hpp"
#include DEVICE_HEADER
#include "services/tracer/Tracer.hpp"

namespace
{
    void FeedWatchdog()
    {
        WWDG->CR |= WWDG_CR_T;
    }
}

namespace hal
{
    InterruptContext interruptContext{};

    DefaultFaultTracer::DefaultFaultTracer(const infra::MemoryRange<const uint32_t>& instructionRange, const uint32_t* endOfStack, TracerProvider tracerProvider)
        : instructionRange(instructionRange)
        , endOfStack(endOfStack)
        , tracerProvider(tracerProvider)
        , hardfaultRegistration(static_cast<IRQn_Type>(-13), [this]()
              {
                  static_assert(__CORTEX_M >= 0x3U);
                  if (this->tracerProvider)
                  {
                      auto& tracer = this->tracerProvider();
                      tracer.Trace() << "*** Hard fault! ***";
                      DumpCurrentInterruptStackAndAbort(tracer);
                  }
                  else
                      std::abort();
              })
    {}

    void DefaultFaultTracer::DumpInterruptStackAndAbort(infra::BoundedConstString fault) const
    {
        if (tracerProvider)
        {
            auto& tracer = tracerProvider();
            tracer.Trace() << "*** Fault: " << fault << " ***";
            DumpCurrentInterruptStackAndAbort(tracer);
        }
        else
            std::abort();
    }

    void DefaultFaultTracer::PrintBacktrace(const uint32_t* stack, services::Tracer& tracer) const
    {
        FeedWatchdog();

        tracer.Trace() << infra::endl
                       << "Backtrace:";

        // Note: This is not a true backtrace (also called callstack), but one that works well enough.
        // Ideally the stack is smartly unwound using debug information. Instead we just print all values
        // in the stack that look like code addresses. The result will still contain the backtrace, but also
        // some spurious values in between.
        //
        // Technically we should also skip the initial fault stack registers, but it also contains the PC and LR values
        // which other tooling depend on to be useful.
        for (const uint32_t* stackPointer = stack; stackPointer != endOfStack; ++stackPointer)
        {
            if (instructionRange.contains(reinterpret_cast<uint32_t*>(*stackPointer)))
                tracer.Continue() << " 0x" << infra::hex << infra::Width(8, '0') << (*stackPointer & 0xfffffffe);

            FeedWatchdog();
        }

        tracer.Trace() << "";
    }

    void DefaultFaultTracer::DumpCfsrContents(uint32_t cfsr, services::Tracer& tracer) const
    {
        /* Memory Manage Faults */
        if (cfsr & SCB_CFSR_IACCVIOL_Msk)
            tracer.Trace() << "  MemManageFault: Instruction access violation";
        if (cfsr & SCB_CFSR_DACCVIOL_Msk)
            tracer.Trace() << "  MemManageFault: Data access violation";
        if (cfsr & SCB_CFSR_MUNSTKERR_Msk)
            tracer.Trace() << "  MemManageFault: Unstacking for exception return";
        if (cfsr & SCB_CFSR_MSTKERR_Msk)
            tracer.Trace() << "  MemManageFault: Stacking for exception entry";
        if (cfsr & SCB_CFSR_MLSPERR_Msk)
            tracer.Trace() << "  MemManageFault: FP lazy state preservation";
        if (cfsr & SCB_CFSR_MMARVALID_Msk)
            tracer.Trace() << "  MemManageFault: MMFAR valid";

        /* Bus Faults */
        if (cfsr & SCB_CFSR_IBUSERR_Msk)
            tracer.Trace() << "  BusFault: Instruction bus error";
        if (cfsr & SCB_CFSR_PRECISERR_Msk)
            tracer.Trace() << "  BusFault: Precise data bus error";
        if (cfsr & SCB_CFSR_IMPRECISERR_Msk)
            tracer.Trace() << "  BusFault: Imprecise data bus error";
        if (cfsr & SCB_CFSR_UNSTKERR_Msk)
            tracer.Trace() << "  BusFault: Unstacking for exception return";
        if (cfsr & SCB_CFSR_STKERR_Msk)
            tracer.Trace() << "  BusFault: Stacking for exception entry";
        if (cfsr & SCB_CFSR_LSPERR_Msk)
            tracer.Trace() << "  BusFault: FP lazy state preservation";
        if (cfsr & SCB_CFSR_BFARVALID_Msk)
            tracer.Trace() << "  BusFault: BFAR valid";

        /* Usage Faults */
        if (cfsr & SCB_CFSR_UNDEFINSTR_Msk)
            tracer.Trace() << "  UsageFault: Illegal instruction";
        if (cfsr & SCB_CFSR_INVSTATE_Msk)
            tracer.Trace() << "  UsageFault: Invalid state";
        if (cfsr & SCB_CFSR_INVPC_Msk)
            tracer.Trace() << "  UsageFault: Invalid PC load usage fault";
            // Ignoring NOCP
#if defined(SCB_CFSR_STKOF_Msk)
        if (cfsr & SCB_CFSR_STKOF_Msk)
            tracer.Trace() << "  UsageFault: Stack overflow";
#endif
        if (cfsr & SCB_CFSR_UNALIGNED_Msk)
            tracer.Trace() << "  UsageFault: Unaligned access";
        if (cfsr & SCB_CFSR_DIVBYZERO_Msk)
            tracer.Trace() << "  UsageFault: Divide by zero";
    }

    [[noreturn]] void DefaultFaultTracer::DumpCurrentInterruptStackAndAbort(services::Tracer& tracer) const
    {
        if (!interruptContext.stack)
        {
            tracer.Trace() << "No interrupt stack available";
            std::abort();
        }

        PrintBacktrace(hal::interruptContext.stack, tracer);

        /* How to interpret the fault. This is all explained in detail in: https://www.keil.com/appnotes/files/apnt209.pdf
         * Check SCB->HFSR for the cause of the hardfault. Most likely it was one of these scenarios:
         *   - DEBUGEVT (bit 31 set): Debug event, which is usually called during a `std::abort()`.
         *     - This usually means PC will point to the abort handler, and LR will point to the instruction that caused the abort.
         *   - FORCED (bit 30 set): A configurable fault (Usage, Bus, Memory Management) has occurred and the system could not handle it.
         *     - This requires a bit more investigation, as explained below.
         *
         *  For FORCED faults, check the FSR/FAR registers to determine the cause.
         *  Looks for which bits are set. Sometimes BFAR or MMFAR points to the fault address.
         */

        auto r0 = interruptContext.stack[0];
        auto r1 = interruptContext.stack[1];
        auto r2 = interruptContext.stack[2];
        auto r3 = interruptContext.stack[3];
        auto r12 = interruptContext.stack[4];
        auto lr = interruptContext.stack[5];
        auto pc = interruptContext.stack[6];
        auto psr = interruptContext.stack[7];
        tracer.Trace() << "Stack frame";
        tracer.Trace() << " R0  : 0x" << infra::hex << infra::Width(8, '0') << r0; // Usually contains the parameter values
        tracer.Trace() << " R1  : 0x" << infra::hex << infra::Width(8, '0') << r1; // Usually contains the parameter values
        tracer.Trace() << " R2  : 0x" << infra::hex << infra::Width(8, '0') << r2; // Usually contains the parameter values
        tracer.Trace() << " R3  : 0x" << infra::hex << infra::Width(8, '0') << r3; // Usually contains the parameter values
        tracer.Trace() << " R12 : 0x" << infra::hex << infra::Width(8, '0') << r12;
        tracer.Trace() << " LR  : 0x" << infra::hex << infra::Width(8, '0') << lr;  // Link Register, where the current function was called from.
        tracer.Trace() << " PC  : 0x" << infra::hex << infra::Width(8, '0') << pc;  // Program Counter, usually where the fault occurred.
        tracer.Trace() << " PSR : 0x" << infra::hex << infra::Width(8, '0') << psr; // Program Status Register

        auto cfsr = SCB->CFSR;
        auto MemManageFaultAddress = SCB->MMFAR;
        auto BusFaultAddress = SCB->BFAR;

        tracer.Trace() << "FSR/FAR:";
        tracer.Trace() << " HFSR: 0x" << infra::hex << infra::Width(8, '0') << SCB->HFSR;
        if (SCB->HFSR & SCB_HFSR_VECTTBL_Msk)
            tracer.Trace() << "    VECTTBL (Hard Fault)";
        if (SCB->HFSR & SCB_HFSR_DEBUGEVT_Msk)
            tracer.Trace() << "  DEBUGEVT"; // This is typically forced by a call to std::abort()
        if (SCB->HFSR & SCB_HFSR_FORCED_Msk)
            tracer.Trace() << "  FORCED (Hard Fault)";
        tracer.Trace() << " CFSR: 0x" << infra::hex << infra::Width(8, '0') << cfsr;
        DumpCfsrContents(cfsr, tracer);
        tracer.Trace() << " DFSR: 0x" << infra::hex << infra::Width(8, '0') << SCB->DFSR;
        tracer.Trace() << " AFSR: 0x" << infra::hex << infra::Width(8, '0') << SCB->AFSR;
        if (cfsr & SCB_CFSR_MMARVALID_Msk)
            tracer.Trace() << " MMFAR: 0x" << infra::hex << infra::Width(8, '0') << MemManageFaultAddress;
        if (cfsr & SCB_CFSR_BFARVALID_Msk)
            tracer.Trace() << " BFAR: 0x" << infra::hex << infra::Width(8, '0') << BusFaultAddress;
        tracer.Trace() << "Misc";
        tracer.Trace() << " LR/EXC_RETURN= 0x" << infra::hex << infra::Width(8, '0') << interruptContext.lrValue;
        tracer.Trace() << infra::endl;

        // Aborting here so that this function's stack is maintained for debugging.
        std::abort();
    }

}
