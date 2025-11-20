#include "hal_st/cortex/Fault.hpp"
#include DEVICE_HEADER
#include "services/tracer/Tracer.hpp"

namespace
{
    extern "C" uint32_t link_code_location;
    extern "C" uint32_t link_code_end;
    extern "C" uint32_t _estack;

    struct InterruptContext
    {
        const uint32_t* stack;
        uint32_t lrValue;
    } interruptContext;

    static hal::fault::TracerProvider tracerProvider = nullptr;

    void FeedWatchdog()
    {
        WWDG->CR |= WWDG_CR_T;
    }

    void PrintBacktrace(const uint32_t* stack, services::Tracer& tracer)
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
        for (const uint32_t* stackPointer = stack; stackPointer != &_estack; ++stackPointer)
        {
            if (*stackPointer >= reinterpret_cast<uint32_t>(&link_code_location) && *stackPointer < reinterpret_cast<uint32_t>(&link_code_end))
                tracer.Continue() << " 0x" << infra::hex << infra::Width(8, '0') << (*stackPointer & 0xfffffffe);

            FeedWatchdog();
        }

        tracer.Trace() << ""; // new line
    }

    void DumpCurrentInterruptStackAndAbort(services::Tracer& tracer)
    {
        // Copying variables onto the stack so that it can be inspected in the debugger

        auto r0 = interruptContext.stack[0];
        auto r1 = interruptContext.stack[1];
        auto r2 = interruptContext.stack[2];
        auto r3 = interruptContext.stack[3];
        auto r12 = interruptContext.stack[4];
        auto lr = interruptContext.stack[5];
        auto pc = interruptContext.stack[6];
        auto psr = interruptContext.stack[7];

        auto cfsr = SCB->CFSR;
        auto MemManageFaultAddress = SCB->MMFAR;
        auto BusFaultAddress = SCB->BFAR;

        /* How to interpret the fault. This is all explained in detail in: https://www.keil.com/appnotes/files/apnt209.pdf
         * Check SCB->HFSR for the cause of the hardfault. Most likely it was one of these scenarios:
         *   - DEBUGEVT (bit 31 set): Debug event, which is usually called during a `std::abort()`.
         *     - This usually means PC will point to the abort handler, and LR will point to the instruction that caused the abort.
         *   - FORCED (bit 30 set): A configurable fault (Usage, Bus, Memory Management) has occurred and the system could not handle it.
         *     - This requires a bit more investiation, as explain below.
         *
         *  For FORCED faults, check the FSR/FAR registers to determine the cause.
         *  Looks for which bits are set. Sometimes BFAR or MMFAR points to the fault address.
         */

        PrintBacktrace(interruptContext.stack, tracer);

        tracer.Trace() << "Stack frame";
        tracer.Trace() << " R0  : 0x" << infra::hex << infra::Width(8, '0') << r0; // Usuaully contains the parameter values
        tracer.Trace() << " R1  : 0x" << infra::hex << infra::Width(8, '0') << r1; // Usuaully contains the parameter values
        tracer.Trace() << " R2  : 0x" << infra::hex << infra::Width(8, '0') << r2; // Usuaully contains the parameter values
        tracer.Trace() << " R3  : 0x" << infra::hex << infra::Width(8, '0') << r3; // Usuaully contains the parameter values
        tracer.Trace() << " R12 : 0x" << infra::hex << infra::Width(8, '0') << r12;
        tracer.Trace() << " LR  : 0x" << infra::hex << infra::Width(8, '0') << lr;  // Link Register, where the current function was called from.
        tracer.Trace() << " PC  : 0x" << infra::hex << infra::Width(8, '0') << pc;  // Program Counter, usually where the fault occured.
        tracer.Trace() << " PSR : 0x" << infra::hex << infra::Width(8, '0') << psr; // Program Status Register
        tracer.Trace() << "FSR/FAR:";
        tracer.Trace() << " HFSR: 0x" << infra::hex << infra::Width(8, '0') << SCB->HFSR;
        if (SCB->HFSR & SCB_HFSR_DEBUGEVT_Msk)
            tracer.Trace() << "  (DEBUGEVT active)"; // This is typically forced by a call to std::abort()
        tracer.Trace() << " CFSR: 0x" << infra::hex << infra::Width(8, '0') << cfsr;
        tracer.Trace() << " DFSR: 0x" << infra::hex << infra::Width(8, '0') << SCB->DFSR;
        tracer.Trace() << " AFSR: 0x" << infra::hex << infra::Width(8, '0') << SCB->AFSR;
        if (cfsr & SCB_CFSR_MMARVALID_Msk)
            tracer.Trace() << " MMFAR: 0x" << infra::hex << infra::Width(8, '0') << MemManageFaultAddress;
        if (cfsr & SCB_CFSR_BFARVALID_Msk)
            tracer.Trace() << " BFAR: 0x" << infra::hex << infra::Width(8, '0') << BusFaultAddress;
        tracer.Trace() << "Misc";
        tracer.Trace() << " LR/EXC_RETURN= 0x" << infra::hex << infra::Width(8, '0') << interruptContext.lrValue;
        tracer.Trace() << infra::endl;

        std::abort();
    }

}

namespace hal::fault
{
    void SetInterruptContext(const uint32_t* stack, uint32_t lrValue)
    {
        interruptContext.stack = stack;
        interruptContext.lrValue = lrValue;
    }

    void DumpInterruptStackAndAbort(infra::BoundedConstString fault)
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

    DefaultHandler::DefaultHandler(TracerProvider tracerProvider)
        : hardfaultRegistration(static_cast<IRQn_Type>(-13), []()
              {
                  if (::tracerProvider)
                  {
                      auto& tracer = ::tracerProvider();
                      tracer.Trace() << "*** Hard fault! ***";
                      DumpCurrentInterruptStackAndAbort(tracer);
                  }
                  else
                      std::abort();
              })
    {
        if (tracerProvider)
            ::tracerProvider = std::move(tracerProvider);
    }

}
