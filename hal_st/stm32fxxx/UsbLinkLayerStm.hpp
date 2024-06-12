#ifndef HAL_USB_LINK_LAYER_STM_HPP
#define HAL_USB_LINK_LAYER_STM_HPP

#include DEVICE_HEADER
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal/interfaces/UsbLinkLayer.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/util/InterfaceConnector.hpp"

#if defined(USB_OTG_FS)

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

        void OpenEndPoint(uint8_t ep_addr, UsbEndPointType type, uint16_t ep_mps) override;
        void CloseEndPoint(uint8_t ep_addr) override;
        void FlushEndPoint(uint8_t ep_addr) override;
        void StallEndPoint(uint8_t ep_addr) override;
        void ClearStallEndPoint(uint8_t ep_addr) override;
        bool IsStallEndPoint(uint8_t ep_addr) override;
        void SetUsbAddress(uint8_t dev_addr) override;
        void Transmit(uint8_t ep_addr, infra::ConstByteRange data) override;
        void PrepareReceive(uint8_t ep_addr, infra::ConstByteRange data) override;
        uint32_t GetReceiveDataSize(uint8_t ep_addr) override;

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

#endif
