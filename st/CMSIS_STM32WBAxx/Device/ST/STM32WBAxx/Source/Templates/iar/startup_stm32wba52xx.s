;********************************************************************************
;* File Name          : startup_stm32wba52xx.s
;* Author             : MCD Application Team
;* Description        : STM32WBA5xx Ultra Low Power Devices vector
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == _iar_program_start,
;*                      - Set the vector table entries with the exceptions ISR
;*                        address.
;*                      - Branches to main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the Cortex-M33 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;********************************************************************************
;* @attention
;*
;* Copyright (c) 2022 STMicroelectronics.
;* All rights reserved.
;*
;* This software is licensed under terms that can be found in the LICENSE file
;* in the root directory of this software component.
;* If no LICENSE file comes with this software, it is provided AS-IS.
;*
;*******************************************************************************
;
;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler                    ; Reset Handler

        DCD     NMI_Handler                      ; NMI Handler
        DCD     HardFault_Handler                ; Hard Fault Handler
        DCD     MemManage_Handler                ; MPU Fault Handler
        DCD     BusFault_Handler                 ; Bus Fault Handler
        DCD     UsageFault_Handler               ; Usage Fault Handler
        DCD     SecureFault_Handler              ; Secure Fault Handler
        DCD     0                                ; Reserved
        DCD     0                                ; Reserved
        DCD     0                                ; Reserved
        DCD     SVC_Handler                      ; SVCall Handler
        DCD     DebugMon_Handler                 ; Debug Monitor Handler
        DCD     0                                ; Reserved
        DCD     PendSV_Handler                   ; PendSV Handler
        DCD     SysTick_Handler                  ; SysTick Handler

         ; External Interrupts
        DCD     WWDG_IRQHandler                  ; Window WatchDog
        DCD     PVD_IRQHandler                   ; PVD through EXTI Line detection Interrupt
        DCD     RTC_IRQHandler                   ; RTC non-secure interrupt
        DCD     RTC_S_IRQHandler                 ; RTC secure interrupt
        DCD     TAMP_IRQHandler                  ; Tamper non-secure interrupt
        DCD     RAMCFG_IRQHandler                ; RAMCFG global
        DCD     FLASH_IRQHandler                 ; FLASH non-secure global interrupt
        DCD     FLASH_S_IRQHandler               ; FLASH secure global interrupt
        DCD     GTZC_IRQHandler                  ; Global TrustZone Controller interrupt
        DCD     RCC_IRQHandler                   ; RCC non-secure global interrupt
        DCD     RCC_S_IRQHandler                 ; RCC secure global interrupt
        DCD     EXTI0_IRQHandler                 ; EXTI Line0 interrupt
        DCD     EXTI1_IRQHandler                 ; EXTI Line1 interrupt
        DCD     EXTI2_IRQHandler                 ; EXTI Line2 interrupt
        DCD     EXTI3_IRQHandler                 ; EXTI Line3 interrupt
        DCD     EXTI4_IRQHandler                 ; EXTI Line4 interrupt
        DCD     EXTI5_IRQHandler                 ; EXTI Line5 interrupt
        DCD     EXTI6_IRQHandler                 ; EXTI Line6 interrupt
        DCD     EXTI7_IRQHandler                 ; EXTI Line7 interrupt
        DCD     EXTI8_IRQHandler                 ; EXTI Line8 interrupt
        DCD     EXTI9_IRQHandler                 ; EXTI Line9 interrupt
        DCD     EXTI10_IRQHandler                ; EXTI Line10 interrupt
        DCD     EXTI11_IRQHandler                ; EXTI Line11 interrupt
        DCD     EXTI12_IRQHandler                ; EXTI Line12 interrupt
        DCD     EXTI13_IRQHandler                ; EXTI Line13 interrupt
        DCD     EXTI14_IRQHandler                ; EXTI Line14 interrupt
        DCD     EXTI15_IRQHandler                ; EXTI Line15 interrupt
        DCD     IWDG_IRQHandler                  ; IWDG global interrupt
        DCD     SAES_IRQHandler                  ; Secure AES global interrupt
        DCD     GPDMA1_Channel0_IRQHandler       ; GPDMA1 Channel 0 global interrupt
        DCD     GPDMA1_Channel1_IRQHandler       ; GPDMA1 Channel 1 global interrupt
        DCD     GPDMA1_Channel2_IRQHandler       ; GPDMA1 Channel 2 global interrupt
        DCD     GPDMA1_Channel3_IRQHandler       ; GPDMA1 Channel 3 global interrupt
        DCD     GPDMA1_Channel4_IRQHandler       ; GPDMA1 Channel 4 global interrupt
        DCD     GPDMA1_Channel5_IRQHandler       ; GPDMA1 Channel 5 global interrupt
        DCD     GPDMA1_Channel6_IRQHandler       ; GPDMA1 Channel 6 global interrupt
        DCD     GPDMA1_Channel7_IRQHandler       ; GPDMA1 Channel 7 global interrupt
        DCD     TIM1_BRK_IRQHandler              ; TIM1 Break interrupt
        DCD     TIM1_UP_IRQHandler               ; TIM1 Update interrupt
        DCD     TIM1_TRG_COM_IRQHandler          ; TIM1 Trigger and Commutation interrupt
        DCD     TIM1_CC_IRQHandler               ; TIM1 Capture Compare interrupt
        DCD     TIM2_IRQHandler                  ; TIM2 global interrupt
        DCD     TIM3_IRQHandler                  ; TIM3 global interrupt
        DCD     I2C1_EV_IRQHandler               ; I2C1 Event interrupt
        DCD     I2C1_ER_IRQHandler               ; I2C1 Error interrupt
        DCD     SPI1_IRQHandler                  ; SPI1 global interrupt
        DCD     USART1_IRQHandler                ; USART1 global interrupt
        DCD     USART2_IRQHandler                ; USART2 global interrupt
        DCD     LPUART1_IRQHandler               ; LPUART1 global interrupt
        DCD     LPTIM1_IRQHandler                ; LPTIM1 global interrupt
        DCD     LPTIM2_IRQHandler                ; LPTIM2 global interrupt
        DCD     TIM16_IRQHandler                 ; TIM16 global interrupt
        DCD     TIM17_IRQHandler                 ; TIM17 global interrupt
        DCD     0                                ; Reserved
        DCD     I2C3_EV_IRQHandler               ; I2C3 event interrupt
        DCD     I2C3_ER_IRQHandler               ; I2C3 error interrupt
        DCD     0                                ; Reserved
        DCD     TSC_IRQHandler                   ; Touch Sense Controller global interrupt
        DCD     AES_IRQHandler                   ; AES global interrupt
        DCD     RNG_IRQHandler                   ; RNG global interrupt
        DCD     FPU_IRQHandler                   ; FPU global interrupt
        DCD     HASH_IRQHandler                  ; HASH global interrupt
        DCD     PKA_IRQHandler                   ; PKA global interrupt
        DCD     SPI3_IRQHandler                  ; SPI3 global interrupt
        DCD     ICACHE_IRQHandler                ; Instruction cache global interrupt
        DCD     ADC4_IRQHandler                  ; LP ADC (12bits) global interrupt
        DCD     RADIO_IRQHandler                 ; 2.4GHz RADIO global interrupt
        DCD     WKUP_IRQHandler                  ; PWR global WKUP pin interrupt
        DCD     HSEM_IRQHandler                  ; HSEM non-secure global interrupt
        DCD     HSEM_S_IRQHandler                ; HSEM secure global interrupt
        DCD     WKUP_S_IRQHandler                ; PWR secure global WKUP pin interrupt

__Vectors_End

__Vectors       EQU   __vector_table
__Vectors_Size  EQU   __Vectors_End - __Vectors

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB
        PUBWEAK Reset_Handler
        SECTION .text:CODE:NOROOT:REORDER(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
MemManage_Handler
        B MemManage_Handler

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
BusFault_Handler
        B BusFault_Handler

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
UsageFault_Handler
        B UsageFault_Handler

        PUBWEAK SecureFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SecureFault_Handler
        B SecureFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
DebugMon_Handler
        B DebugMon_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SysTick_Handler
        B SysTick_Handler

        PUBWEAK WWDG_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
WWDG_IRQHandler
        B WWDG_IRQHandler

        PUBWEAK PVD_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PVD_IRQHandler
        B PVD_IRQHandler

        PUBWEAK RTC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RTC_IRQHandler
        B RTC_IRQHandler

        PUBWEAK RTC_S_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RTC_S_IRQHandler
        B RTC_S_IRQHandler

        PUBWEAK TAMP_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TAMP_IRQHandler
        B TAMP_IRQHandler

        PUBWEAK RAMCFG_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RAMCFG_IRQHandler
        B RAMCFG_IRQHandler

        PUBWEAK FLASH_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
FLASH_IRQHandler
        B FLASH_IRQHandler

        PUBWEAK FLASH_S_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
FLASH_S_IRQHandler
        B FLASH_S_IRQHandler

        PUBWEAK GTZC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
GTZC_IRQHandler
        B GTZC_IRQHandler

        PUBWEAK RCC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RCC_IRQHandler
        B RCC_IRQHandler

        PUBWEAK RCC_S_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RCC_S_IRQHandler
        B RCC_S_IRQHandler

        PUBWEAK EXTI0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI0_IRQHandler
        B EXTI0_IRQHandler

        PUBWEAK EXTI1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI1_IRQHandler
        B EXTI1_IRQHandler

        PUBWEAK EXTI2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI2_IRQHandler
        B EXTI2_IRQHandler

        PUBWEAK EXTI3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI3_IRQHandler
        B EXTI3_IRQHandler

        PUBWEAK EXTI4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI4_IRQHandler
        B EXTI4_IRQHandler

        PUBWEAK EXTI5_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI5_IRQHandler
        B EXTI5_IRQHandler

        PUBWEAK EXTI6_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI6_IRQHandler
        B EXTI6_IRQHandler

        PUBWEAK EXTI7_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI7_IRQHandler
        B EXTI7_IRQHandler

        PUBWEAK EXTI8_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI8_IRQHandler
        B EXTI8_IRQHandler

        PUBWEAK EXTI9_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI9_IRQHandler
        B EXTI9_IRQHandler

        PUBWEAK EXTI10_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI10_IRQHandler
        B EXTI10_IRQHandler

        PUBWEAK EXTI11_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI11_IRQHandler
        B EXTI11_IRQHandler

        PUBWEAK EXTI12_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI12_IRQHandler
        B EXTI12_IRQHandler

        PUBWEAK EXTI13_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI13_IRQHandler
        B EXTI13_IRQHandler

        PUBWEAK EXTI14_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI14_IRQHandler
        B EXTI14_IRQHandler

        PUBWEAK EXTI15_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI15_IRQHandler
        B EXTI15_IRQHandler

        PUBWEAK IWDG_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
IWDG_IRQHandler
        B IWDG_IRQHandler

        PUBWEAK SAES_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SAES_IRQHandler
        B SAES_IRQHandler

        PUBWEAK GPDMA1_Channel0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
GPDMA1_Channel0_IRQHandler
        B GPDMA1_Channel0_IRQHandler

        PUBWEAK GPDMA1_Channel1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
GPDMA1_Channel1_IRQHandler
        B GPDMA1_Channel1_IRQHandler

        PUBWEAK GPDMA1_Channel2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
GPDMA1_Channel2_IRQHandler
        B GPDMA1_Channel2_IRQHandler

        PUBWEAK GPDMA1_Channel3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
GPDMA1_Channel3_IRQHandler
        B GPDMA1_Channel3_IRQHandler

        PUBWEAK GPDMA1_Channel4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
GPDMA1_Channel4_IRQHandler
        B GPDMA1_Channel4_IRQHandler

        PUBWEAK GPDMA1_Channel5_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
GPDMA1_Channel5_IRQHandler
        B GPDMA1_Channel5_IRQHandler

        PUBWEAK GPDMA1_Channel6_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
GPDMA1_Channel6_IRQHandler
        B GPDMA1_Channel6_IRQHandler

        PUBWEAK GPDMA1_Channel7_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
GPDMA1_Channel7_IRQHandler
        B GPDMA1_Channel7_IRQHandler

        PUBWEAK TIM1_BRK_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM1_BRK_IRQHandler
        B TIM1_BRK_IRQHandler

        PUBWEAK TIM1_UP_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM1_UP_IRQHandler
        B TIM1_UP_IRQHandler

        PUBWEAK TIM1_TRG_COM_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM1_TRG_COM_IRQHandler
        B TIM1_TRG_COM_IRQHandler

        PUBWEAK TIM1_CC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM1_CC_IRQHandler
        B TIM1_CC_IRQHandler

        PUBWEAK TIM2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM2_IRQHandler
        B TIM2_IRQHandler

        PUBWEAK TIM3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM3_IRQHandler
        B TIM3_IRQHandler

        PUBWEAK I2C1_EV_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C1_EV_IRQHandler
        B I2C1_EV_IRQHandler

        PUBWEAK I2C1_ER_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C1_ER_IRQHandler
        B I2C1_ER_IRQHandler

        PUBWEAK SPI1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI1_IRQHandler
        B SPI1_IRQHandler

        PUBWEAK USART1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
USART1_IRQHandler
        B USART1_IRQHandler

        PUBWEAK USART2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
USART2_IRQHandler
        B USART2_IRQHandler

        PUBWEAK LPUART1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
LPUART1_IRQHandler
        B LPUART1_IRQHandler

        PUBWEAK LPTIM1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
LPTIM1_IRQHandler
        B LPTIM1_IRQHandler

        PUBWEAK LPTIM2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
LPTIM2_IRQHandler
        B LPTIM2_IRQHandler

        PUBWEAK TIM16_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM16_IRQHandler
        B TIM16_IRQHandler

        PUBWEAK TIM17_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM17_IRQHandler
        B TIM17_IRQHandler

        PUBWEAK I2C3_EV_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C3_EV_IRQHandler
        B I2C3_EV_IRQHandler

        PUBWEAK I2C3_ER_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C3_ER_IRQHandler
        B I2C3_ER_IRQHandler

        PUBWEAK TSC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TSC_IRQHandler
        B TSC_IRQHandler

        PUBWEAK AES_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
AES_IRQHandler
        B AES_IRQHandler

        PUBWEAK RNG_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RNG_IRQHandler
        B RNG_IRQHandler

        PUBWEAK FPU_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
FPU_IRQHandler
        B FPU_IRQHandler

        PUBWEAK HASH_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
HASH_IRQHandler
        B HASH_IRQHandler

        PUBWEAK PKA_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PKA_IRQHandler
        B PKA_IRQHandler

        PUBWEAK SPI3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI3_IRQHandler
        B SPI3_IRQHandler

        PUBWEAK ICACHE_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ICACHE_IRQHandler
        B ICACHE_IRQHandler

        PUBWEAK ADC4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ADC4_IRQHandler
        B ADC4_IRQHandler

        PUBWEAK RADIO_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RADIO_IRQHandler
        B RADIO_IRQHandler

        PUBWEAK WKUP_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
WKUP_IRQHandler
        B WKUP_IRQHandler

        PUBWEAK HSEM_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
HSEM_IRQHandler
        B HSEM_IRQHandler

        PUBWEAK HSEM_S_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
HSEM_S_IRQHandler
        B HSEM_S_IRQHandler

        PUBWEAK WKUP_S_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
WKUP_S_IRQHandler
        B WKUP_S_IRQHandler

        END
