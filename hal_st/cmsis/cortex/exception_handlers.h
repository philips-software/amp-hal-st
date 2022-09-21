#ifndef EXCEPTION_HANDLERS_H
#define EXCEPTION_HANDLERS_H

#if defined(__cplusplus)
extern "C"
{
#endif
    void Default_Handler(void);

    void Reset_Handler(void);
    void NMI_Handler(void);
    void HardFault_Handler(void);
    void MemManage_Handler(void);
    void BusFault_Handler(void);
    void UsageFault_Handler(void);
    void SVC_Handler(void);
    void DebugMon_Handler(void);
    void PendSV_Handler(void);
    void SysTick_Handler(void);

#if defined(__cplusplus)
}
#endif

#endif
