#include "exception_handlers.h"

// Forward declaration of the specific IRQ handlers. When the application
// defines a handler (with the same name), this will automatically take
// precedence over these weak definitions

void __attribute__((naked)) DefaultHandlerForward()
{
    asm volatile("bx %0" : : "r" (Default_Handler));
}

void __attribute__ ((weak, alias ("DefaultHandlerForward")))
WWDG_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
PVD_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
TAMP_STAMP_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
RTC_WKUP_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
FLASH_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
RCC_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
EXTI0_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
EXTI1_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
EXTI2_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
EXTI3_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
EXTI4_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
DMA1_Channel1_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
DMA1_Channel2_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
DMA1_Channel3_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
DMA1_Channel4_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
DMA1_Channel5_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
DMA1_Channel6_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
DMA1_Channel7_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
DMA1_Channel8_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
ADC_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
CAN1_TX_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
CAN1_RX0_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
CAN1_RX1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
CAN1_SCE_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
EXTI9_5_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
TIM15_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
TIM16_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
TIM17_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
TIM18_DAC2_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
CEC_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
TIM2_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
TIM3_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
TIM4_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
I2C1_EV_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
I2C1_ER_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
I2C2_EV_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
I2C2_ER_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
SPI1_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
SPI2_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
USART1_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
USART2_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
USART3_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
EXTI15_10_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
RTC_Alarm_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
TIM12_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
TIM13_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
TIM14_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
TIM5_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
SPI3_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
TIM6_DAC1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
TIM7_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
DMA2_Channel1_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
DMA2_Channel2_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
DMA2_Channel3_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
DMA2_Channel4_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
DMA2_Channel5_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
SDADC1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
SDADC2_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
SDADC3_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
COMP1_2_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
USB_HP_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
USB_LP_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
USB_WAKEUP_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward")))
TIM19_IRQHandler(void);
void __attribute__ ((weak, alias ("DefaultHandlerForward")))
FPU_IRQHandler(void);


// ----------------------------------------------------------------------------

extern unsigned int _estack;

typedef void
(* const pHandler)(void);

// ----------------------------------------------------------------------------

// The vector table.
// This relies on the linker script to place at correct location in memory.

__attribute__ ((section(".isr_vector"),used))
pHandler  __isr_vectors[] =
  {
  // Core Level - CM4
      (pHandler) &_estack,                      // The initial stack pointer
      Reset_Handler,                            // The reset handler

      NMI_Handler,                              // The NMI handler
      HardFault_Handler,                        // The hard fault handler

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
      MemManage_Handler,                        // The MPU fault handler
      BusFault_Handler,                        // The bus fault handler
      UsageFault_Handler,                        // The usage fault handler
#else
      0, 0, 0,                                  // Reserved
#endif
      0,                                        // Reserved
      0,                                        // Reserved
      0,                                        // Reserved
      0,                                        // Reserved
      SVC_Handler,                              // SVCall handler
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
      DebugMon_Handler,                         // Debug monitor handler
#else
      0,                                        // Reserved
#endif
      0,                                        // Reserved
      PendSV_Handler,                           // The PendSV handler
      SysTick_Handler,                          // The SysTick handler

      WWDG_IRQHandler,
      PVD_IRQHandler,
      TAMP_STAMP_IRQHandler,
      RTC_WKUP_IRQHandler,
      FLASH_IRQHandler,
      RCC_IRQHandler,
      EXTI0_IRQHandler,
      EXTI1_IRQHandler,
      EXTI2_IRQHandler,
      EXTI3_IRQHandler,
      EXTI4_IRQHandler,
      DMA1_Channel1_IRQHandler,
      DMA1_Channel2_IRQHandler,
      DMA1_Channel3_IRQHandler,
      DMA1_Channel4_IRQHandler,
      DMA1_Channel5_IRQHandler,
      DMA1_Channel6_IRQHandler,
      DMA1_Channel7_IRQHandler,
      ADC_IRQHandler,
      CAN1_TX_IRQHandler,
      CAN1_RX0_IRQHandler,
      CAN1_RX1_IRQHandler,
      CAN1_SCE_IRQHandler,
      EXTI9_5_IRQHandler,
      TIM15_IRQHandler,
      TIM16_IRQHandler,
      TIM17_IRQHandler,
      TIM18_DAC2_IRQHandler,
      TIM2_IRQHandler,
      TIM3_IRQHandler,
      TIM4_IRQHandler,
      I2C1_EV_IRQHandler,
      I2C1_ER_IRQHandler,
      I2C2_EV_IRQHandler,
      I2C2_ER_IRQHandler,
      SPI1_IRQHandler,
      SPI2_IRQHandler,
      USART1_IRQHandler,
      USART2_IRQHandler,
      USART3_IRQHandler,
      EXTI15_10_IRQHandler,
      RTC_Alarm_IRQHandler,
      CEC_IRQHandler,
      TIM12_IRQHandler,
      TIM13_IRQHandler,
      TIM14_IRQHandler,
      0,
      0,
      0,
      0,
      TIM5_IRQHandler,
      SPI3_IRQHandler,
      0,
      0,
      TIM6_DAC1_IRQHandler,
      TIM7_IRQHandler,
      DMA2_Channel1_IRQHandler,
      DMA2_Channel2_IRQHandler,
      DMA2_Channel3_IRQHandler,
      DMA2_Channel4_IRQHandler,
      DMA2_Channel5_IRQHandler,
      SDADC1_IRQHandler,
      SDADC2_IRQHandler,
      SDADC3_IRQHandler,
      COMP1_2_IRQHandler,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      USB_HP_IRQHandler,
      USB_LP_IRQHandler,
      USB_WAKEUP_IRQHandler,
      0,
      TIM19_IRQHandler,
      0,
      0,
      FPU_IRQHandler
  };
