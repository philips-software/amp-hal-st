#include "exception_handlers.h"
#include <stdlib.h>

#if !defined(STM32F745xx) && !defined(STM32F746xx) && !defined(STM32F756xx) && !defined(STM32F767xx)
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
void __attribute__((weak, alias("DefaultHandlerForward"))) PVD_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TAMP_STAMP_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) RTC_WKUP_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) FLASH_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) RCC_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) EXTI0_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) EXTI1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) EXTI2_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) EXTI3_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) EXTI4_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA1_Stream0_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA1_Stream1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA1_Stream2_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA1_Stream3_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA1_Stream4_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA1_Stream5_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA1_Stream6_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) ADC_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) EXTI9_5_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TIM1_BRK_TIM9_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TIM1_UP_TIM10_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TIM1_TRG_COM_TIM11_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TIM1_CC_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TIM2_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TIM3_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TIM4_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) I2C1_EV_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) I2C1_ER_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) I2C2_EV_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) I2C2_ER_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) SPI1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) SPI2_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) USART1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) USART2_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) EXTI15_10_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) RTC_Alarm_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) OTG_FS_WKUP_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA1_Stream7_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) FMC_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) SDMMC1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TIM5_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) SPI3_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA2_Stream0_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA2_Stream1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA2_Stream2_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA2_Stream3_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA2_Stream4_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) CAN1_TX_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) CAN1_RX0_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) CAN1_RX1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) CAN1_SCE_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) CAN2_TX_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) CAN2_RX0_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) CAN2_RX1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) CAN2_SCE_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) USART3_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TIM8_BRK_TIM12_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TIM8_UP_TIM13_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TIM8_TRG_COM_TIM14_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TIM8_CC_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) UART4_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) UART5_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TIM6_DAC_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) TIM7_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) OTG_HS_EP1_OUT_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) OTG_HS_EP1_IN_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) OTG_HS_WKUP_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) OTG_HS_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) CRYP_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) HASH_RNG_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) RNG_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) ETH_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) ETH_WKUP_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DCMI_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) OTG_FS_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA2_Stream5_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA2_Stream6_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA2_Stream7_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) USART6_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) I2C3_EV_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) I2C3_ER_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) FPU_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) SPI4_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) SPI5_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) UART7_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) UART8_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) SPI6_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) SAI1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) LTDC_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) LTDC_ER_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DMA2D_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) SAI2_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) QUADSPI_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) LPTIM1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) CEC_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) I2C4_EV_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) I2C4_ER_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) SPDIF_RX_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DSIHOST_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DFSDM1_FLT0_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DFSDM1_FLT1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DFSDM1_FLT2_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) DFSDM1_FLT3_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) SDMMC2_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) CAN3_TX_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) CAN3_RX0_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) CAN3_RX1_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) CAN3_SCE_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) JPEG_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) MDIOS_IRQHandler(void);

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

      // Chip level - STM32F7xx
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
      DMA1_Stream0_IRQHandler,
      DMA1_Stream1_IRQHandler,
      DMA1_Stream2_IRQHandler,
      DMA1_Stream3_IRQHandler,
      DMA1_Stream4_IRQHandler,
      DMA1_Stream5_IRQHandler,
      DMA1_Stream6_IRQHandler,
      ADC_IRQHandler,
      CAN1_TX_IRQHandler,
      CAN1_RX0_IRQHandler,
      CAN1_RX1_IRQHandler,
      CAN1_SCE_IRQHandler,
      EXTI9_5_IRQHandler,
      TIM1_BRK_TIM9_IRQHandler,
      TIM1_UP_TIM10_IRQHandler,
      TIM1_TRG_COM_TIM11_IRQHandler,
      TIM1_CC_IRQHandler,
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
      OTG_FS_WKUP_IRQHandler,
      TIM8_BRK_TIM12_IRQHandler,
      TIM8_UP_TIM13_IRQHandler,
      TIM8_TRG_COM_TIM14_IRQHandler,
      TIM8_CC_IRQHandler,
      DMA1_Stream7_IRQHandler,
      FMC_IRQHandler,
      SDMMC1_IRQHandler,
      TIM5_IRQHandler,
      SPI3_IRQHandler,
      UART4_IRQHandler,
      UART5_IRQHandler,
      TIM6_DAC_IRQHandler,
      TIM7_IRQHandler,
      DMA2_Stream0_IRQHandler,
      DMA2_Stream1_IRQHandler,
      DMA2_Stream2_IRQHandler,
      DMA2_Stream3_IRQHandler,
      DMA2_Stream4_IRQHandler,
      ETH_IRQHandler,
      ETH_WKUP_IRQHandler,
      CAN2_TX_IRQHandler,
      CAN2_RX0_IRQHandler,
      CAN2_RX1_IRQHandler,
      CAN2_SCE_IRQHandler,
      OTG_FS_IRQHandler,
      DMA2_Stream5_IRQHandler,
      DMA2_Stream6_IRQHandler,
      DMA2_Stream7_IRQHandler,
      USART6_IRQHandler,
      I2C3_EV_IRQHandler,
      I2C3_ER_IRQHandler,
      OTG_HS_EP1_OUT_IRQHandler,
      OTG_HS_EP1_IN_IRQHandler,
      OTG_HS_WKUP_IRQHandler,
      OTG_HS_IRQHandler,
      DCMI_IRQHandler,
#ifdef STM32F756xx
      CRYP_IRQHandler,
      HASH_RNG_IRQHandler,
#else
      0,
      RNG_IRQHandler,
#endif
      FPU_IRQHandler,
      UART7_IRQHandler,
      UART8_IRQHandler,
      SPI4_IRQHandler,
      SPI5_IRQHandler,
      SPI6_IRQHandler,
      SAI1_IRQHandler,
#if defined(STM32F746xx) || defined(STM32F756xx) ||defined(STM32F767xx)
      LTDC_IRQHandler,
      LTDC_ER_IRQHandler,
#else
      0,
      0,
#endif
      DMA2D_IRQHandler,
      SAI2_IRQHandler,
      QUADSPI_IRQHandler,
      LPTIM1_IRQHandler,
      CEC_IRQHandler,
      I2C4_EV_IRQHandler,
      I2C4_ER_IRQHandler,
      SPDIF_RX_IRQHandler,
#ifdef STM32F767xx
      DSIHOST_IRQHandler,
      DFSDM1_FLT0_IRQHandler,
      DFSDM1_FLT1_IRQHandler,
      DFSDM1_FLT2_IRQHandler,
      DFSDM1_FLT3_IRQHandler,
      SDMMC2_IRQHandler,
      CAN3_TX_IRQHandler,
      CAN3_RX0_IRQHandler,
      CAN3_RX1_IRQHandler,
      CAN3_SCE_IRQHandler,
      JPEG_IRQHandler,
      MDIOS_IRQHandler,
#endif
};
