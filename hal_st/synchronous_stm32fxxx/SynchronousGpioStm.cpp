#include "hal_st/synchronous_stm32fxxx/SynchronousGpioStm.hpp"

namespace hal
{
    namespace
    {
        GPIO_TypeDef* const portToGPIOPort[] = {
            GPIOA,
            GPIOB,
            GPIOC,
            GPIOD,
            GPIOE,
            GPIOF,
    #if defined(STM32F2) || defined(STM32F4) || defined(STM32F7)
            GPIOG,
            GPIOH,
            GPIOI,
    #endif
    #if defined(STM32F7)
            GPIOJ,
            GPIOK
    #endif
        };

        const uint16_t pinToGPIOPin[16] = {
            GPIO_PIN_0,
            GPIO_PIN_1,
            GPIO_PIN_2,
            GPIO_PIN_3,
            GPIO_PIN_4,
            GPIO_PIN_5,
            GPIO_PIN_6,
            GPIO_PIN_7,
            GPIO_PIN_8,
            GPIO_PIN_9,
            GPIO_PIN_10,
            GPIO_PIN_11,
            GPIO_PIN_12,
            GPIO_PIN_13,
            GPIO_PIN_14,
            GPIO_PIN_15,
        };

        const uint32_t weakPullToPuPd[3] = {
            GPIO_NOPULL,
            GPIO_PULLUP,
            GPIO_PULLDOWN
        };

        const uint32_t speedToSpeed[] = {
            GPIO_SPEED_LOW,
            GPIO_SPEED_MEDIUM,
#if defined(STM32F2) || defined(STM32F4) || defined(STM32F7)
            GPIO_SPEED_FAST,
#endif
            GPIO_SPEED_HIGH
        };

        const uint32_t driveToOutputMode[2] = {
            GPIO_MODE_OUTPUT_PP,
            GPIO_MODE_OUTPUT_OD
        };

        const uint32_t driveToAFMode[2] = {
            GPIO_MODE_AF_PP,
            GPIO_MODE_AF_OD
        };
    }

    SynchronousOutputPinStm::SynchronousOutputPinStm(Port port, uint8_t index, Drive drive, Speed speed)
        : port(port)
        , index(index)
    {
        GPIO_InitTypeDef GPIO_InitStructure = {};
        GPIO_InitStructure.Pin = pinToGPIOPin[index];
        GPIO_InitStructure.Mode = driveToOutputMode[static_cast<uint8_t>(drive)];
        GPIO_InitStructure.Speed = speedToSpeed[static_cast<uint8_t>(speed)];
        HAL_GPIO_Init(portToGPIOPort[static_cast<uint8_t>(port)], &GPIO_InitStructure);
    }

    SynchronousOutputPinStm::~SynchronousOutputPinStm()
    {
        GPIO_InitTypeDef GPIO_InitStructure = {};
        GPIO_InitStructure.Pin = pinToGPIOPin[index];
        GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
        GPIO_InitStructure.Pull = weakPullToPuPd[static_cast<uint8_t>(WeakPull::Default)];
        HAL_GPIO_Init(portToGPIOPort[static_cast<uint8_t>(port)], &GPIO_InitStructure);
    }

    void SynchronousOutputPinStm::Set(bool value)
    {
        HAL_GPIO_WritePin(portToGPIOPort[static_cast<uint8_t>(port)], pinToGPIOPin[index], value ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }

    bool SynchronousOutputPinStm::GetOutputLatch() const
    {
        return (portToGPIOPort[static_cast<uint8_t>(port)]->ODR & pinToGPIOPin[index]) != GPIO_PIN_RESET;
    }

    SmallPeripheralPinStm::SmallPeripheralPinStm(const Definition<1>& definition)
        : port(definition.positions[0].port)
        , index(definition.positions[0].pin)
    {
        ConfigPeripheralPin(definition.positions[0].alternateFunction, definition.drive, definition.speed, definition.weakPull);
    }

    void SmallPeripheralPinStm::ConfigPeripheralPin(uint8_t af, Drive drive, Speed speed, WeakPull weakPull)
    {
        GPIO_InitTypeDef GPIO_InitStructure = {};
        GPIO_InitStructure.Pin = pinToGPIOPin[index];
        GPIO_InitStructure.Mode = driveToAFMode[static_cast<int>(drive)];
        GPIO_InitStructure.Pull = weakPullToPuPd[static_cast<int>(weakPull)];
        GPIO_InitStructure.Speed = speedToSpeed[static_cast<int>(speed)];
        GPIO_InitStructure.Alternate = af;
        HAL_GPIO_Init(portToGPIOPort[static_cast<int>(port)], &GPIO_InitStructure);
    }

    SmallPeripheralPinStm::~SmallPeripheralPinStm()
    {
        GPIO_InitTypeDef GPIO_InitStructure = {};
        GPIO_InitStructure.Pin = pinToGPIOPin[index];
        GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
        GPIO_InitStructure.Pull = weakPullToPuPd[static_cast<uint8_t>(WeakPull::Default)];
        HAL_GPIO_Init(portToGPIOPort[static_cast<uint8_t>(port)], &GPIO_InitStructure);
    }
}
