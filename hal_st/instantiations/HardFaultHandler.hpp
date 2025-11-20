#pragma once

#include "services/tracer/Tracer.hpp"

namespace main_
{

    void HardFaultHandler(const uint32_t* faultStack, uint32_t lr_value, services::Tracer& tracer)
    {
        // Copying variables onto the stack so that it can be inspected in the debugger

        auto r0 = faultStack[0];
        auto r1 = faultStack[1];
        auto r2 = faultStack[2];
        auto r3 = faultStack[3];
        auto r12 = faultStack[4];
        auto lr = faultStack[5];
        auto pc = faultStack[6];
        auto psr = faultStack[7];

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

        tracer.Trace() << "*** Hardfault! ***";
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
        tracer.Trace() << " CFSR: 0x" << infra::hex << infra::Width(8, '0') << cfsr;
        tracer.Trace() << " DFSR: 0x" << infra::hex << infra::Width(8, '0') << SCB->DFSR;
        tracer.Trace() << " AFSR: 0x" << infra::hex << infra::Width(8, '0') << SCB->AFSR;
        if (cfsr & 0x0080)
            tracer.Trace() << " MMFAR: 0x" << infra::hex << infra::Width(8, '0') << MemManageFaultAddress;
        if (cfsr & 0x8000)
            tracer.Trace() << " BFAR: 0x" << infra::hex << infra::Width(8, '0') << BusFaultAddress;
        tracer.Trace() << "Misc";
        tracer.Trace() << " LR/EXC_RETURN= 0x" << infra::hex << infra::Width(8, '0') << lr_value;

        std::abort();
    }

}
