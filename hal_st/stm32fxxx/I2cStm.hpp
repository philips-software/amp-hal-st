#ifndef HAL_I2C_STM_HPP
#define HAL_I2C_STM_HPP

#include DEVICE_HEADER
#include "hal/interfaces/I2c.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/util/AutoResetFunction.hpp"

namespace hal
{
    namespace detail
    {
        struct I2cConfig
        {
#if defined(I2C_TIMINGR_PRESC)
            uint32_t timing = 0x00304d4d;
#endif
#if defined(I2C_CCR_CCR)
            uint32_t clockSpeed = 400000;
#endif
        };
    }

    class I2cStm
        : public hal::I2cMaster
    {
    public:
        using Config = detail::I2cConfig;

        I2cStm(uint8_t oneBasedI2cIndex, hal::GpioPinStm& scl, hal::GpioPinStm& sda, const Config& config = {});
        ~I2cStm();

        void SendData(I2cAddress address, infra::ConstByteRange data, Action nextAction, infra::Function<void(Result, uint32_t numberOfBytesSent)> onSent) override;
        void ReceiveData(I2cAddress address, infra::ByteRange data, Action nextAction, infra::Function<void(Result)> onReceived) override;
        void SetErrorPolicy(I2cErrorPolicy& policy) override;
        void ResetErrorPolicy() override;

    protected:
        virtual void DeviceNotFound();
        virtual void BusError();
        virtual void ArbitrationLost();

    private:
        void EventInterrupt();
        void ErrorInterrupt();
#if defined(I2C_ISR_TXE)
        void ReadReceivedData();
#endif
        void Clear();

    private:
        uint8_t instance;
        hal::PeripheralPinStm scl;
        hal::PeripheralPinStm sda;

        ImmediateInterruptHandler evInterruptHandler;
        DispatchedInterruptHandler erInterruptHandler;

        I2C_HandleTypeDef i2cHandle;

        I2cErrorPolicy* errorPolicy{ nullptr };

        Action nextAction;
#if defined(STM32F2) || defined(STM32F4)
        I2cAddress address{ 0 };
#endif
        infra::ConstByteRange sendData;
        infra::ByteRange receiveData;
        uint32_t sent = 0;
        uint32_t received = 0;
        infra::AutoResetFunction<void(Result, uint32_t numberOfBytesSent)> onSent;
        bool continuingPrevious = false;

        infra::AutoResetFunction<void(Result)> onReceived;
    };
}
#endif
