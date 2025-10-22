#include "hal_st/stm32fxxx/I2cStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/ReallyAssert.hpp"

namespace hal
{
    I2cStm::I2cStm(uint8_t oneBasedI2cIndex, hal::GpioPinStm& scl, hal::GpioPinStm& sda, const Config& config)
        : instance(oneBasedI2cIndex - 1)
        , scl(scl, PinConfigTypeStm::i2cScl, oneBasedI2cIndex)
        , sda(sda, PinConfigTypeStm::i2cSda, oneBasedI2cIndex)
        , evInterruptHandler(peripheralI2cEvIrq[instance], [this]()
              {
                  EventInterrupt();
              })
        , erInterruptHandler(peripheralI2cErIrq[instance], [this]()
              {
                  ErrorInterrupt();
              })
    {
        EnableClockI2c(instance);

        i2cHandle.Instance = peripheralI2c[instance];
#if defined(I2C_TIMINGR_PRESC)
        i2cHandle.Init.Timing = config.timing;
        i2cHandle.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
#endif
#if defined(STM32F2) || defined(STM32F4)
        i2cHandle.Init.ClockSpeed = config.clockSpeed;
        i2cHandle.Init.DutyCycle = I2C_DUTYCYCLE_2;
#endif
        i2cHandle.Init.OwnAddress1 = 0;
        i2cHandle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
        i2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
        i2cHandle.Init.OwnAddress2 = 0;
        i2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
        i2cHandle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
        HAL_I2C_Init(&i2cHandle);

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || \
    defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F411xE) || defined(STM32F446xx) || \
    defined(STM32F469xx) || defined(STM32F479xx) ||                                                 \
    defined(STM32F0) || defined(STM32F3) || defined(STM32F7) || defined(STM32H5)
        HAL_I2CEx_AnalogFilter_Config(&i2cHandle, I2C_ANALOGFILTER_ENABLE);
#endif
    }

    I2cStm::~I2cStm()
    {
        DisableClockI2c(instance);
    }

    void I2cStm::SendData(I2cAddress address, infra::ConstByteRange data, Action nextAction, infra::Function<void(Result, uint32_t numberOfBytesSent)> onSent)
    {
        really_assert(this->onSent == nullptr);
        really_assert(this->onReceived == nullptr);

        sendData = data;
        this->onSent = onSent;
        this->nextAction = nextAction;
#if defined(STM32F2) || defined(STM32F4)
        this->address = address;
#endif
        sent = 0;

        __DMB();

#if defined(I2C_CR1_TXIE)
        peripheralI2c[instance]->CR2 = ((address.address << 1) & I2C_CR2_SADD) | (continuingPrevious ? 0 : I2C_CR2_START) | (std::min<uint32_t>(sendData.size(), 255) << 16) | (sendData.size() > 255 || nextAction == Action::continueSession ? I2C_CR2_RELOAD : 0);
        peripheralI2c[instance]->CR1 |= I2C_CR1_TXIE | I2C_CR1_TCIE | I2C_CR1_NACKIE | I2C_CR1_ERRIE;
#else
        really_assert(!sendData.empty());
        if (!continuingPrevious)
            peripheralI2c[instance]->CR1 |= I2C_CR1_START;

        really_assert((peripheralI2c[instance]->CR2 & I2C_CR2_ITBUFEN) == 0);
        peripheralI2c[instance]->CR2 |= I2C_CR2_ITEVTEN | I2C_CR2_ITERREN | (continuingPrevious ? I2C_CR2_ITBUFEN : 0);
#endif
    }

    void I2cStm::ReceiveData(I2cAddress address, infra::ByteRange data, Action nextAction, infra::Function<void(Result)> onReceived)
    {
        really_assert(this->onSent == nullptr);
        really_assert(this->onReceived == nullptr);

        receiveData = data;
        this->onReceived = onReceived;
        this->nextAction = nextAction;
#if defined(STM32F2) || defined(STM32F4)
        this->address = address;
#endif
        received = 0;

        __DMB();

#if defined(I2C_ICR_STOPCF)
        peripheralI2c[instance]->ICR = I2C_ICR_STOPCF;
        peripheralI2c[instance]->CR2 = ((address.address << 1) & I2C_CR2_SADD) | I2C_CR2_RD_WRN | (continuingPrevious ? 0 : I2C_CR2_START) | (std::min<uint32_t>(receiveData.size(), 255) << 16) | (receiveData.size() > 255 || nextAction == Action::continueSession ? I2C_CR2_RELOAD : 0);
        peripheralI2c[instance]->CR1 |= I2C_CR1_RXIE | I2C_CR1_TCIE | I2C_CR1_NACKIE | I2C_CR1_ERRIE;
#else
        really_assert(!receiveData.empty());
        peripheralI2c[instance]->CR1 = (peripheralI2c[instance]->CR1 | I2C_CR1_ACK) & ~I2C_CR1_POS;

        if (!continuingPrevious)
            peripheralI2c[instance]->CR1 |= I2C_CR1_START;

        really_assert((peripheralI2c[instance]->CR2 & I2C_CR2_ITBUFEN) == 0);
        peripheralI2c[instance]->CR2 |= I2C_CR2_ITEVTEN | I2C_CR2_ITERREN;
#endif
    }

    void I2cStm::DeviceNotFound()
    {
        std::abort();
    }

    void I2cStm::BusError()
    {
        std::abort();
    }

    void I2cStm::ArbitrationLost()
    {
        std::abort();
    }

    void I2cStm::EventInterrupt()
    {
#if defined(I2C_ISR_TXE)
        if ((peripheralI2c[instance]->ISR & I2C_ISR_NACKF) != 0)
        {
            peripheralI2c[instance]->CR1 &= ~(I2C_CR1_TXIE | I2C_CR1_RXIE | I2C_CR1_TCIE | I2C_CR1_NACKIE | I2C_CR1_ERRIE);
            peripheralI2c[instance]->ICR = I2C_ICR_NACKCF;

            infra::EventDispatcher::Instance().Schedule([this]()
                {
                    if (sent == 0 && received == 0)
                        DeviceNotFound();
                    else if (!sendData.empty())
                    {
                        sendData.clear();
                        onSent(hal::Result::partialComplete, sent - 1);
                    }
                    else
                    {
                        receiveData.clear();
                        onReceived(hal::Result::partialComplete);
                    }
                });
        }

        if ((peripheralI2c[instance]->ISR & I2C_ISR_TXIS) != 0)
        {
            assert(!sendData.empty());
            peripheralI2c[instance]->TXDR = sendData.front();
            sendData.pop_front();
            ++sent;
        }

        ReadReceivedData();

        if ((peripheralI2c[instance]->ISR & I2C_ISR_TCR) != 0) // Transfer Complete Reload
        {
            if (sendData.empty() && receiveData.empty())
            {
                peripheralI2c[instance]->CR1 &= ~(I2C_CR1_TXIE | I2C_CR1_RXIE | I2C_CR1_TCIE | I2C_CR1_NACKIE | I2C_CR1_ERRIE);
                if (nextAction == Action::stop)
                    peripheralI2c[instance]->CR2 = I2C_CR2_STOP;
                continuingPrevious = nextAction == Action::continueSession;
                infra::EventDispatcher::Instance().Schedule([this]()
                    {
                        onSent(hal::Result::complete, sent);
                    });
            }
            else if (!receiveData.empty())
                peripheralI2c[instance]->CR2 = (std::min<uint32_t>(receiveData.size(), 255) << 16) | (receiveData.size() > 255 || nextAction == Action::continueSession ? I2C_CR2_RELOAD : 0);
            else
                peripheralI2c[instance]->CR2 = (std::min<uint32_t>(sendData.size(), 255) << 16) | (sendData.size() > 255 || nextAction == Action::continueSession ? I2C_CR2_RELOAD : 0);
        }

        if ((peripheralI2c[instance]->ISR & I2C_ISR_TC) != 0) // Transfer Complete
        {
            ReadReceivedData();
            really_assert(sendData.empty());
            really_assert(receiveData.empty());
            peripheralI2c[instance]->CR1 &= ~(I2C_CR1_TXIE | I2C_CR1_RXIE | I2C_CR1_TCIE | I2C_CR1_NACKIE | I2C_CR1_ERRIE);
            if (nextAction == Action::stop)
                peripheralI2c[instance]->CR2 = I2C_CR2_STOP;
            continuingPrevious = nextAction == Action::continueSession;
            if (onSent != nullptr)
                infra::EventDispatcher::Instance().Schedule([this]()
                    {
                        sent = 0;
                        onSent(hal::Result::complete, sent);
                    });
            else
                infra::EventDispatcher::Instance().Schedule([this]()
                    {
                        received = 0;
                        onReceived(hal::Result::complete);
                    });
        }
#else
        auto sr1 = peripheralI2c[instance]->SR1;

        if ((sr1 & I2C_SR1_SB) != 0)
        {
            peripheralI2c[instance]->DR = (address.address << 1) | (sendData.empty() ? 1 : 0);
        }
        else if ((sr1 & I2C_SR1_ADDR) != 0)
        {
            if (receiveData.size() == 1 || receiveData.size() == 2)
                peripheralI2c[instance]->CR1 &= ~I2C_CR1_ACK;
            if (receiveData.size() == 2)
                peripheralI2c[instance]->CR1 |= I2C_CR1_POS;

            (void)peripheralI2c[instance]->SR2; // Clear ADDR

            if (receiveData.size() == 1)
            {
                if (nextAction == Action::stop)
                    peripheralI2c[instance]->CR1 |= I2C_CR1_STOP;
                continuingPrevious = nextAction == Action::continueSession;
            }

            if (receiveData.size() != 2)
                peripheralI2c[instance]->CR2 |= I2C_CR2_ITBUFEN;
        }
        else if ((sr1 & I2C_SR1_TXE) != 0 && !sendData.empty() && (peripheralI2c[instance]->CR2 & I2C_CR2_ITBUFEN) != 0)
        {
            auto data = sendData.front();
            sendData.pop_front();
            ++sent;

            if (sendData.empty())
                peripheralI2c[instance]->CR2 &= ~I2C_CR2_ITBUFEN;

            peripheralI2c[instance]->DR = data;
        }
        else if ((sr1 & I2C_SR1_BTF) != 0 && receiveData.size() == 2)
        {
            peripheralI2c[instance]->CR2 &= ~(I2C_CR2_ITEVTEN | I2C_CR2_ITERREN);

            if (nextAction == Action::stop)
                peripheralI2c[instance]->CR1 |= I2C_CR1_STOP;
            continuingPrevious = nextAction == Action::continueSession;

            receiveData.front() = peripheralI2c[instance]->DR;
            receiveData.pop_front();
            ++received;

            receiveData.front() = peripheralI2c[instance]->DR;
            receiveData.pop_front();
            ++received;

            infra::EventDispatcher::Instance().Schedule([this]()
                {
                    onReceived(hal::Result::complete);
                });
        }
        else if ((sr1 & I2C_SR1_RXNE) != 0 && receiveData.size() == 1)
        {
            peripheralI2c[instance]->CR2 &= ~(I2C_CR2_ITEVTEN | I2C_CR2_ITERREN | I2C_CR2_ITBUFEN);

            receiveData.front() = peripheralI2c[instance]->DR;
            ;
            receiveData.pop_front();
            ++received;

            infra::EventDispatcher::Instance().Schedule([this]()
                {
                    onReceived(hal::Result::complete);
                });
        }
        else if ((sr1 & I2C_SR1_RXNE) != 0 && !receiveData.empty())
        {
            if (receiveData.size() == 2)
                peripheralI2c[instance]->CR1 &= ~I2C_CR1_ACK;

            receiveData.front() = peripheralI2c[instance]->DR;
            ;
            receiveData.pop_front();
            ++received;

            if (receiveData.size() == 1)
            {
                if (nextAction == Action::stop)
                    peripheralI2c[instance]->CR1 |= I2C_CR1_STOP;
                continuingPrevious = nextAction == Action::continueSession;
            }
        }
        else if ((sr1 & I2C_SR1_BTF) != 0 && sendData.empty() && onSent != nullptr)
        {
            peripheralI2c[instance]->CR2 &= ~(I2C_CR2_ITEVTEN | I2C_CR2_ITERREN);

            if (nextAction == Action::stop)
                peripheralI2c[instance]->CR1 |= I2C_CR1_STOP;
            else
                (void)peripheralI2c[instance]->DR; // Clear BTF
            continuingPrevious = nextAction == Action::continueSession;

            infra::EventDispatcher::Instance().Schedule([this]()
                {
                    onSent(hal::Result::complete, sent);
                });
        }
        else
            std::abort();
#endif
    }

    void I2cStm::ErrorInterrupt()
    {
#if defined(I2C_ISR_BERR)
        if ((peripheralI2c[instance]->ISR & I2C_ISR_BERR) != 0)
        {
            peripheralI2c[instance]->ICR |= I2C_ISR_BERR;
            BusError();
        }

        if ((peripheralI2c[instance]->ISR & I2C_ISR_ARLO) != 0)
        {
            peripheralI2c[instance]->ICR |= I2C_ISR_ARLO;
            ArbitrationLost();
        }

        really_assert(
            (peripheralI2c[instance]->ISR & I2C_ISR_OVR) == 0 && (peripheralI2c[instance]->ISR & I2C_ISR_PECERR) == 0 && (peripheralI2c[instance]->ISR & I2C_ISR_TIMEOUT) == 0 && (peripheralI2c[instance]->ISR & I2C_ISR_ALERT) == 0);
#else
        if ((peripheralI2c[instance]->SR1 & I2C_SR1_AF) != 0)
        {
            peripheralI2c[instance]->SR1 &= ~I2C_SR1_AF;
            peripheralI2c[instance]->CR2 &= ~(I2C_CR2_ITEVTEN | I2C_CR2_ITERREN | I2C_CR2_ITBUFEN);
            peripheralI2c[instance]->CR1 |= I2C_CR1_STOP;

            infra::EventDispatcher::Instance().Schedule([this]()
                {
                    if (sent == 0 && received == 0)
                        DeviceNotFound();
                    else if (!sendData.empty())
                    {
                        sendData.clear();
                        onSent(hal::Result::partialComplete, sent - 1);
                    }
                    else
                    {
                        receiveData.clear();
                        onReceived(hal::Result::partialComplete);
                    }
                });
        }
        else if ((peripheralI2c[instance]->SR1 & I2C_SR1_BERR) != 0)
        {
            peripheralI2c[instance]->SR1 &= ~I2C_SR1_BERR;
            peripheralI2c[instance]->CR2 &= ~(I2C_CR2_ITEVTEN | I2C_CR2_ITERREN | I2C_CR2_ITBUFEN);
            peripheralI2c[instance]->CR1 |= I2C_CR1_STOP;

            infra::EventDispatcher::Instance().Schedule([this]()
                {
                    BusError();
                });
        }
        else if ((peripheralI2c[instance]->SR1 & I2C_SR1_ARLO) != 0)
        {
            peripheralI2c[instance]->SR1 &= ~I2C_SR1_ARLO;
            peripheralI2c[instance]->CR2 &= ~(I2C_CR2_ITEVTEN | I2C_CR2_ITERREN | I2C_CR2_ITBUFEN);

            infra::EventDispatcher::Instance().Schedule([this]()
                {
                    ArbitrationLost();
                });
        }
        else
            std::abort();
#endif
    }

#if defined(I2C_ISR_TXE)
    void I2cStm::ReadReceivedData()
    {
        while ((peripheralI2c[instance]->ISR & I2C_ISR_RXNE) != 0)
        {
            assert(!receiveData.empty());
            receiveData.front() = peripheralI2c[instance]->RXDR;
            receiveData.pop_front();
            ++received;
        }
    }
#endif
}
