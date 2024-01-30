#ifndef HAL_I2C_STM_HPP
#define HAL_I2C_STM_HPP

#include DEVICE_HEADER
#include "hal/interfaces/I2c.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/util/AutoResetFunction.hpp"

namespace hal
{
    class I2cStm
        : public hal::I2cMaster
    {
    public:
        struct Config
        {
            constexpr Config()
            {}

#if defined(STM32F0) || defined(STM32F7) || defined(STM32WB) || defined(STM32G4) || defined(STM32G0)
            uint32_t timing = 0x00304d4d;
#endif
#if defined(STM32F2) || defined(STM32F4)
            uint32_t clockSpeed = 400000;
#endif
        };

        I2cStm(uint8_t oneBasedI2cIndex, hal::GpioPinStm& scl, hal::GpioPinStm& sda, const Config& config = Config());
        ~I2cStm();

        void SendData(I2cAddress address, infra::ConstByteRange data, Action nextAction, infra::Function<void(Result, uint32_t numberOfBytesSent)> onSent) override;
        void ReceiveData(I2cAddress address, infra::ByteRange data, Action nextAction, infra::Function<void(Result)> onReceived) override;

    protected:
        virtual void DeviceNotFound();
        virtual void BusError();
        virtual void ArbitrationLost();

    private:
        void EventInterrupt();
        void ErrorInterrupt();
#if defined(STM32F0) || defined(STM32F7) || defined(STM32WB) || defined(STM32G4) || defined(STM32G0)
        void ReadReceivedData();
#endif

    private:
        uint8_t instance;
        hal::PeripheralPinStm scl;
        hal::PeripheralPinStm sda;

        ImmediateInterruptHandler evInterruptHandler;
        DispatchedInterruptHandler erInterruptHandler;

        I2C_HandleTypeDef i2cHandle;

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
