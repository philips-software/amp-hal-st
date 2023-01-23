#include "hal_st/stm32fxxx/UsbLinkLayerStm.hpp"
#include <cstdlib>

#ifndef STM32F3

extern "C" void OTG_FS_IRQHandler(void)
{
    static_cast<hal::UsbLinkLayerStm&>(hal::UsbLinkLayerStm::Instance()).IrqHandler();
}

void HAL_PCD_MspInit(PCD_HandleTypeDef* hpcd)
{
    __USB_OTG_FS_CLK_ENABLE();

    HAL_NVIC_SetPriority(OTG_FS_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef* hpcd)
{
    __USB_OTG_FS_CLK_DISABLE();

    HAL_NVIC_DisableIRQ(OTG_FS_IRQn);
}

void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef* hpcd)
{
    static_cast<hal::UsbLinkLayerStm*>(hpcd->pData)->SetupStageCallback();
}

void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum)
{
    static_cast<hal::UsbLinkLayerStm*>(hpcd->pData)->DataOutStageCallback(epnum);
}

void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum)
{
    static_cast<hal::UsbLinkLayerStm*>(hpcd->pData)->DataInStageCallback(epnum);
}

void HAL_PCD_SOFCallback(PCD_HandleTypeDef* hpcd)
{
    static_cast<hal::UsbLinkLayerStm*>(hpcd->pData)->StartOfFrameCallback();
}

void HAL_PCD_ResetCallback(PCD_HandleTypeDef* hpcd)
{
    static_cast<hal::UsbLinkLayerStm*>(hpcd->pData)->ResetCallback();
}

void HAL_PCD_SuspendCallback(PCD_HandleTypeDef* hpcd)
{
    static_cast<hal::UsbLinkLayerStm*>(hpcd->pData)->SuspendCallback();
}

void HAL_PCD_ResumeCallback(PCD_HandleTypeDef* hpcd)
{
    static_cast<hal::UsbLinkLayerStm*>(hpcd->pData)->ResumeCallback();
}

void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum)
{
    static_cast<hal::UsbLinkLayerStm*>(hpcd->pData)->GetObserver().IsochronousOutIncomplete(epnum);
}

void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum)
{
    static_cast<hal::UsbLinkLayerStm*>(hpcd->pData)->GetObserver().IsochronousInIncomplete(epnum);
}

namespace hal
{
    UsbLinkLayerStm::UsbLinkLayerStm(UsbDeviceFactory& deviceFactory, hal::GpioPin& powerSwitch, hal::GpioPin& vbus, hal::GpioPin& overCurrent, hal::GpioPinStm& id, hal::GpioPinStm& dm, hal::GpioPinStm& dp)
        : deviceFactory(deviceFactory)
        , powerSwitch(powerSwitch)
        , vbus(vbus)
        , overCurrent(overCurrent)
        , id(id, hal::PinConfigTypeStm::usbFsId, 0)
        , dm(dm, hal::PinConfigTypeStm::usbFsDm, 0)
        , dp(dp, hal::PinConfigTypeStm::usbFsDp, 0)
    {
        pcd.pData = this;
        pcd.Instance = USB_OTG_FS;
        pcd.Init.dev_endpoints = 7;
        pcd.Init.speed = PCD_SPEED_FULL;
        pcd.Init.dma_enable = DISABLE;
        pcd.Init.ep0_mps = DEP0CTL_MPS_64;
        pcd.Init.phy_itface = PCD_PHY_EMBEDDED;
        pcd.Init.Sof_enable = DISABLE;
        pcd.Init.low_power_enable = DISABLE;
        pcd.Init.lpm_enable = DISABLE;
        pcd.Init.vbus_sensing_enable = DISABLE;
        pcd.Init.use_dedicated_ep1 = DISABLE;
        HAL_PCD_Init(&pcd);

        HAL_PCDEx_SetRxFiFo(&pcd, 0x80);
        HAL_PCDEx_SetTxFiFo(&pcd, 0, 0x40);
        HAL_PCDEx_SetTxFiFo(&pcd, 1, 0x80);

        HAL_PCD_Start(&pcd);
    }

    UsbLinkLayerStm::~UsbLinkLayerStm()
    {
        deviceFactory.Destroy();

        HAL_PCD_Stop(&pcd);
        HAL_PCD_DeInit(&pcd);
    }

    void UsbLinkLayerStm::OpenEndPoint(uint8_t ep_addr, UsbEndPointType type, uint16_t ep_mps)
    {
        HAL_PCD_EP_Open(&pcd, ep_addr, ep_mps, static_cast<uint8_t>(type));
    }

    void UsbLinkLayerStm::CloseEndPoint(uint8_t ep_addr)
    {
        HAL_PCD_EP_Close(&pcd, ep_addr);
    }

    void UsbLinkLayerStm::FlushEndPoint(uint8_t ep_addr)
    {
        HAL_PCD_EP_Flush(&pcd, ep_addr);
    }

    void UsbLinkLayerStm::StallEndPoint(uint8_t ep_addr)
    {
        HAL_PCD_EP_SetStall(&pcd, ep_addr);
    }

    void UsbLinkLayerStm::ClearStallEndPoint(uint8_t ep_addr)
    {
        HAL_PCD_EP_ClrStall(&pcd, ep_addr);
    }

    bool UsbLinkLayerStm::IsStallEndPoint(uint8_t ep_addr)
    {
        PCD_HandleTypeDef* hpcd = &pcd;

        if ((ep_addr & 0x80) == 0x80)
            return hpcd->IN_ep[ep_addr & 0x7F].is_stall;
        else
            return hpcd->OUT_ep[ep_addr & 0x7F].is_stall;
    }

    void UsbLinkLayerStm::SetUsbAddress(uint8_t dev_addr)
    {
        HAL_PCD_SetAddress(&pcd, dev_addr);
    }

    void UsbLinkLayerStm::Transmit(uint8_t ep_addr, infra::ConstByteRange data)
    {
        HAL_PCD_EP_Transmit(&pcd, ep_addr, const_cast<uint8_t*>(data.begin()), data.size());
    }

    void UsbLinkLayerStm::PrepareReceive(uint8_t ep_addr, infra::ConstByteRange data)
    {
        HAL_PCD_EP_Receive(&pcd, ep_addr, const_cast<uint8_t*>(data.begin()), data.size());
    }

    uint32_t UsbLinkLayerStm::GetReceiveDataSize(uint8_t ep_addr)
    {
        return HAL_PCD_EP_GetRxCount(&pcd, ep_addr);
    }

    void UsbLinkLayerStm::SetupStageCallback()
    {
        GetObserver().SetupStage(infra::ConstByteRange((uint8_t*)pcd.Setup, (uint8_t*)pcd.Setup + sizeof(pcd.Setup)));
    }

    void UsbLinkLayerStm::DataOutStageCallback(uint8_t epnum)
    {
        GetObserver().DataOutStage(epnum, infra::ByteRange(pcd.OUT_ep[epnum].xfer_buff, pcd.OUT_ep[epnum].xfer_buff + pcd.OUT_ep[epnum].xfer_count));
    }

    void UsbLinkLayerStm::DataInStageCallback(uint8_t epnum)
    {
        GetObserver().DataInStage(epnum, infra::ByteRange(pcd.IN_ep[epnum].xfer_buff, pcd.IN_ep[epnum].xfer_buff + pcd.IN_ep[epnum].xfer_count));
    }

    void UsbLinkLayerStm::StartOfFrameCallback()
    {
        GetObserver().StartOfFrame();
    }

    void UsbLinkLayerStm::ResetCallback()
    {
        UsbSpeed speed = UsbSpeed::full;

        // Specification of the type of hpcd->Init.speed: Either PCD_Core_Speed or USB_Core_Speed_ with either values High 0 Full 2 or High 0 High in full 1 Low 2 Full 3. Great.
        switch (pcd.Init.speed)
        {
            case PCD_SPEED_HIGH:
                speed = UsbSpeed::high;
                break;
            case PCD_SPEED_FULL:
                speed = UsbSpeed::full;
                break;
            default:
                speed = UsbSpeed::full;
                break;
        }

        deviceFactory.Create(*this);
    }

    void UsbLinkLayerStm::SuspendCallback()
    {
        if (HasObserver())
            GetObserver().Suspend();
        __HAL_PCD_GATE_PHYCLOCK(&pcd);
    }

    void UsbLinkLayerStm::ResumeCallback()
    {
        __HAL_PCD_UNGATE_PHYCLOCK(&pcd);
        if (HasObserver())
            GetObserver().Resume();
    }

    void UsbLinkLayerStm::IrqHandler()
    {
        HAL_PCD_IRQHandler(&pcd);
    }
}

#endif
