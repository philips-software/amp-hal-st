#include "hal_st/stm32fxxx/CanStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"

#if defined(HAS_PERIPHERAL_CAN)

namespace hal
{
    CanStm::CanStm(uint8_t oneBasedCanIndex, GpioPinStm& tx, GpioPinStm& rx, const Config& config)
        : canIndex(oneBasedCanIndex - 1)
        , tx(tx, hal::PinConfigTypeStm::canTx, oneBasedCanIndex)
        , rx(rx, hal::PinConfigTypeStm::canRx, oneBasedCanIndex)
        , canTxInterruptDispatcher(peripheralCanTxIrq[canIndex], [this]()
              { HandleTxInterrupt(); })
        , canRxInterruptDispatcher(peripheralCanRx0Irq[canIndex], [this]()
              { HandleRxInterrupt(); })
        , canErrorInterruptDispatcher(peripheralCanSceIrq[canIndex], [this]()
              { HandleErrorInterrupt(); })
    {
        EnableClockCan(canIndex);

        peripheralHandle.Instance = peripheralCan[canIndex];
        peripheralHandle.Init.Prescaler = config.prescaler;
        peripheralHandle.Init.Mode = CAN_MODE_NORMAL; // CAN_MODE_SILENT_LOOPBACK
        peripheralHandle.Init.SyncJumpWidth = CAN_SJW_4TQ;
        peripheralHandle.Init.TimeSeg1 = CAN_BS1_6TQ;
        peripheralHandle.Init.TimeSeg2 = CAN_BS2_8TQ;
        peripheralHandle.Init.TimeTriggeredMode = DISABLE;
        peripheralHandle.Init.AutoBusOff = ENABLE;
        peripheralHandle.Init.AutoWakeUp = ENABLE;
        peripheralHandle.Init.AutoRetransmission = DISABLE;
        peripheralHandle.Init.ReceiveFifoLocked = DISABLE;
        peripheralHandle.Init.TransmitFifoPriority = DISABLE;

        HAL_CAN_Init(&peripheralHandle);

        peripheralCan[canIndex]->IER |= CAN_IER_TMEIE;
        peripheralCan[canIndex]->IER |= CAN_IER_BOFIE | CAN_IER_ERRIE;

        // peripheralCan[canIndex]->FMR |= CAN_FMR_FINIT;          // Start filter initialization
        // peripheralCan[canIndex]->FS1R |= CAN_FS1R_FSC0;         // 32 bit address
        // peripheralCan[canIndex]->FM1R &= ~CAN_FM1R_FBM0;        // Mask
        // peripheralCan[canIndex]->sFilterRegister[0].FR1 = 0;    // No significant bits
        // peripheralCan[canIndex]->sFilterRegister[0].FR2 = 0;    // No significant bits
        // peripheralCan[canIndex]->FA1R |= CAN_FA1R_FACT0;        // Make filter active
        // peripheralCan[canIndex]->FMR &= ~CAN_FMR_FINIT;         // End filter initialization

        CAN_FilterTypeDef CAN_FilterStruct;
        CAN_FilterStruct.FilterIdHigh = 0x0000;                   /* Upper 16bit filter ID */
        CAN_FilterStruct.FilterIdLow = 0x0000;                    /* Filter lower 16bit ID */
        CAN_FilterStruct.FilterMaskIdHigh = 0x0000;               /* Upper 16bit filter mask */
        CAN_FilterStruct.FilterMaskIdLow = 0x0000;                /* Lower 16bit filter mask */
        CAN_FilterStruct.FilterFIFOAssignment = CAN_FILTER_FIFO0; /* Which FIFO will be assigned to filter */
        CAN_FilterStruct.FilterBank = 0;
        CAN_FilterStruct.FilterMode = CAN_FILTERMODE_IDMASK;        /* Identifier mask mode */
        CAN_FilterStruct.FilterScale = CAN_FILTERSCALE_32BIT;       /* 32bit ID filter */
        CAN_FilterStruct.FilterActivation = ENABLE;                 /* Enable this filter */
        CAN_FilterStruct.SlaveStartFilterBank = 14;                           /* Start slave bank filter (?) */
        HAL_CAN_ConfigFilter(&peripheralHandle, &CAN_FilterStruct); /* Initialize filter */
    }

    CanStm::~CanStm()
    {
        HAL_CAN_DeInit(&peripheralHandle);

        DisableClockCan(canIndex);
    }

    void CanStm::SendData(Id id, const Message& data, const infra::Function<void(bool success)>& actionOnCompletion)
    {
        onMessageSent = actionOnCompletion;

        peripheralCan[canIndex]->sTxMailBox[0].TDTR = data.size();
        peripheralCan[canIndex]->sTxMailBox[0].TDLR = 0;
        peripheralCan[canIndex]->sTxMailBox[0].TDHR = 0;

        peripheralCan[canIndex]->sTxMailBox[0].TDLR = reinterpret_cast<const uint32_t*>(data.data())[0];
        peripheralCan[canIndex]->sTxMailBox[0].TDHR = reinterpret_cast<const uint32_t*>(data.data())[1];

        peripheralCan[canIndex]->sTxMailBox[0].TIR = 0;

        if (id.Is11BitId())
        {
            peripheralCan[canIndex]->sTxMailBox[0].TIR |= id.Get11BitId() << 21;
        }
        else
        {
            peripheralCan[canIndex]->sTxMailBox[0].TIR |= CAN_TI0R_IDE;
            peripheralCan[canIndex]->sTxMailBox[0].TIR |= id.Get29BitId() << 3;
        }

        peripheralCan[canIndex]->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;
    }

    void CanStm::ReceiveData(const infra::Function<void(Id id, const Message& data)>& receivedAction)
    {
        onMessageReceived = receivedAction;
        peripheralCan[canIndex]->IER |= CAN_IER_FMPIE0;
    }

    void CanStm::HandleTxInterrupt()
    {
        onMessageSent((peripheralCan[canIndex]->TSR & CAN_TSR_TERR0) == 0);
        peripheralCan[canIndex]->TSR |= CAN_TSR_RQCP0;
        canTxInterruptDispatcher.ClearPending();
    }

    void CanStm::HandleRxInterrupt()
    {
        Id id = (peripheralCan[canIndex]->sFIFOMailBox[0].RIR & CAN_RI0R_IDE) == 0
                    ? Id::Create11BitId(peripheralCan[canIndex]->sFIFOMailBox[0].RIR >> 21)
                    : Id::Create29BitId(peripheralCan[canIndex]->sFIFOMailBox[0].RIR >> 3);

        Message message(8, uint8_t(0));
        reinterpret_cast<uint32_t*>(message.data())[0] = peripheralCan[canIndex]->sFIFOMailBox[0].RDLR;
        reinterpret_cast<uint32_t*>(message.data())[1] = peripheralCan[canIndex]->sFIFOMailBox[0].RDHR;
        message.resize(peripheralCan[canIndex]->sFIFOMailBox[0].RDTR & CAN_RDT0R_DLC);

        peripheralCan[canIndex]->RF0R |= CAN_RF0R_RFOM0;
        onMessageReceived(id, message);

        canRxInterruptDispatcher.ClearPending();
    }

    void CanStm::HandleErrorInterrupt()
    {
        ;
    }
}

#endif
