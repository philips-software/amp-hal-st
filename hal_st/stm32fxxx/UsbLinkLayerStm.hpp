#ifndef HAL_USB_LINK_LAYER_STM_HPP
#define HAL_USB_LINK_LAYER_STM_HPP

#include DEVICE_HEADER
#include "hal/interfaces/UsbLinkLayer.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/util/InterfaceConnector.hpp"

namespace hal
{
    class UsbDevice;

    class UsbLinkLayerStm
        : public UsbLinkLayer
        , public infra::InterfaceConnector<UsbLinkLayerStm>
    {
    public:
        UsbLinkLayerStm(UsbDeviceFactory& deviceFactory, hal::GpioPin& powerSwitch, hal::GpioPin& vbus, hal::GpioPin& overCurrent, hal::GpioPinStm& id, hal::GpioPinStm& dm, hal::GpioPinStm& dp);
        ~UsbLinkLayerStm();

        virtual void OpenEndPoint(uint8_t ep_addr, UsbEndPointType type, uint16_t ep_mps) override;
        virtual void CloseEndPoint(uint8_t ep_addr) override;
        virtual void FlushEndPoint(uint8_t ep_addr) override;
        virtual void StallEndPoint(uint8_t ep_addr) override;
        virtual void ClearStallEndPoint(uint8_t ep_addr) override;
        virtual bool IsStallEndPoint(uint8_t ep_addr) override;
        virtual void SetUsbAddress(uint8_t dev_addr) override;
        virtual void Transmit(uint8_t ep_addr, infra::ConstByteRange data) override;
        virtual void PrepareReceive(uint8_t ep_addr, infra::ConstByteRange data) override;
        virtual uint32_t GetReceiveDataSize(uint8_t ep_addr) override;

        void SetupStageCallback();
        void DataOutStageCallback(uint8_t epnum);
        void DataInStageCallback(uint8_t epnum);
        void StartOfFrameCallback();
        void ResetCallback();
        void SuspendCallback();
        void ResumeCallback();

        void IrqHandler();

    private:
        UsbDeviceFactory& deviceFactory;

        hal::OutputPin powerSwitch;
        hal::InputPin vbus;
        hal::InputPin overCurrent;
        hal::PeripheralPinStm id;
        hal::PeripheralPinStm dm;
        hal::PeripheralPinStm dp;

        PCD_HandleTypeDef pcd;
    };
}

#endif
