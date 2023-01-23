#ifndef HAL_CAN_STM_HPP
#define HAL_CAN_STM_HPP

#include "cmsis_device.h"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal/interfaces/Can.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"

#if defined(HAS_PERIPHERAL_CAN)

namespace hal
{
    class CanStm
        : Can
    {
    public:
        struct Config
        {
            constexpr Config()
            {}

            uint32_t prescaler = 1024;
        };

        CanStm(uint8_t oneBasedCanIndex, GpioPinStm& tx, GpioPinStm& rx, const Config& config = Config());
        ~CanStm();

    public:
        virtual void SendData(Id id, const Message& data, const infra::Function<void(bool success)>& actionOnCompletion = infra::emptyFunction) override;
        virtual void ReceiveData(const infra::Function<void(Id id, const Message& data)>& receivedAction) override;

    private:
        void HandleTxInterrupt();
        void HandleRxInterrupt();
        void HandleErrorInterrupt();

    private:
        CAN_HandleTypeDef peripheralHandle = {};
        uint8_t canIndex;
        PeripheralPinStm tx;
        PeripheralPinStm rx;

        infra::Function<void(bool success)> onMessageSent;
        infra::Function<void(Id id, const Message& data)> onMessageReceived;

        DispatchedInterruptHandler canTxInterruptDispatcher;
        DispatchedInterruptHandler canRxInterruptDispatcher;
        DispatchedInterruptHandler canErrorInterruptDispatcher;
    };
}

#endif

#endif
