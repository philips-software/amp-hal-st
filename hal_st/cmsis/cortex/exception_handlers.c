#include <stdlib.h>
#include "exception_handlers.h"

extern void __attribute__((noreturn)) _start(void);

void __attribute__((section(".after_vectors"), weak, naked)) Reset_Handler(void)
{
    asm volatile("ldr r0,=_start \n bx r0":::);
}

void __attribute__((weak)) Default_Handler()
{
    abort();
}

void __attribute__((naked)) DefaultExceptionHandlerForward()
{
    asm volatile("bx %0" : : "r" (Default_Handler));
}

void __attribute__((weak, alias("DefaultExceptionHandlerForward"))) NMI_Handler(void);
void __attribute__((weak, alias("DefaultExceptionHandlerForward"))) HardFault_Handler(void);
void __attribute__((weak, alias("DefaultExceptionHandlerForward"))) MemManage_Handler(void);
void __attribute__((weak, alias("DefaultExceptionHandlerForward"))) BusFault_Handler(void);
void __attribute__((weak, alias("DefaultExceptionHandlerForward"))) UsageFault_Handler(void);
void __attribute__((weak, alias("DefaultExceptionHandlerForward"))) SVC_Handler(void);
void __attribute__((weak, alias("DefaultExceptionHandlerForward"))) DebugMon_Handler(void);
void __attribute__((weak, alias("DefaultExceptionHandlerForward"))) PendSV_Handler(void);
void __attribute__((weak, alias("DefaultExceptionHandlerForward"))) SysTick_Handler(void);
