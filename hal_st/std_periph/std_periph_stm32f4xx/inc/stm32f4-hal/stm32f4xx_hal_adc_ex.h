/**
  ******************************************************************************
  * @file    stm32f4xx_hal_adc_ex.h
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    14-August-2015
  * @brief   Header file of ADC HAL module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_ADC_EX_H
#define __STM32F4xx_ADC_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @addtogroup ADCEx
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/
/** @defgroup ADCEx_Exported_Types ADC Exported Types
  * @{
  */
   
/** 
  * @brief ADC Configuration injected Channel structure definition
  */ 
typedef struct 
{
  uint32_t InjectedChannel;                /*!< Configure the ADC injected channel.
                                                This parameter can be a value of @ref ADC_channels */ 
  uint32_t InjectedRank;                   /*!< The rank in the injected group sequencer
                                                This parameter must be a number between Min_Data = 1 and Max_Data = 4. */ 
  uint32_t InjectedSamplingTime;           /*!< The sample time value to be set for the selected channel.
                                                This parameter can be a value of @ref ADC_sampling_times */
  uint32_t InjectedOffset;                 /*!< Defines the offset to be subtracted from the raw converted data when convert injected channels.
                                                This parameter must be a number between Min_Data = 0x000 and Max_Data = 0xFFF. */
  uint32_t InjectedNbrOfConversion;        /*!< Specifies the number of ADC conversions that will be done using the sequencer for
                                                injected channel group.
                                                This parameter must be a number between Min_Data = 1 and Max_Data = 4. */
  uint32_t AutoInjectedConv;               /*!< Enables or disables the selected ADC automatic injected group 
                                                conversion after regular one */
  uint32_t InjectedDiscontinuousConvMode;  /*!< Specifies whether the conversion is performed in Discontinuous mode or not for injected channels.
                                                This parameter can be set to ENABLE or DISABLE. */
  uint32_t ExternalTrigInjecConvEdge;      /*!< Select the external trigger edge and enable the trigger of an injected channels. 
                                                This parameter can be a value of @ref ADCEx_External_trigger_edge_Injected */
  uint32_t ExternalTrigInjecConv;          /*!< Select the external event used to trigger the start of conversion of a injected channels.
                                                This parameter can be a value of @ref ADCEx_External_trigger_Source_Injected */
}ADC_InjectionConfTypeDef;

/** 
  * @brief ADC Configuration multi-mode structure definition  
  */ 
typedef struct
{
  uint32_t Mode;              /*!< Configures the ADC to operate in independent or multi mode. 
                                   This parameter can be a value of @ref ADCEx_Common_mode */
  uint32_t DMAAccessMode;     /*!< Configures the Direct memory access mode for multi ADC mode.
                                   This parameter can be a value of @ref ADCEx_Direct_memory_access_mode_for_multi_mode */
  uint32_t TwoSamplingDelay;  /*!< Configures the Delay between 2 sampling phases.
                                   This parameter can be a value of @ref ADC_delay_between_2_sampling_phases */
}ADC_MultiModeTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup ADCEx_Exported_Constants ADC Exported Constants
  * @{
  */

/** @defgroup ADCEx_Common_mode ADC Common Mode
  * @{
  */ 
#define ADC_MODE_INDEPENDENT                  ((uint32_t)0x00000000)      
#define ADC_DUALMODE_REGSIMULT_INJECSIMULT    ((uint32_t)ADC_CCR_MULTI_0)
#define ADC_DUALMODE_REGSIMULT_ALTERTRIG      ((uint32_t)ADC_CCR_MULTI_1)
#define ADC_DUALMODE_INJECSIMULT              ((uint32_t)(ADC_CCR_MULTI_2 | ADC_CCR_MULTI_0))
#define ADC_DUALMODE_REGSIMULT                ((uint32_t)(ADC_CCR_MULTI_2 | ADC_CCR_MULTI_1))
#define ADC_DUALMODE_INTERL                   ((uint32_t)(ADC_CCR_MULTI_2 | ADC_CCR_MULTI_1 | ADC_CCR_MULTI_0))
#define ADC_DUALMODE_ALTERTRIG                ((uint32_t)(ADC_CCR_MULTI_3 | ADC_CCR_MULTI_0))
#define ADC_TRIPLEMODE_REGSIMULT_INJECSIMULT  ((uint32_t)(ADC_CCR_MULTI_4 | ADC_CCR_MULTI_0))
#define ADC_TRIPLEMODE_REGSIMULT_AlterTrig    ((uint32_t)(ADC_CCR_MULTI_4 | ADC_CCR_MULTI_1))
#define ADC_TRIPLEMODE_INJECSIMULT            ((uint32_t)(ADC_CCR_MULTI_4 | ADC_CCR_MULTI_2 | ADC_CCR_MULTI_0))
#define ADC_TRIPLEMODE_REGSIMULT              ((uint32_t)(ADC_CCR_MULTI_4 | ADC_CCR_MULTI_2 | ADC_CCR_MULTI_1))
#define ADC_TRIPLEMODE_INTERL                 ((uint32_t)(ADC_CCR_MULTI_4 | ADC_CCR_MULTI_2 | ADC_CCR_MULTI_1 | ADC_CCR_MULTI_0))
#define ADC_TRIPLEMODE_ALTERTRIG              ((uint32_t)(ADC_CCR_MULTI_4 | ADC_CCR_MULTI_3 | ADC_CCR_MULTI_0))
/**
  * @}
  */ 

/** @defgroup ADCEx_Direct_memory_access_mode_for_multi_mode ADC Direct Memory Access Mode For Multi Mode
  * @{
  */ 
#define ADC_DMAACCESSMODE_DISABLED  ((uint32_t)0x00000000)     /*!< DMA mode disabled */
#define ADC_DMAACCESSMODE_1         ((uint32_t)ADC_CCR_DMA_0)  /*!< DMA mode 1 enabled (2 / 3 half-words one by one - 1 then 2 then 3)*/
#define ADC_DMAACCESSMODE_2         ((uint32_t)ADC_CCR_DMA_1)  /*!< DMA mode 2 enabled (2 / 3 half-words by pairs - 2&1 then 1&3 then 3&2)*/
#define ADC_DMAACCESSMODE_3         ((uint32_t)ADC_CCR_DMA)    /*!< DMA mode 3 enabled (2 / 3 bytes by pairs - 2&1 then 1&3 then 3&2) */
/**
  * @}
  */ 

/** @defgroup ADCEx_External_trigger_edge_Injected ADC External Trigger Edge Injected
  * @{
  */ 
#define ADC_EXTERNALTRIGINJECCONVEDGE_NONE           ((uint32_t)0x00000000)
#define ADC_EXTERNALTRIGINJECCONVEDGE_RISING         ((uint32_t)ADC_CR2_JEXTEN_0)
#define ADC_EXTERNALTRIGINJECCONVEDGE_FALLING        ((uint32_t)ADC_CR2_JEXTEN_1)
#define ADC_EXTERNALTRIGINJECCONVEDGE_RISINGFALLING  ((uint32_t)ADC_CR2_JEXTEN)
/**
  * @}
  */ 

/** @defgroup ADCEx_External_trigger_Source_Injected ADC External Trigger Source Injected
  * @{
  */ 
#define ADC_EXTERNALTRIGINJECCONV_T1_CC4           ((uint32_t)0x00000000)
#define ADC_EXTERNALTRIGINJECCONV_T1_TRGO          ((uint32_t)ADC_CR2_JEXTSEL_0)
#define ADC_EXTERNALTRIGINJECCONV_T2_CC1           ((uint32_t)ADC_CR2_JEXTSEL_1)
#define ADC_EXTERNALTRIGINJECCONV_T2_TRGO          ((uint32_t)(ADC_CR2_JEXTSEL_1 | ADC_CR2_JEXTSEL_0))
#define ADC_EXTERNALTRIGINJECCONV_T3_CC2           ((uint32_t)ADC_CR2_JEXTSEL_2)
#define ADC_EXTERNALTRIGINJECCONV_T3_CC4           ((uint32_t)(ADC_CR2_JEXTSEL_2 | ADC_CR2_JEXTSEL_0))
#define ADC_EXTERNALTRIGINJECCONV_T4_CC1           ((uint32_t)(ADC_CR2_JEXTSEL_2 | ADC_CR2_JEXTSEL_1))
#define ADC_EXTERNALTRIGINJECCONV_T4_CC2           ((uint32_t)(ADC_CR2_JEXTSEL_2 | ADC_CR2_JEXTSEL_1 | ADC_CR2_JEXTSEL_0))
#define ADC_EXTERNALTRIGINJECCONV_T4_CC3           ((uint32_t)ADC_CR2_JEXTSEL_3)
#define ADC_EXTERNALTRIGINJECCONV_T4_TRGO          ((uint32_t)(ADC_CR2_JEXTSEL_3 | ADC_CR2_JEXTSEL_0))
#define ADC_EXTERNALTRIGINJECCONV_T5_CC4           ((uint32_t)(ADC_CR2_JEXTSEL_3 | ADC_CR2_JEXTSEL_1))
#define ADC_EXTERNALTRIGINJECCONV_T5_TRGO          ((uint32_t)(ADC_CR2_JEXTSEL_3 | ADC_CR2_JEXTSEL_1 | ADC_CR2_JEXTSEL_0))
#define ADC_EXTERNALTRIGINJECCONV_T8_CC2           ((uint32_t)(ADC_CR2_JEXTSEL_3 | ADC_CR2_JEXTSEL_2))
#define ADC_EXTERNALTRIGINJECCONV_T8_CC3           ((uint32_t)(ADC_CR2_JEXTSEL_3 | ADC_CR2_JEXTSEL_2 | ADC_CR2_JEXTSEL_0))
#define ADC_EXTERNALTRIGINJECCONV_T8_CC4           ((uint32_t)(ADC_CR2_JEXTSEL_3 | ADC_CR2_JEXTSEL_2 | ADC_CR2_JEXTSEL_1))
#define ADC_EXTERNALTRIGINJECCONV_EXT_IT15         ((uint32_t)ADC_CR2_JEXTSEL)
#define ADC_INJECTED_SOFTWARE_START                ((uint32_t)ADC_CR2_JEXTSEL + 1)
/**
  * @}
  */ 

/** @defgroup ADCEx_injected_channel_selection ADC Injected Channel Selection
  * @{
  */ 
#define ADC_INJECTED_RANK_1    ((uint32_t)0x00000001)
#define ADC_INJECTED_RANK_2    ((uint32_t)0x00000002)
#define ADC_INJECTED_RANK_3    ((uint32_t)0x00000003)
#define ADC_INJECTED_RANK_4    ((uint32_t)0x00000004)
/**
  * @}
  */

/** @defgroup ADCEx_channels  ADC Specific Channels
  * @{
  */
#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx) || \
    defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F410Tx) || defined(STM32F410Cx) || \
    defined(STM32F410Rx) || defined(STM32F412xG)  
#define ADC_CHANNEL_TEMPSENSOR  ((uint32_t)ADC_CHANNEL_16)
#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx || STM32F401xC || STM32F401xE || STM32F410xx || STM32F412xG */

#if defined(STM32F411xE) || defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || \
    defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx) 
#define ADC_CHANNEL_DIFFERENTIATION_TEMPSENSOR_VBAT ((uint32_t)0x10000000) /* Dummy bit for driver internal usage, not used in ADC channel setting registers CR1 or SQRx */
#define ADC_CHANNEL_TEMPSENSOR  ((uint32_t)ADC_CHANNEL_18 | ADC_CHANNEL_DIFFERENTIATION_TEMPSENSOR_VBAT)
#endif /* STM32F411xE || STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F446xx || STM32F469xx || STM32F479xx */      
/**
  * @}
  */ 


/**
  * @}
  */ 

/* Exported macro ------------------------------------------------------------*/
/** @defgroup ADC_Exported_Macros ADC Exported Macros
  * @{
  */

/**
  * @}
  */ 

/* Exported functions --------------------------------------------------------*/
/** @addtogroup ADCEx_Exported_Functions
  * @{
  */

/** @addtogroup ADCEx_Exported_Functions_Group1
  * @{
  */

/* I/O operation functions ******************************************************/
HAL_StatusTypeDef HAL_ADCEx_InjectedStart(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADCEx_InjectedStop(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADCEx_InjectedPollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout);
HAL_StatusTypeDef HAL_ADCEx_InjectedStart_IT(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADCEx_InjectedStop_IT(ADC_HandleTypeDef* hadc);
uint32_t HAL_ADCEx_InjectedGetValue(ADC_HandleTypeDef* hadc, uint32_t InjectedRank);
HAL_StatusTypeDef HAL_ADCEx_MultiModeStart_DMA(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length);
HAL_StatusTypeDef HAL_ADCEx_MultiModeStop_DMA(ADC_HandleTypeDef* hadc);
uint32_t HAL_ADCEx_MultiModeGetValue(ADC_HandleTypeDef* hadc);
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc);

/* Peripheral Control functions *************************************************/
HAL_StatusTypeDef HAL_ADCEx_InjectedConfigChannel(ADC_HandleTypeDef* hadc,ADC_InjectionConfTypeDef* sConfigInjected);
HAL_StatusTypeDef HAL_ADCEx_MultiModeConfigChannel(ADC_HandleTypeDef* hadc, ADC_MultiModeTypeDef* multimode);

/**
  * @}
  */ 

/**
  * @}
  */
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup ADCEx_Private_Constants ADC Private Constants
  * @{
  */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup ADCEx_Private_Macros ADC Private Macros
  * @{
  */
#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx) || \
    defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F410Tx) || defined(STM32F410Cx) || \
    defined(STM32F410Rx) || defined(STM32F411xE) || defined(STM32F412xG)
#define IS_ADC_CHANNEL(CHANNEL) ((CHANNEL) <= ADC_CHANNEL_18)
#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx || STM32F401xC || STM32F401xE || STM32F410xx || STM32F411xE || STM32F412xG */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || \
    defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)
#define IS_ADC_CHANNEL(CHANNEL) (((CHANNEL) <= ADC_CHANNEL_18)  || \
                                 ((CHANNEL) == ADC_CHANNEL_TEMPSENSOR))
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F446xx || STM32F469xx || STM32F479xx */      

#define IS_ADC_MODE(MODE) (((MODE) == ADC_MODE_INDEPENDENT)                 || \
                           ((MODE) == ADC_DUALMODE_REGSIMULT_INJECSIMULT)   || \
                           ((MODE) == ADC_DUALMODE_REGSIMULT_ALTERTRIG)     || \
                           ((MODE) == ADC_DUALMODE_INJECSIMULT)             || \
                           ((MODE) == ADC_DUALMODE_REGSIMULT)               || \
                           ((MODE) == ADC_DUALMODE_INTERL)                  || \
                           ((MODE) == ADC_DUALMODE_ALTERTRIG)               || \
                           ((MODE) == ADC_TRIPLEMODE_REGSIMULT_INJECSIMULT) || \
                           ((MODE) == ADC_TRIPLEMODE_REGSIMULT_AlterTrig)   || \
                           ((MODE) == ADC_TRIPLEMODE_INJECSIMULT)           || \
                           ((MODE) == ADC_TRIPLEMODE_REGSIMULT)             || \
                           ((MODE) == ADC_TRIPLEMODE_INTERL)                || \
                           ((MODE) == ADC_TRIPLEMODE_ALTERTRIG))
#define IS_ADC_DMA_ACCESS_MODE(MODE) (((MODE) == ADC_DMAACCESSMODE_DISABLED) || \
                                      ((MODE) == ADC_DMAACCESSMODE_1)        || \
                                      ((MODE) == ADC_DMAACCESSMODE_2)        || \
                                      ((MODE) == ADC_DMAACCESSMODE_3))
#define IS_ADC_EXT_INJEC_TRIG_EDGE(EDGE) (((EDGE) == ADC_EXTERNALTRIGINJECCONVEDGE_NONE)    || \
                                          ((EDGE) == ADC_EXTERNALTRIGINJECCONVEDGE_RISING)  || \
                                          ((EDGE) == ADC_EXTERNALTRIGINJECCONVEDGE_FALLING) || \
                                          ((EDGE) == ADC_EXTERNALTRIGINJECCONVEDGE_RISINGFALLING))
#define IS_ADC_EXT_INJEC_TRIG(INJTRIG) (((INJTRIG) == ADC_EXTERNALTRIGINJECCONV_T1_CC4)  || \
                                        ((INJTRIG) == ADC_EXTERNALTRIGINJECCONV_T1_TRGO) || \
                                        ((INJTRIG) == ADC_EXTERNALTRIGINJECCONV_T2_CC1)  || \
                                        ((INJTRIG) == ADC_EXTERNALTRIGINJECCONV_T2_TRGO) || \
                                        ((INJTRIG) == ADC_EXTERNALTRIGINJECCONV_T3_CC2)  || \
                                        ((INJTRIG) == ADC_EXTERNALTRIGINJECCONV_T3_CC4)  || \
                                        ((INJTRIG) == ADC_EXTERNALTRIGINJECCONV_T4_CC1)  || \
                                        ((INJTRIG) == ADC_EXTERNALTRIGINJECCONV_T4_CC2)  || \
                                        ((INJTRIG) == ADC_EXTERNALTRIGINJECCONV_T4_CC3)  || \
                                        ((INJTRIG) == ADC_EXTERNALTRIGINJECCONV_T4_TRGO) || \
                                        ((INJTRIG) == ADC_EXTERNALTRIGINJECCONV_T5_CC4)  || \
                                        ((INJTRIG) == ADC_EXTERNALTRIGINJECCONV_T5_TRGO) || \
                                        ((INJTRIG) == ADC_EXTERNALTRIGINJECCONV_T8_CC2)  || \
                                        ((INJTRIG) == ADC_EXTERNALTRIGINJECCONV_T8_CC3)  || \
                                        ((INJTRIG) == ADC_EXTERNALTRIGINJECCONV_T8_CC4)  || \
                                        ((INJTRIG) == ADC_EXTERNALTRIGINJECCONV_EXT_IT15)|| \
                                        ((INJTRIG) == ADC_INJECTED_SOFTWARE_START))
#define IS_ADC_INJECTED_LENGTH(LENGTH) (((LENGTH) >= ((uint32_t)1)) && ((LENGTH) <= ((uint32_t)4)))
#define IS_ADC_INJECTED_RANK(RANK) (((RANK) >= ((uint32_t)1)) && ((RANK) <= ((uint32_t)4)))

/**
  * @brief  Set the selected injected Channel rank.
  * @param  _CHANNELNB_: Channel number.
  * @param  _RANKNB_: Rank number. 
  * @param  _JSQR_JL_: Sequence length.     
  * @retval None
  */
#define   ADC_JSQR(_CHANNELNB_, _RANKNB_, _JSQR_JL_)  (((uint32_t)((uint16_t)(_CHANNELNB_))) << (5 * (uint8_t)(((_RANKNB_) + 3) - (_JSQR_JL_))))

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup ADCEx_Private_Functions ADC Private Functions
  * @{
  */

/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__STM32F4xx_ADC_EX_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
