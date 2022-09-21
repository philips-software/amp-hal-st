#include "exception_handlers.h"

// Forward declaration of the specific IRQ handlers. When the application
// defines a handler (with the same name), this will automatically take
// precedence over these weak definitions

void __attribute__((naked)) DefaultHandlerForward()
{
    asm volatile("bx %0" : : "r" (Default_Handler));
}

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
void __attribute__((weak, alias("DefaultHandlerForward"))) FSMC_IRQHandler(void);
void __attribute__((weak, alias("DefaultHandlerForward"))) SDIO_IRQHandler(void);
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
void __attribute__((weak, alias("DefaultHandlerForward"))) HASH_RNG_IRQHandler(void);
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
void __attribute__((weak, alias("DefaultHandlerForward"))) CRYP_IRQHandler(void);
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

extern unsigned int _estack;

typedef void(*const pHandler)(void);

__attribute__((section(".isr_vector"), used)) pHandler __isr_vectors[] =
{
        // Core Level
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

        // Chip Level - STM32F4xx
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

  #if defined(STM32F401xC) || defined(STM32F401xE)
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        EXTI9_5_IRQHandler,     // External Line[9:5]s
        TIM1_BRK_TIM9_IRQHandler,       // TIM1 Break and TIM9
        TIM1_UP_TIM10_IRQHandler,       // TIM1 Update and TIM10
        TIM1_TRG_COM_TIM11_IRQHandler,  // TIM1 Trigger and Commutation and TIM11
        TIM1_CC_IRQHandler,     // TIM1 Capture Compare
        TIM2_IRQHandler,        // TIM2
        TIM3_IRQHandler,        // TIM3
        TIM4_IRQHandler,        // TIM4
        I2C1_EV_IRQHandler,     // I2C1 Event
        I2C1_ER_IRQHandler,     // I2C1 Error
        I2C2_EV_IRQHandler,     // I2C2 Event
        I2C2_ER_IRQHandler,     // I2C2 Error
        SPI1_IRQHandler,        // SPI1
        SPI2_IRQHandler,        // SPI2
        USART1_IRQHandler,      // USART1
        USART2_IRQHandler,      // USART2
        0,      // Reserved
        EXTI15_10_IRQHandler,   // External Line[15:10]s
        RTC_Alarm_IRQHandler,   // RTC Alarm (A and B) through EXTI Line
        OTG_FS_WKUP_IRQHandler, // USB OTG FS Wakeup through EXTI line
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        DMA1_Stream7_IRQHandler,        // DMA1 Stream7
        0,      // Reserved
        SDIO_IRQHandler,        // SDIO
        TIM5_IRQHandler,        // TIM5
        SPI3_IRQHandler,        // SPI3
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        DMA2_Stream0_IRQHandler,        // DMA2 Stream 0
        DMA2_Stream1_IRQHandler,        // DMA2 Stream 1
        DMA2_Stream2_IRQHandler,        // DMA2 Stream 2
        DMA2_Stream3_IRQHandler,        // DMA2 Stream 3
        DMA2_Stream4_IRQHandler,        // DMA2 Stream 4
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        OTG_FS_IRQHandler,      // USB OTG FS
        DMA2_Stream5_IRQHandler,        // DMA2 Stream 5
        DMA2_Stream6_IRQHandler,        // DMA2 Stream 6
        DMA2_Stream7_IRQHandler,        // DMA2 Stream 7
        USART6_IRQHandler,      // USART6
        I2C3_EV_IRQHandler,     // I2C3 event
        I2C3_ER_IRQHandler,     // I2C3 error
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        FPU_IRQHandler, // FPU
        0,      // Reserved
        0,      // Reserved
        SPI4_IRQHandler        // SPI4

  #elif defined (STM32F405xx)

        CAN1_TX_IRQHandler,     // CAN1 TX
        CAN1_RX0_IRQHandler,    // CAN1 RX0
        CAN1_RX1_IRQHandler,    // CAN1 RX1
        CAN1_SCE_IRQHandler,    // CAN1 SCE
        EXTI9_5_IRQHandler,     // External Line[9:5]s
        TIM1_BRK_TIM9_IRQHandler,       // TIM1 Break and TIM9
        TIM1_UP_TIM10_IRQHandler,       // TIM1 Update and TIM10
        TIM1_TRG_COM_TIM11_IRQHandler,  // TIM1 Trigger and Commutation and TIM11
        TIM1_CC_IRQHandler,     // TIM1 Capture Compare
        TIM2_IRQHandler,        // TIM2
        TIM3_IRQHandler,        // TIM3
        TIM4_IRQHandler,        // TIM4
        I2C1_EV_IRQHandler,     // I2C1 Event
        I2C1_ER_IRQHandler,     // I2C1 Error
        I2C2_EV_IRQHandler,     // I2C2 Event
        I2C2_ER_IRQHandler,     // I2C2 Error
        SPI1_IRQHandler,        // SPI1
        SPI2_IRQHandler,        // SPI2
        USART1_IRQHandler,      // USART1
        USART2_IRQHandler,      // USART2
        USART3_IRQHandler,      // USART3
        EXTI15_10_IRQHandler,   // External Line[15:10]s
        RTC_Alarm_IRQHandler,   // RTC Alarm (A and B) through EXTI Line
        OTG_FS_WKUP_IRQHandler, // USB OTG FS Wakeup through EXTI line
        TIM8_BRK_TIM12_IRQHandler,      // TIM8 Break and TIM12
        TIM8_UP_TIM13_IRQHandler,       // TIM8 Update and TIM13
        TIM8_TRG_COM_TIM14_IRQHandler,  // TIM8 Trigger and Commutation and TIM14
        TIM8_CC_IRQHandler,     // TIM8 Capture Compare
        DMA1_Stream7_IRQHandler,        // DMA1 Stream7
        FSMC_IRQHandler,        // FSMC
        SDIO_IRQHandler,        // SDIO
        TIM5_IRQHandler,        // TIM5
        SPI3_IRQHandler,        // SPI3
        UART4_IRQHandler,       // UART4
        UART5_IRQHandler,       // UART5
        TIM6_DAC_IRQHandler,    // TIM6 and DAC1&2 underrun errors
        TIM7_IRQHandler,        // TIM7
        DMA2_Stream0_IRQHandler,        // DMA2 Stream 0
        DMA2_Stream1_IRQHandler,        // DMA2 Stream 1
        DMA2_Stream2_IRQHandler,        // DMA2 Stream 2
        DMA2_Stream3_IRQHandler,        // DMA2 Stream 3
        DMA2_Stream4_IRQHandler,        // DMA2 Stream 4
        0,      // Reserved
        0,      // Reserved
        CAN2_TX_IRQHandler,     // CAN2 TX
        CAN2_RX0_IRQHandler,    // CAN2 RX0
        CAN2_RX1_IRQHandler,    // CAN2 RX1
        CAN2_SCE_IRQHandler,    // CAN2 SCE
        OTG_FS_IRQHandler,      // USB OTG FS
        DMA2_Stream5_IRQHandler,        // DMA2 Stream 5
        DMA2_Stream6_IRQHandler,        // DMA2 Stream 6
        DMA2_Stream7_IRQHandler,        // DMA2 Stream 7
        USART6_IRQHandler,      // USART6
        I2C3_EV_IRQHandler,     // I2C3 event
        I2C3_ER_IRQHandler,     // I2C3 error
        OTG_HS_EP1_OUT_IRQHandler,      // USB OTG HS End Point 1 Out
        OTG_HS_EP1_IN_IRQHandler,       // USB OTG HS End Point 1 In
        OTG_HS_WKUP_IRQHandler, // USB OTG HS Wakeup through EXTI
        OTG_HS_IRQHandler,      // USB OTG HS
        0,      // Reserved
        0,      // Reserved
        HASH_RNG_IRQHandler,    // Hash and Rng
        FPU_IRQHandler // FPU

  #elif defined (STM32F407xx)

        CAN1_TX_IRQHandler,     // CAN1 TX
        CAN1_RX0_IRQHandler,    // CAN1 RX0
        CAN1_RX1_IRQHandler,    // CAN1 RX1
        CAN1_SCE_IRQHandler,    // CAN1 SCE
        EXTI9_5_IRQHandler,     // External Line[9:5]s
        TIM1_BRK_TIM9_IRQHandler,       // TIM1 Break and TIM9
        TIM1_UP_TIM10_IRQHandler,       // TIM1 Update and TIM10
        TIM1_TRG_COM_TIM11_IRQHandler,  // TIM1 Trigger and Commutation and TIM11
        TIM1_CC_IRQHandler,     // TIM1 Capture Compare
        TIM2_IRQHandler,        // TIM2
        TIM3_IRQHandler,        // TIM3
        TIM4_IRQHandler,        // TIM4
        I2C1_EV_IRQHandler,     // I2C1 Event
        I2C1_ER_IRQHandler,     // I2C1 Error
        I2C2_EV_IRQHandler,     // I2C2 Event
        I2C2_ER_IRQHandler,     // I2C2 Error
        SPI1_IRQHandler,        // SPI1
        SPI2_IRQHandler,        // SPI2
        USART1_IRQHandler,      // USART1
        USART2_IRQHandler,      // USART2
        USART3_IRQHandler,      // USART3
        EXTI15_10_IRQHandler,   // External Line[15:10]s
        RTC_Alarm_IRQHandler,   // RTC Alarm (A and B) through EXTI Line
        OTG_FS_WKUP_IRQHandler, // USB OTG FS Wakeup through EXTI line
        TIM8_BRK_TIM12_IRQHandler,      // TIM8 Break and TIM12
        TIM8_UP_TIM13_IRQHandler,       // TIM8 Update and TIM13
        TIM8_TRG_COM_TIM14_IRQHandler,  // TIM8 Trigger and Commutation and TIM14
        TIM8_CC_IRQHandler,     // TIM8 Capture Compare
        DMA1_Stream7_IRQHandler,        // DMA1 Stream7
        FSMC_IRQHandler,        // FSMC
        SDIO_IRQHandler,        // SDIO
        TIM5_IRQHandler,        // TIM5
        SPI3_IRQHandler,        // SPI3
        UART4_IRQHandler,       // UART4
        UART5_IRQHandler,       // UART5
        TIM6_DAC_IRQHandler,    // TIM6 and DAC1&2 underrun errors
        TIM7_IRQHandler,        // TIM7
        DMA2_Stream0_IRQHandler,        // DMA2 Stream 0
        DMA2_Stream1_IRQHandler,        // DMA2 Stream 1
        DMA2_Stream2_IRQHandler,        // DMA2 Stream 2
        DMA2_Stream3_IRQHandler,        // DMA2 Stream 3
        DMA2_Stream4_IRQHandler,        // DMA2 Stream 4
        ETH_IRQHandler, // Ethernet
        ETH_WKUP_IRQHandler,    // Ethernet Wakeup through EXTI line
        CAN2_TX_IRQHandler,     // CAN2 TX
        CAN2_RX0_IRQHandler,    // CAN2 RX0
        CAN2_RX1_IRQHandler,    // CAN2 RX1
        CAN2_SCE_IRQHandler,    // CAN2 SCE
        OTG_FS_IRQHandler,      // USB OTG FS
        DMA2_Stream5_IRQHandler,        // DMA2 Stream 5
        DMA2_Stream6_IRQHandler,        // DMA2 Stream 6
        DMA2_Stream7_IRQHandler,        // DMA2 Stream 7
        USART6_IRQHandler,      // USART6
        I2C3_EV_IRQHandler,     // I2C3 event
        I2C3_ER_IRQHandler,     // I2C3 error
        OTG_HS_EP1_OUT_IRQHandler,      // USB OTG HS End Point 1 Out
        OTG_HS_EP1_IN_IRQHandler,       // USB OTG HS End Point 1 In
        OTG_HS_WKUP_IRQHandler, // USB OTG HS Wakeup through EXTI
        OTG_HS_IRQHandler,      // USB OTG HS
        DCMI_IRQHandler,        // DCMI
        0,      // CRYP crypto
        HASH_RNG_IRQHandler,    // Hash and Rng
        FPU_IRQHandler // FPU

  #elif defined(STM32F411xE)

        WWDG_IRQHandler,        // Window WatchDog
        PVD_IRQHandler, // PVD through EXTI Line detection
        TAMP_STAMP_IRQHandler,  // Tamper and TimeStamps through the EXTI line
        RTC_WKUP_IRQHandler,    // RTC Wakeup through the EXTI line
        FLASH_IRQHandler,       // FLASH
        RCC_IRQHandler, // RCC
        EXTI0_IRQHandler,       // EXTI Line0
        EXTI1_IRQHandler,       // EXTI Line1
        EXTI2_IRQHandler,       // EXTI Line2
        EXTI3_IRQHandler,       // EXTI Line3
        EXTI4_IRQHandler,       // EXTI Line4
        DMA1_Stream0_IRQHandler,        // DMA1 Stream 0
        DMA1_Stream1_IRQHandler,        // DMA1 Stream 1
        DMA1_Stream2_IRQHandler,        // DMA1 Stream 2
        DMA1_Stream3_IRQHandler,        // DMA1 Stream 3
        DMA1_Stream4_IRQHandler,        // DMA1 Stream 4
        DMA1_Stream5_IRQHandler,        // DMA1 Stream 5
        DMA1_Stream6_IRQHandler,        // DMA1 Stream 6
        ADC_IRQHandler, // ADC1, ADC2 and ADC3s
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        EXTI9_5_IRQHandler,     // External Line[9:5]s
        TIM1_BRK_TIM9_IRQHandler,       // TIM1 Break and TIM9
        TIM1_UP_TIM10_IRQHandler,       // TIM1 Update and TIM10
        TIM1_TRG_COM_TIM11_IRQHandler,  // TIM1 Trigger and Commutation and TIM11
        TIM1_CC_IRQHandler,     // TIM1 Capture Compare
        TIM2_IRQHandler,        // TIM2
        TIM3_IRQHandler,        // TIM3
        TIM4_IRQHandler,        // TIM4
        I2C1_EV_IRQHandler,     // I2C1 Event
        I2C1_ER_IRQHandler,     // I2C1 Error
        I2C2_EV_IRQHandler,     // I2C2 Event
        I2C2_ER_IRQHandler,     // I2C2 Error
        SPI1_IRQHandler,        // SPI1
        SPI2_IRQHandler,        // SPI2
        USART1_IRQHandler,      // USART1
        USART2_IRQHandler,      // USART2
        0,      // Reserved
        EXTI15_10_IRQHandler,   // External Line[15:10]s
        RTC_Alarm_IRQHandler,   // RTC Alarm (A and B) through EXTI Line
        OTG_FS_WKUP_IRQHandler, // USB OTG FS Wakeup through EXTI line
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        DMA1_Stream7_IRQHandler,        // DMA1 Stream7
        0,      // Reserved
        SDIO_IRQHandler,        // SDIO
        TIM5_IRQHandler,        // TIM5
        SPI3_IRQHandler,        // SPI3
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        DMA2_Stream0_IRQHandler,        // DMA2 Stream 0
        DMA2_Stream1_IRQHandler,        // DMA2 Stream 1
        DMA2_Stream2_IRQHandler,        // DMA2 Stream 2
        DMA2_Stream3_IRQHandler,        // DMA2 Stream 3
        DMA2_Stream4_IRQHandler,        // DMA2 Stream 4
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        OTG_FS_IRQHandler,      // USB OTG FS
        DMA2_Stream5_IRQHandler,        // DMA2 Stream 5
        DMA2_Stream6_IRQHandler,        // DMA2 Stream 6
        DMA2_Stream7_IRQHandler,        // DMA2 Stream 7
        USART6_IRQHandler,      // USART6
        I2C3_EV_IRQHandler,     // I2C3 event
        I2C3_ER_IRQHandler,     // I2C3 error
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        0,      // Reserved
        FPU_IRQHandler, // FPU
        0,      // Reserved
        0,      // Reserved
        SPI4_IRQHandler,        // SPI4
        SPI5_IRQHandler,        // SPI5

  #elif defined (STM32F415xx)

        CAN1_TX_IRQHandler,     // CAN1 TX
        CAN1_RX0_IRQHandler,    // CAN1 RX0
        CAN1_RX1_IRQHandler,    // CAN1 RX1
        CAN1_SCE_IRQHandler,    // CAN1 SCE
        EXTI9_5_IRQHandler,     // External Line[9:5]s
        TIM1_BRK_TIM9_IRQHandler,       // TIM1 Break and TIM9
        TIM1_UP_TIM10_IRQHandler,       // TIM1 Update and TIM10
        TIM1_TRG_COM_TIM11_IRQHandler,  // TIM1 Trigger and Commutation and TIM11
        TIM1_CC_IRQHandler,     // TIM1 Capture Compare
        TIM2_IRQHandler,        // TIM2
        TIM3_IRQHandler,        // TIM3
        TIM4_IRQHandler,        // TIM4
        I2C1_EV_IRQHandler,     // I2C1 Event
        I2C1_ER_IRQHandler,     // I2C1 Error
        I2C2_EV_IRQHandler,     // I2C2 Event
        I2C2_ER_IRQHandler,     // I2C2 Error
        SPI1_IRQHandler,        // SPI1
        SPI2_IRQHandler,        // SPI2
        USART1_IRQHandler,      // USART1
        USART2_IRQHandler,      // USART2
        USART3_IRQHandler,      // USART3
        EXTI15_10_IRQHandler,   // External Line[15:10]s
        RTC_Alarm_IRQHandler,   // RTC Alarm (A and B) through EXTI Line
        OTG_FS_WKUP_IRQHandler, // USB OTG FS Wakeup through EXTI line
        TIM8_BRK_TIM12_IRQHandler,      // TIM8 Break and TIM12
        TIM8_UP_TIM13_IRQHandler,       // TIM8 Update and TIM13
        TIM8_TRG_COM_TIM14_IRQHandler,  // TIM8 Trigger and Commutation and TIM14
        TIM8_CC_IRQHandler,     // TIM8 Capture Compare
        DMA1_Stream7_IRQHandler,        // DMA1 Stream7
        FSMC_IRQHandler,        // FSMC
        SDIO_IRQHandler,        // SDIO
        TIM5_IRQHandler,        // TIM5
        SPI3_IRQHandler,        // SPI3
        UART4_IRQHandler,       // UART4
        UART5_IRQHandler,       // UART5
        TIM6_DAC_IRQHandler,    // TIM6 and DAC1&2 underrun errors
        TIM7_IRQHandler,        // TIM7
        DMA2_Stream0_IRQHandler,        // DMA2 Stream 0
        DMA2_Stream1_IRQHandler,        // DMA2 Stream 1
        DMA2_Stream2_IRQHandler,        // DMA2 Stream 2
        DMA2_Stream3_IRQHandler,        // DMA2 Stream 3
        DMA2_Stream4_IRQHandler,        // DMA2 Stream 4
        0,      // Reserved
        0,      // Reserved
        CAN2_TX_IRQHandler,     // CAN2 TX
        CAN2_RX0_IRQHandler,    // CAN2 RX0
        CAN2_RX1_IRQHandler,    // CAN2 RX1
        CAN2_SCE_IRQHandler,    // CAN2 SCE
        OTG_FS_IRQHandler,      // USB OTG FS
        DMA2_Stream5_IRQHandler,        // DMA2 Stream 5
        DMA2_Stream6_IRQHandler,        // DMA2 Stream 6
        DMA2_Stream7_IRQHandler,        // DMA2 Stream 7
        USART6_IRQHandler,      // USART6
        I2C3_EV_IRQHandler,     // I2C3 event
        I2C3_ER_IRQHandler,     // I2C3 error
        OTG_HS_EP1_OUT_IRQHandler,      // USB OTG HS End Point 1 Out
        OTG_HS_EP1_IN_IRQHandler,       // USB OTG HS End Point 1 In
        OTG_HS_WKUP_IRQHandler, // USB OTG HS Wakeup through EXTI
        OTG_HS_IRQHandler,      // USB OTG HS
        0,      // Reserved
        CRYP_IRQHandler,        // CRYP crypto
        HASH_RNG_IRQHandler,    // Hash and Rng
        FPU_IRQHandler // FPU

  #elif defined (STM32F417xx)

        CAN1_TX_IRQHandler,     // CAN1 TX
        CAN1_RX0_IRQHandler,    // CAN1 RX0
        CAN1_RX1_IRQHandler,    // CAN1 RX1
        CAN1_SCE_IRQHandler,    // CAN1 SCE
        EXTI9_5_IRQHandler,     // External Line[9:5]s
        TIM1_BRK_TIM9_IRQHandler,       // TIM1 Break and TIM9
        TIM1_UP_TIM10_IRQHandler,       // TIM1 Update and TIM10
        TIM1_TRG_COM_TIM11_IRQHandler,  // TIM1 Trigger and Commutation and TIM11
        TIM1_CC_IRQHandler,     // TIM1 Capture Compare
        TIM2_IRQHandler,        // TIM2
        TIM3_IRQHandler,        // TIM3
        TIM4_IRQHandler,        // TIM4
        I2C1_EV_IRQHandler,     // I2C1 Event
        I2C1_ER_IRQHandler,     // I2C1 Error
        I2C2_EV_IRQHandler,     // I2C2 Event
        I2C2_ER_IRQHandler,     // I2C2 Error
        SPI1_IRQHandler,        // SPI1
        SPI2_IRQHandler,        // SPI2
        USART1_IRQHandler,      // USART1
        USART2_IRQHandler,      // USART2
        USART3_IRQHandler,      // USART3
        EXTI15_10_IRQHandler,   // External Line[15:10]s
        RTC_Alarm_IRQHandler,   // RTC Alarm (A and B) through EXTI Line
        OTG_FS_WKUP_IRQHandler, // USB OTG FS Wakeup through EXTI line
        TIM8_BRK_TIM12_IRQHandler,      // TIM8 Break and TIM12
        TIM8_UP_TIM13_IRQHandler,       // TIM8 Update and TIM13
        TIM8_TRG_COM_TIM14_IRQHandler,  // TIM8 Trigger and Commutation and TIM14
        TIM8_CC_IRQHandler,     // TIM8 Capture Compare
        DMA1_Stream7_IRQHandler,        // DMA1 Stream7
        FSMC_IRQHandler,        // FSMC
        SDIO_IRQHandler,        // SDIO
        TIM5_IRQHandler,        // TIM5
        SPI3_IRQHandler,        // SPI3
        UART4_IRQHandler,       // UART4
        UART5_IRQHandler,       // UART5
        TIM6_DAC_IRQHandler,    // TIM6 and DAC1&2 underrun errors
        TIM7_IRQHandler,        // TIM7
        DMA2_Stream0_IRQHandler,        // DMA2 Stream 0
        DMA2_Stream1_IRQHandler,        // DMA2 Stream 1
        DMA2_Stream2_IRQHandler,        // DMA2 Stream 2
        DMA2_Stream3_IRQHandler,        // DMA2 Stream 3
        DMA2_Stream4_IRQHandler,        // DMA2 Stream 4
        ETH_IRQHandler, // Ethernet
        ETH_WKUP_IRQHandler,    // Ethernet Wakeup through EXTI line
        CAN2_TX_IRQHandler,     // CAN2 TX
        CAN2_RX0_IRQHandler,    // CAN2 RX0
        CAN2_RX1_IRQHandler,    // CAN2 RX1
        CAN2_SCE_IRQHandler,    // CAN2 SCE
        OTG_FS_IRQHandler,      // USB OTG FS
        DMA2_Stream5_IRQHandler,        // DMA2 Stream 5
        DMA2_Stream6_IRQHandler,        // DMA2 Stream 6
        DMA2_Stream7_IRQHandler,        // DMA2 Stream 7
        USART6_IRQHandler,      // USART6
        I2C3_EV_IRQHandler,     // I2C3 event
        I2C3_ER_IRQHandler,     // I2C3 error
        OTG_HS_EP1_OUT_IRQHandler,      // USB OTG HS End Point 1 Out
        OTG_HS_EP1_IN_IRQHandler,       // USB OTG HS End Point 1 In
        OTG_HS_WKUP_IRQHandler, // USB OTG HS Wakeup through EXTI
        OTG_HS_IRQHandler,      // USB OTG HS
        DCMI_IRQHandler,        // DCMI
        CRYP_IRQHandler,        // CRYP crypto
        HASH_RNG_IRQHandler,    // Hash and Rng
        FPU_IRQHandler // FPU

  #elif defined(STM32F427xx)

        CAN1_TX_IRQHandler,     // CAN1 TX
        CAN1_RX0_IRQHandler,    // CAN1 RX0
        CAN1_RX1_IRQHandler,    // CAN1 RX1
        CAN1_SCE_IRQHandler,    // CAN1 SCE
        EXTI9_5_IRQHandler,     // External Line[9:5]s
        TIM1_BRK_TIM9_IRQHandler,       // TIM1 Break and TIM9
        TIM1_UP_TIM10_IRQHandler,       // TIM1 Update and TIM10
        TIM1_TRG_COM_TIM11_IRQHandler,  // TIM1 Trigger and Commutation and TIM11
        TIM1_CC_IRQHandler,     // TIM1 Capture Compare
        TIM2_IRQHandler,        // TIM2
        TIM3_IRQHandler,        // TIM3
        TIM4_IRQHandler,        // TIM4
        I2C1_EV_IRQHandler,     // I2C1 Event
        I2C1_ER_IRQHandler,     // I2C1 Error
        I2C2_EV_IRQHandler,     // I2C2 Event
        I2C2_ER_IRQHandler,     // I2C2 Error
        SPI1_IRQHandler,        // SPI1
        SPI2_IRQHandler,        // SPI2
        USART1_IRQHandler,      // USART1
        USART2_IRQHandler,      // USART2
        USART3_IRQHandler,      // USART3
        EXTI15_10_IRQHandler,   // External Line[15:10]s
        RTC_Alarm_IRQHandler,   // RTC Alarm (A and B) through EXTI Line
        OTG_FS_WKUP_IRQHandler, // USB OTG FS Wakeup through EXTI line
        TIM8_BRK_TIM12_IRQHandler,      // TIM8 Break and TIM12
        TIM8_UP_TIM13_IRQHandler,       // TIM8 Update and TIM13
        TIM8_TRG_COM_TIM14_IRQHandler,  // TIM8 Trigger and Commutation and TIM14
        TIM8_CC_IRQHandler,     // TIM8 Capture Compare
        DMA1_Stream7_IRQHandler,        // DMA1 Stream7
        FMC_IRQHandler, // FMC
        SDIO_IRQHandler,        // SDIO
        TIM5_IRQHandler,        // TIM5
        SPI3_IRQHandler,        // SPI3
        UART4_IRQHandler,       // UART4
        UART5_IRQHandler,       // UART5
        TIM6_DAC_IRQHandler,    // TIM6 and DAC1&2 underrun errors
        TIM7_IRQHandler,        // TIM7
        DMA2_Stream0_IRQHandler,        // DMA2 Stream 0
        DMA2_Stream1_IRQHandler,        // DMA2 Stream 1
        DMA2_Stream2_IRQHandler,        // DMA2 Stream 2
        DMA2_Stream3_IRQHandler,        // DMA2 Stream 3
        DMA2_Stream4_IRQHandler,        // DMA2 Stream 4
        ETH_IRQHandler, // Ethernet
        ETH_WKUP_IRQHandler,    // Ethernet Wakeup through EXTI line
        CAN2_TX_IRQHandler,     // CAN2 TX
        CAN2_RX0_IRQHandler,    // CAN2 RX0
        CAN2_RX1_IRQHandler,    // CAN2 RX1
        CAN2_SCE_IRQHandler,    // CAN2 SCE
        OTG_FS_IRQHandler,      // USB OTG FS
        DMA2_Stream5_IRQHandler,        // DMA2 Stream 5
        DMA2_Stream6_IRQHandler,        // DMA2 Stream 6
        DMA2_Stream7_IRQHandler,        // DMA2 Stream 7
        USART6_IRQHandler,      // USART6
        I2C3_EV_IRQHandler,     // I2C3 event
        I2C3_ER_IRQHandler,     // I2C3 error
        OTG_HS_EP1_OUT_IRQHandler,      // USB OTG HS End Point 1 Out
        OTG_HS_EP1_IN_IRQHandler,       // USB OTG HS End Point 1 In
        OTG_HS_WKUP_IRQHandler, // USB OTG HS Wakeup through EXTI
        OTG_HS_IRQHandler,      // USB OTG HS
        DCMI_IRQHandler,        // DCMI
        0,      // Reserved
        HASH_RNG_IRQHandler,    // Hash and Rng
        FPU_IRQHandler, // FPU
        UART7_IRQHandler,       // UART7
        UART8_IRQHandler,       // UART8
        SPI4_IRQHandler,        // SPI4
        SPI5_IRQHandler,        // SPI5
        SPI6_IRQHandler,        // SPI6
        SAI1_IRQHandler,        // SAI1
        0,      // Reserved
        0,      // Reserved
        DMA2D_IRQHandler       // DMA2D

  #elif defined(STM32F429xx)

        CAN1_TX_IRQHandler,     // CAN1 TX
        CAN1_RX0_IRQHandler,    // CAN1 RX0
        CAN1_RX1_IRQHandler,    // CAN1 RX1
        CAN1_SCE_IRQHandler,    // CAN1 SCE
        EXTI9_5_IRQHandler,     // External Line[9:5]s
        TIM1_BRK_TIM9_IRQHandler,       // TIM1 Break and TIM9
        TIM1_UP_TIM10_IRQHandler,       // TIM1 Update and TIM10
        TIM1_TRG_COM_TIM11_IRQHandler,  // TIM1 Trigger and Commutation and TIM11
        TIM1_CC_IRQHandler,     // TIM1 Capture Compare
        TIM2_IRQHandler,        // TIM2
        TIM3_IRQHandler,        // TIM3
        TIM4_IRQHandler,        // TIM4
        I2C1_EV_IRQHandler,     // I2C1 Event
        I2C1_ER_IRQHandler,     // I2C1 Error
        I2C2_EV_IRQHandler,     // I2C2 Event
        I2C2_ER_IRQHandler,     // I2C2 Error
        SPI1_IRQHandler,        // SPI1
        SPI2_IRQHandler,        // SPI2
        USART1_IRQHandler,      // USART1
        USART2_IRQHandler,      // USART2
        USART3_IRQHandler,      // USART3
        EXTI15_10_IRQHandler,   // External Line[15:10]s
        RTC_Alarm_IRQHandler,   // RTC Alarm (A and B) through EXTI Line
        OTG_FS_WKUP_IRQHandler, // USB OTG FS Wakeup through EXTI line
        TIM8_BRK_TIM12_IRQHandler,      // TIM8 Break and TIM12
        TIM8_UP_TIM13_IRQHandler,       // TIM8 Update and TIM13
        TIM8_TRG_COM_TIM14_IRQHandler,  // TIM8 Trigger and Commutation and TIM14
        TIM8_CC_IRQHandler,     // TIM8 Capture Compare
        DMA1_Stream7_IRQHandler,        // DMA1 Stream7
        FMC_IRQHandler, // FMC
        SDIO_IRQHandler,        // SDIO
        TIM5_IRQHandler,        // TIM5
        SPI3_IRQHandler,        // SPI3
        UART4_IRQHandler,       // UART4
        UART5_IRQHandler,       // UART5
        TIM6_DAC_IRQHandler,    // TIM6 and DAC1&2 underrun errors
        TIM7_IRQHandler,        // TIM7
        DMA2_Stream0_IRQHandler,        // DMA2 Stream 0
        DMA2_Stream1_IRQHandler,        // DMA2 Stream 1
        DMA2_Stream2_IRQHandler,        // DMA2 Stream 2
        DMA2_Stream3_IRQHandler,        // DMA2 Stream 3
        DMA2_Stream4_IRQHandler,        // DMA2 Stream 4
        ETH_IRQHandler, // Ethernet
        ETH_WKUP_IRQHandler,    // Ethernet Wakeup through EXTI line
        CAN2_TX_IRQHandler,     // CAN2 TX
        CAN2_RX0_IRQHandler,    // CAN2 RX0
        CAN2_RX1_IRQHandler,    // CAN2 RX1
        CAN2_SCE_IRQHandler,    // CAN2 SCE
        OTG_FS_IRQHandler,      // USB OTG FS
        DMA2_Stream5_IRQHandler,        // DMA2 Stream 5
        DMA2_Stream6_IRQHandler,        // DMA2 Stream 6
        DMA2_Stream7_IRQHandler,        // DMA2 Stream 7
        USART6_IRQHandler,      // USART6
        I2C3_EV_IRQHandler,     // I2C3 event
        I2C3_ER_IRQHandler,     // I2C3 error
        OTG_HS_EP1_OUT_IRQHandler,      // USB OTG HS End Point 1 Out
        OTG_HS_EP1_IN_IRQHandler,       // USB OTG HS End Point 1 In
        OTG_HS_WKUP_IRQHandler, // USB OTG HS Wakeup through EXTI
        OTG_HS_IRQHandler,      // USB OTG HS
        DCMI_IRQHandler,        // DCMI
        0,      // Reserved
        HASH_RNG_IRQHandler,    // Hash and Rng
        FPU_IRQHandler, // FPU
        UART7_IRQHandler,       // UART7
        UART8_IRQHandler,       // UART8
        SPI4_IRQHandler,        // SPI4
        SPI5_IRQHandler,        // SPI5
        SPI6_IRQHandler,        // SPI6
        SAI1_IRQHandler,        // SAI1
        LTDC_IRQHandler,        // LTDC_IRQHandler
        LTDC_ER_IRQHandler,     // LTDC_ER_IRQHandler
        DMA2D_IRQHandler       // DMA2D

  #elif defined(STM32F437xx)

        CAN1_TX_IRQHandler,     // CAN1 TX
        CAN1_RX0_IRQHandler,    // CAN1 RX0
        CAN1_RX1_IRQHandler,    // CAN1 RX1
        CAN1_SCE_IRQHandler,    // CAN1 SCE
        EXTI9_5_IRQHandler,     // External Line[9:5]s
        TIM1_BRK_TIM9_IRQHandler,       // TIM1 Break and TIM9
        TIM1_UP_TIM10_IRQHandler,       // TIM1 Update and TIM10
        TIM1_TRG_COM_TIM11_IRQHandler,  // TIM1 Trigger and Commutation and TIM11
        TIM1_CC_IRQHandler,     // TIM1 Capture Compare
        TIM2_IRQHandler,        // TIM2
        TIM3_IRQHandler,        // TIM3
        TIM4_IRQHandler,        // TIM4
        I2C1_EV_IRQHandler,     // I2C1 Event
        I2C1_ER_IRQHandler,     // I2C1 Error
        I2C2_EV_IRQHandler,     // I2C2 Event
        I2C2_ER_IRQHandler,     // I2C2 Error
        SPI1_IRQHandler,        // SPI1
        SPI2_IRQHandler,        // SPI2
        USART1_IRQHandler,      // USART1
        USART2_IRQHandler,      // USART2
        USART3_IRQHandler,      // USART3
        EXTI15_10_IRQHandler,   // External Line[15:10]s
        RTC_Alarm_IRQHandler,   // RTC Alarm (A and B) through EXTI Line
        OTG_FS_WKUP_IRQHandler, // USB OTG FS Wakeup through EXTI line
        TIM8_BRK_TIM12_IRQHandler,      // TIM8 Break and TIM12
        TIM8_UP_TIM13_IRQHandler,       // TIM8 Update and TIM13
        TIM8_TRG_COM_TIM14_IRQHandler,  // TIM8 Trigger and Commutation and TIM14
        TIM8_CC_IRQHandler,     // TIM8 Capture Compare
        DMA1_Stream7_IRQHandler,        // DMA1 Stream7
        FMC_IRQHandler, // FMC
        SDIO_IRQHandler,        // SDIO
        TIM5_IRQHandler,        // TIM5
        SPI3_IRQHandler,        // SPI3
        UART4_IRQHandler,       // UART4
        UART5_IRQHandler,       // UART5
        TIM6_DAC_IRQHandler,    // TIM6 and DAC1&2 underrun errors
        TIM7_IRQHandler,        // TIM7
        DMA2_Stream0_IRQHandler,        // DMA2 Stream 0
        DMA2_Stream1_IRQHandler,        // DMA2 Stream 1
        DMA2_Stream2_IRQHandler,        // DMA2 Stream 2
        DMA2_Stream3_IRQHandler,        // DMA2 Stream 3
        DMA2_Stream4_IRQHandler,        // DMA2 Stream 4
        ETH_IRQHandler, // Ethernet
        ETH_WKUP_IRQHandler,    // Ethernet Wakeup through EXTI line
        CAN2_TX_IRQHandler,     // CAN2 TX
        CAN2_RX0_IRQHandler,    // CAN2 RX0
        CAN2_RX1_IRQHandler,    // CAN2 RX1
        CAN2_SCE_IRQHandler,    // CAN2 SCE
        OTG_FS_IRQHandler,      // USB OTG FS
        DMA2_Stream5_IRQHandler,        // DMA2 Stream 5
        DMA2_Stream6_IRQHandler,        // DMA2 Stream 6
        DMA2_Stream7_IRQHandler,        // DMA2 Stream 7
        USART6_IRQHandler,      // USART6
        I2C3_EV_IRQHandler,     // I2C3 event
        I2C3_ER_IRQHandler,     // I2C3 error
        OTG_HS_EP1_OUT_IRQHandler,      // USB OTG HS End Point 1 Out
        OTG_HS_EP1_IN_IRQHandler,       // USB OTG HS End Point 1 In
        OTG_HS_WKUP_IRQHandler, // USB OTG HS Wakeup through EXTI
        OTG_HS_IRQHandler,      // USB OTG HS
        DCMI_IRQHandler,        // DCMI
        CRYP_IRQHandler,        // CRYP crypto
        HASH_RNG_IRQHandler,    // Hash and Rng
        FPU_IRQHandler, // FPU
        UART7_IRQHandler,       // UART7
        UART8_IRQHandler,       // UART8
        SPI4_IRQHandler,        // SPI4
        SPI5_IRQHandler,        // SPI5
        SPI6_IRQHandler,        // SPI6
        SAI1_IRQHandler,        // SAI1
        0,      // Reserved
        0,      // Reserved
        DMA2D_IRQHandler       // DMA2D

  #elif defined(STM32F439xx)

        CAN1_TX_IRQHandler,     // CAN1 TX
        CAN1_RX0_IRQHandler,    // CAN1 RX0
        CAN1_RX1_IRQHandler,    // CAN1 RX1
        CAN1_SCE_IRQHandler,    // CAN1 SCE
        EXTI9_5_IRQHandler,     // External Line[9:5]s
        TIM1_BRK_TIM9_IRQHandler,       // TIM1 Break and TIM9
        TIM1_UP_TIM10_IRQHandler,       // TIM1 Update and TIM10
        TIM1_TRG_COM_TIM11_IRQHandler,  // TIM1 Trigger and Commutation and TIM11
        TIM1_CC_IRQHandler,     // TIM1 Capture Compare
        TIM2_IRQHandler,        // TIM2
        TIM3_IRQHandler,        // TIM3
        TIM4_IRQHandler,        // TIM4
        I2C1_EV_IRQHandler,     // I2C1 Event
        I2C1_ER_IRQHandler,     // I2C1 Error
        I2C2_EV_IRQHandler,     // I2C2 Event
        I2C2_ER_IRQHandler,     // I2C2 Error
        SPI1_IRQHandler,        // SPI1
        SPI2_IRQHandler,        // SPI2
        USART1_IRQHandler,      // USART1
        USART2_IRQHandler,      // USART2
        USART3_IRQHandler,      // USART3
        EXTI15_10_IRQHandler,   // External Line[15:10]s
        RTC_Alarm_IRQHandler,   // RTC Alarm (A and B) through EXTI Line
        OTG_FS_WKUP_IRQHandler, // USB OTG FS Wakeup through EXTI line
        TIM8_BRK_TIM12_IRQHandler,      // TIM8 Break and TIM12
        TIM8_UP_TIM13_IRQHandler,       // TIM8 Update and TIM13
        TIM8_TRG_COM_TIM14_IRQHandler,  // TIM8 Trigger and Commutation and TIM14
        TIM8_CC_IRQHandler,     // TIM8 Capture Compare
        DMA1_Stream7_IRQHandler,        // DMA1 Stream7
        FMC_IRQHandler, // FMC
        SDIO_IRQHandler,        // SDIO
        TIM5_IRQHandler,        // TIM5
        SPI3_IRQHandler,        // SPI3
        UART4_IRQHandler,       // UART4
        UART5_IRQHandler,       // UART5
        TIM6_DAC_IRQHandler,    // TIM6 and DAC1&2 underrun errors
        TIM7_IRQHandler,        // TIM7
        DMA2_Stream0_IRQHandler,        // DMA2 Stream 0
        DMA2_Stream1_IRQHandler,        // DMA2 Stream 1
        DMA2_Stream2_IRQHandler,        // DMA2 Stream 2
        DMA2_Stream3_IRQHandler,        // DMA2 Stream 3
        DMA2_Stream4_IRQHandler,        // DMA2 Stream 4
        ETH_IRQHandler, // Ethernet
        ETH_WKUP_IRQHandler,    // Ethernet Wakeup through EXTI line
        CAN2_TX_IRQHandler,     // CAN2 TX
        CAN2_RX0_IRQHandler,    // CAN2 RX0
        CAN2_RX1_IRQHandler,    // CAN2 RX1
        CAN2_SCE_IRQHandler,    // CAN2 SCE
        OTG_FS_IRQHandler,      // USB OTG FS
        DMA2_Stream5_IRQHandler,        // DMA2 Stream 5
        DMA2_Stream6_IRQHandler,        // DMA2 Stream 6
        DMA2_Stream7_IRQHandler,        // DMA2 Stream 7
        USART6_IRQHandler,      // USART6
        I2C3_EV_IRQHandler,     // I2C3 event
        I2C3_ER_IRQHandler,     // I2C3 error
        OTG_HS_EP1_OUT_IRQHandler,      // USB OTG HS End Point 1 Out
        OTG_HS_EP1_IN_IRQHandler,       // USB OTG HS End Point 1 In
        OTG_HS_WKUP_IRQHandler, // USB OTG HS Wakeup through EXTI
        OTG_HS_IRQHandler,      // USB OTG HS
        DCMI_IRQHandler,        // DCMI
        CRYP_IRQHandler,        // CRYP crypto
        HASH_RNG_IRQHandler,    // Hash and Rng
        FPU_IRQHandler, // FPU
        UART7_IRQHandler,       // UART7
        UART8_IRQHandler,       // UART8
        SPI4_IRQHandler,        // SPI4
        SPI5_IRQHandler,        // SPI5
        SPI6_IRQHandler,        // SPI6
        SAI1_IRQHandler,        // SAI1
        LTDC_IRQHandler,        // LTDC
        LTDC_ER_IRQHandler,     // LTDC error
        DMA2D_IRQHandler       // DMA2D

#else
#error "missing vectors"
#endif
};
