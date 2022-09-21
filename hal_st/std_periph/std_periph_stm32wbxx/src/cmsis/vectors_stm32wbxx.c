#include "exception_handlers.h"
#include <stdlib.h>

#if !defined(STM32WB55xx)
#error "Check the vector table"
#endif

void __attribute__((naked)) DefaultHandlerForward()
{
    asm volatile("bx %0" : : "r" (Default_Handler));
}

// Forward declaration of the specific IRQ handlers. When the application
// defines a handler (with the same name), this will automatically take
// precedence over these weak definitions

void __attribute__((weak, alias("DefaultHandlerForward"))) WWDG_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) PVD_PVM_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TAMP_STAMP_LSECSS_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) RTC_WKUP_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) FLASH_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) RCC_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) EXTI0_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) EXTI1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) EXTI2_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) EXTI3_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) EXTI4_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA1_Channel1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA1_Channel2_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA1_Channel3_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA1_Channel4_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA1_Channel5_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA1_Channel6_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA1_Channel7_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) ADC1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) USB_HP_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) USB_LP_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) C2SEV_PWR_C2H_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) COMP_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) EXTI9_5_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TIM1_BRK_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TIM1_UP_TIM16_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TIM1_TRG_COM_TIM17_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TIM1_CC_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TIM2_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) PKA_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) I2C1_EV_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) I2C1_ER_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) I2C3_EV_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) I2C3_ER_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) SPI1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) SPI2_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) USART1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) LPUART1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) SAI1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TSC_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) EXTI15_10_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) RTC_Alarm_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) CRS_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) PWR_SOTF_BLEACT_802ACT_RFPHASE_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) IPCC_C1_RX_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) IPCC_C1_TX_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) HSEM_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) LPTIM1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) LPTIM2_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) LCD_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) QUADSPI_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) AES1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) AES2_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) RNG_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) FPU_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA2_Channel1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA2_Channel2_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA2_Channel3_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA2_Channel4_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA2_Channel5_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA2_Channel6_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA2_Channel7_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMAMUX1_OVR_IRQHandler(void);


extern unsigned int _estack;

typedef void (*const pHandler)(void);

__attribute__ ((section(".isr_vector"),used)) pHandler __isr_vectors[] =
{
      // Core level
      (pHandler)&_estack,
      Reset_Handler,
      NMI_Handler,
      HardFault_Handler,
      MemManage_Handler,
      BusFault_Handler,
      UsageFault_Handler,
      0,
      0,
      0,
      0,
      SVC_Handler,
      DebugMon_Handler,
      0,
      PendSV_Handler,
      SysTick_Handler,

      // Chip level - STM32WB55xx
      WWDG_IRQHandler,
      PVD_PVM_IRQHandler,
      TAMP_STAMP_LSECSS_IRQHandler,
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
      ADC1_IRQHandler,
      USB_HP_IRQHandler,
      USB_LP_IRQHandler,
      C2SEV_PWR_C2H_IRQHandler,
      COMP_IRQHandler,
      EXTI9_5_IRQHandler,
      TIM1_BRK_IRQHandler,
      TIM1_UP_TIM16_IRQHandler,
      TIM1_TRG_COM_TIM17_IRQHandler,
      TIM1_CC_IRQHandler,
      TIM2_IRQHandler,
      PKA_IRQHandler,
      I2C1_EV_IRQHandler,
      I2C1_ER_IRQHandler,
      I2C3_EV_IRQHandler,
      I2C3_ER_IRQHandler,
      SPI1_IRQHandler,
      SPI2_IRQHandler,
      USART1_IRQHandler,
      LPUART1_IRQHandler,
      SAI1_IRQHandler,
      TSC_IRQHandler,
      EXTI15_10_IRQHandler,
      RTC_Alarm_IRQHandler,
      CRS_IRQHandler,
      PWR_SOTF_BLEACT_802ACT_RFPHASE_IRQHandler,
      IPCC_C1_RX_IRQHandler,
      IPCC_C1_TX_IRQHandler,
      HSEM_IRQHandler,
      LPTIM1_IRQHandler,
      LPTIM2_IRQHandler,
      LCD_IRQHandler,
      QUADSPI_IRQHandler,
      AES1_IRQHandler,
      AES2_IRQHandler,
      RNG_IRQHandler,
      FPU_IRQHandler,
      DMA2_Channel1_IRQHandler,
      DMA2_Channel2_IRQHandler,
      DMA2_Channel3_IRQHandler,
      DMA2_Channel4_IRQHandler,
      DMA2_Channel5_IRQHandler,
      DMA2_Channel6_IRQHandler,
      DMA2_Channel7_IRQHandler,
      DMAMUX1_OVR_IRQHandler
};
