#ifndef CORTEX_PLATFORM_WINDOWS_HPP
#define CORTEX_PLATFORM_WINDOWS_HPP

// Of course windows is not a cortex platform, but this header contains various types and function
// definitions that satisfy Intellisense in Visual Studio

#include <cstdint>

typedef uint32_t IRQn_Type;

void NVIC_SetPriorityGrouping(uint32_t PriorityGroup);
uint32_t NVIC_GetPriorityGrouping();
void NVIC_EnableIRQ(IRQn_Type IRQn);
void NVIC_DisableIRQ(IRQn_Type IRQn);
uint32_t NVIC_GetPendingIRQ(IRQn_Type IRQn);
void NVIC_SetPendingIRQ(IRQn_Type IRQn);
void NVIC_ClearPendingIRQ(IRQn_Type IRQn);
uint32_t NVIC_GetActive(IRQn_Type IRQn);
void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority);
uint32_t NVIC_GetPriority(IRQn_Type IRQn);
uint32_t NVIC_EncodePriority(uint32_t PriorityGroup, uint32_t PreemptPriority, uint32_t SubPriority);
void NVIC_DecodePriority(uint32_t Priority, uint32_t PriorityGroup, uint32_t* pPreemptPriority, uint32_t* pSubPriority);
void NVIC_SystemReset();
uint32_t SysTick_Config(uint32_t ticks);

typedef struct
{
    uint32_t CPUID;
    uint32_t ICSR;
    uint32_t VTOR;
    uint32_t AIRCR;
    uint32_t SCR;
    uint32_t CCR;
    uint8_t  SHP[12];
    uint32_t SHCSR;
    uint32_t CFSR;
    uint32_t HFSR;
    uint32_t DFSR;
    uint32_t MMFAR;
    uint32_t BFAR;
    uint32_t AFSR;
    uint32_t PFR[2];
    uint32_t DFR;
    uint32_t ADR;
    uint32_t MMFR[4];
    uint32_t ISAR[5];
    uint32_t RESERVED0[5];
    uint32_t CPACR;
} SCB_Type;

typedef struct
{
    uint32_t CTRL;
    uint32_t LOAD;
    uint32_t VAL;
    uint32_t CALIB;
} SysTick_Type;

extern SysTick_Type* SysTick;
extern SCB_Type* SCB;

#define __DSB()
#define __ISB()
#define __DMB()
#define __WFE()
#define __SEV()

#define __disable_irq()
#define __enable_irq()

#define __builtin_unreachable()

#define __CORTEX_M 3

#endif
