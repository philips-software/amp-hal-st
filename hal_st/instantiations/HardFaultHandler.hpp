#pragma once

#include "services/tracer/Tracer.hpp"

namespace main_
{

    void HardFaultHandler(const uint32_t* faultStack, uint32_t lr_value, services::Tracer& tracer)
    {
        auto r0 = faultStack[0];
        auto r1 = faultStack[1];
        auto r2 = faultStack[2];
        auto r3 = faultStack[3];
        auto r12 = faultStack[4];
        auto lr = faultStack[5]; // Link Register
        auto pc = faultStack[6]; // Program Counter
        auto psr = faultStack[7];

        auto cfsr = SCB->CFSR;
        auto BusFaultAddress = SCB->BFAR;
        auto MemManageFaultAddress = SCB->MMFAR;

        tracer.Trace() << "*** Hardfault! ***";
        tracer.Trace() << "Stack frame";
        tracer.Trace() << " R0  : 0x" << infra::hex << infra::Width(8, '0') << r0;
        tracer.Trace() << " R1  : 0x" << infra::hex << infra::Width(8, '0') << r1;
        tracer.Trace() << " R2  : 0x" << infra::hex << infra::Width(8, '0') << r2;
        tracer.Trace() << " R3  : 0x" << infra::hex << infra::Width(8, '0') << r3;
        tracer.Trace() << " R12 : 0x" << infra::hex << infra::Width(8, '0') << r12;
        tracer.Trace() << " LR  : 0x" << infra::hex << infra::Width(8, '0') << lr;
        tracer.Trace() << " PC  : 0x" << infra::hex << infra::Width(8, '0') << pc;
        tracer.Trace() << " PSR : 0x" << infra::hex << infra::Width(8, '0') << psr;
        tracer.Trace() << "FSR/FAR:";
        tracer.Trace() << " CFSR: 0x" << infra::hex << infra::Width(8, '0') << cfsr;
        tracer.Trace() << " HFSR: 0x" << infra::hex << infra::Width(8, '0') << SCB->HFSR;
        tracer.Trace() << " DFSR: 0x" << infra::hex << infra::Width(8, '0') << SCB->DFSR;
        tracer.Trace() << " AFSR: 0x" << infra::hex << infra::Width(8, '0') << SCB->AFSR;
        if (cfsr & 0x0080)
            tracer.Trace() << " MMFAR: 0x" << infra::hex << infra::Width(8, '0') << MemManageFaultAddress;
        if (cfsr & 0x8000)
            tracer.Trace() << " MMFAR: 0x" << infra::hex << infra::Width(8, '0') << BusFaultAddress;
        tracer.Trace() << "Misc";
        tracer.Trace() << " LR/EXC_RETURN= 0x" << infra::hex << infra::Width(8, '0') << lr_value;

        std::abort();
    }

}
