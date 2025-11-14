#include "hal_st/stm32fxxx/EthernetMacStm.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/BitLogic.hpp"
#include DEVICE_HEADER

namespace hal
{
    EthernetMacStm::EthernetMacStm(EthernetSmi& ethernetSmi, LinkSpeed linkSpeed, std::array<uint8_t, 6> macAddress)
        : ethernetSmi(ethernetSmi)
        , macAddress(macAddress)
        , interrupt(peripheralEthernetIrq[0], [this]()
              {
                  Interrupt();
              })
        , receiveDescriptors(*this)
        , sendDescriptors(*this)
    {
        // Set Channel Tx descriptor list address register
        peripheralEthernet[0]->DMACTDLAR = reinterpret_cast<uint32_t>(&sendDescriptors.descriptors[0]);
        // Set tail pointer to first element
        peripheralEthernet[0]->DMACTDTPR = reinterpret_cast<uint32_t>(&sendDescriptors.descriptors[0]);
        // Set Channel Tx descriptor ring length register (must be len - 1)
        peripheralEthernet[0]->DMACTDRLR = sendDescriptors.descriptors.max_size() - 1;

        // Set Channel Rx descriptor list address register
        peripheralEthernet[0]->DMACRDLAR = reinterpret_cast<uint32_t>(&receiveDescriptors.descriptors[0]);
        // Init tail pointer to first element, will be changed after allocation of buffer
        peripheralEthernet[0]->DMACRDTPR = reinterpret_cast<uint32_t>(&receiveDescriptors.descriptors[0]);
        // Set Channel Rx descriptor ring length register (must be len - 1)
        peripheralEthernet[0]->DMACRDRLR = receiveDescriptors.descriptors.max_size() - 1;

        // Set MAC address
        peripheralEthernet[0]->MACA0LR = reinterpret_cast<const uint32_t*>(macAddress.data())[0];
        peripheralEthernet[0]->MACA0HR = reinterpret_cast<const uint32_t*>(macAddress.data())[1] & 0xffff;

        // Set default MAC settings like in HAL_ETH see ETH_MACDMAConfig()
        peripheralEthernet[0]->MACCR |= ((linkSpeed == LinkSpeed::fullDuplex100MHz || linkSpeed == LinkSpeed::halfDuplex100MHz) ? ETH_MACCR_FES : 0) |
                                        ((linkSpeed == LinkSpeed::fullDuplex100MHz || linkSpeed == LinkSpeed::fullDuplex10MHz) ? ETH_MACCR_DM : 0) |
                                        ETH_MACCR_SARC_REPADDR0 | ETH_MACCR_CST | ETH_MACCR_IPC | ETH_MACCR_ACS | ETH_MACCR_JD | ETH_MACCR_TE | ETH_MACCR_RE;

        // Set Transmit Store and Forward
        peripheralEthernet[0]->MTLTQOMR |= ETH_MTLTQOMR_TSF;

        // Set default DMA settings like in HAL_ETH see ETH_MACDMAConfig()
        peripheralEthernet[0]->DMASBMR |= ETH_DMASBMR_AAL;
        peripheralEthernet[0]->DMACCR |= ETH_DMACCR_DSL_64BIT;
        peripheralEthernet[0]->DMACTCR |= ETH_DMACTCR_TPBL_32PBL;
        peripheralEthernet[0]->DMACRCR |= ETH_DMACRCR_RPBL_32PBL;

        // Disable Rx MMC Interrupts (mask register -> 1 = OFF)
        peripheralEthernet[0]->MMCRIMR |= ETH_MMCRIMR_RXLPITRCIM | ETH_MMCRIMR_RXLPIUSCIM | ETH_MMCRIMR_RXUCGPIM | ETH_MMCRIMR_RXALGNERPIM | ETH_MMCRIMR_RXCRCERPIM;

        // Disable Tx MMC Interrupts (mask register -> 1 = OFF)
        peripheralEthernet[0]->MMCTIMR |= ETH_MMCTIMR_TXLPITRCIM | ETH_MMCTIMR_TXLPIUSCIM | ETH_MMCTIMR_TXGPKTIM | ETH_MMCTIMR_TXMCOLGPIM | ETH_MMCTIMR_TXSCOLGPIM;

        // Enable the DMA transmission
        peripheralEthernet[0]->DMACTCR |= ETH_DMACTCR_ST;

        // Clear Tx and Rx process stopped flags
        peripheralEthernet[0]->DMACSR |= (ETH_DMACSR_TPS | ETH_DMACSR_RPS);

        // Set the Flush Transmit FIFO bit
        peripheralEthernet[0]->MTLTQOMR |= ETH_MTLTQOMR_FTQ;

        // Enable interrupts
        peripheralEthernet[0]->DMACIER |= ETH_DMACIER_TIE | ETH_DMACIER_RIE | ETH_DMACIER_RSE | ETH_DMACIER_FBEE | ETH_DMACIER_AIE | ETH_DMACIER_NIE;
    }

    EthernetMacStm::~EthernetMacStm()
    {
        ResetDma();
        peripheralEthernet[0]->MACCR = 0;
    }

    void EthernetMacStm::SendBuffer(infra::ConstByteRange data, bool last)
    {
        sendDescriptors.SendBuffer(data, last);
    }

    void EthernetMacStm::RetryAllocation()
    {
        receiveDescriptors.RequestReceiveBuffers();
    }

    void EthernetMacStm::AddMacAddressFilter(MacAddress address)
    {
        uint32_t lr = reinterpret_cast<const uint32_t*>(address.data())[0];
        uint32_t hr = (reinterpret_cast<const uint32_t*>(address.data())[1] & 0xffff) | (1 << 31);

        if ((peripheralEthernet[0]->MACA1HR & infra::Bit<uint32_t>(31)) == 0)
        {
            peripheralEthernet[0]->MACA1LR = lr;
            peripheralEthernet[0]->MACA1HR = hr;
        }
        else if ((peripheralEthernet[0]->MACA2HR & infra::Bit<uint32_t>(31)) == 0)
        {
            peripheralEthernet[0]->MACA2LR = lr;
            peripheralEthernet[0]->MACA2HR = hr;
        }
        else if ((peripheralEthernet[0]->MACA3HR & infra::Bit<uint32_t>(31)) == 0)
        {
            peripheralEthernet[0]->MACA3LR = lr;
            peripheralEthernet[0]->MACA3HR = hr;
        }
        else
        {
            // No free mac address found. Hint: implement address hashing
            abort();
        }
    }

    void EthernetMacStm::RemoveMacAddressFilter(MacAddress address)
    {
        uint32_t lr = reinterpret_cast<const uint32_t*>(address.data())[0];
        uint32_t hr = (reinterpret_cast<const uint32_t*>(address.data())[1] & 0xffff) | (1 << 31);

        if (peripheralEthernet[0]->MACA1HR == hr && peripheralEthernet[0]->MACA1LR == lr)
        {
            peripheralEthernet[0]->MACA1LR = 0;
            peripheralEthernet[0]->MACA1HR = 0;
        }
        else if (peripheralEthernet[0]->MACA2HR == hr && peripheralEthernet[0]->MACA2LR == lr)
        {
            peripheralEthernet[0]->MACA2LR = 0;
            peripheralEthernet[0]->MACA2HR = 0;
        }
        else if (peripheralEthernet[0]->MACA3HR == hr && peripheralEthernet[0]->MACA3LR == lr)
        {
            peripheralEthernet[0]->MACA3LR = 0;
            peripheralEthernet[0]->MACA3HR = 0;
        }
        else
        {
            // Address not found
            abort();
        }
    }

    void EthernetMacStm::ResetDma()
    {
        peripheralEthernet[0]->DMAMR |= ETH_DMAMR_SWR;
        while ((peripheralEthernet[0]->DMAMR & ETH_DMAMR_SWR) != 0)
        {
        }
    }

    void EthernetMacStm::Interrupt()
    {
        // Normal interrupt summary
        if ((peripheralEthernet[0]->DMACSR & ETH_DMACSR_NIS) != 0)
        {
            peripheralEthernet[0]->DMACSR = ETH_DMACSR_NIS;
            // Transmit status
            if ((peripheralEthernet[0]->DMACSR & ETH_DMACSR_TI) != 0)
            {
                peripheralEthernet[0]->DMACSR = ETH_DMACSR_TI;
                sendDescriptors.SentFrame();
            }

            // Receive status
            if ((peripheralEthernet[0]->DMACSR & ETH_DMACSR_RI) != 0)
            {
                peripheralEthernet[0]->DMACSR = ETH_DMACSR_RI;
                receiveDescriptors.ReceivedFrame();
            }
        }

        // Abnormal interrupt summary
        if ((peripheralEthernet[0]->DMACSR & ETH_DMACSR_AIS) != 0)
        {
            // Receiver process stopped: Indicates an error in our logic
            if ((peripheralEthernet[0]->DMACSR & ETH_DMACSR_RPS) != 0)
                std::abort();

            // Fatal bus error by ethernet DMA: Indicates an error in setting up descriptors
            if ((peripheralEthernet[0]->DMACSR & ETH_DMACSR_FBE) != 0)
                std::abort();

            peripheralEthernet[0]->DMACSR |= ETH_DMACSR_AIS | ETH_DMACSR_RPS | ETH_DMACSR_FBE;
        }
    }

    EthernetMacStm::ReceiveDescriptors::ReceiveDescriptors(EthernetMacStm& ethernetMac)
        : ethernetMac(ethernetMac)
    {
        for (auto& descriptor : descriptors)
        {
            descriptor.DESC0 = 0;
            descriptor.DESC1 = 0;
            descriptor.DESC2 = 0;
            descriptor.DESC3 = 0;
        }

        infra::EventDispatcher::Instance().Schedule([this]()
            {
                // This is scheduled so that the observer is instantiated
                RequestReceiveBuffers();
            });
    }

    void EthernetMacStm::ReceiveDescriptors::ReceivedFrame()
    {
        while (receivedFramesAllocated != 0 && (descriptors[receiveDescriptorReceiveIndex].DESC3 & ETH_DMARXNDESCWBF_OWN) == 0)
        {
            bool receiveDone = (descriptors[receiveDescriptorReceiveIndex].DESC3 & ETH_DMARXNDESCWBF_OWN) == 0 && (descriptors[receiveDescriptorReceiveIndex].DESC3 & ETH_DMARXNDESCWBF_LD) != 0;
            uint16_t frameSize = (descriptors[receiveDescriptorReceiveIndex].DESC3 & ETH_DMARXNDESCWBF_PL);
            bool errorFrame = (descriptors[receiveDescriptorReceiveIndex].DESC3 & ETH_DMARXNDESCWBF_ES) != 0 && (descriptors[receiveDescriptorReceiveIndex].DESC3 & ETH_DMARXNDESCWBF_LD) != 0;
            descriptors[receiveDescriptorReceiveIndex].DESC0 = 0; // Buffer address
            ++receivedFrameBuffers;
            --receivedFramesAllocated;
            ++receiveDescriptorReceiveIndex;
            if (receiveDescriptorReceiveIndex == descriptors.size())
                receiveDescriptorReceiveIndex = 0;

            RequestReceiveBuffer();

            if (receiveDone)
            {
                if (!errorFrame)
                    ethernetMac.GetObserver().ReceivedFrame(receivedFrameBuffers, frameSize);
                else
                    ethernetMac.GetObserver().ReceivedErrorFrame(receivedFrameBuffers, frameSize);
                receivedFrameBuffers = 0;
            }
        }
    }

    void EthernetMacStm::ReceiveDescriptors::RequestReceiveBuffers()
    {
        while (receivedFramesAllocated != descriptors.size())
            if (!RequestReceiveBuffer())
                break;

        // Enable the MAC reception
        peripheralEthernet[0]->MACCR |= ETH_MACCR_RE;
        // Start Receive
        peripheralEthernet[0]->DMACRCR |= ETH_DMACRCR_SR;
    }

    bool EthernetMacStm::ReceiveDescriptors::RequestReceiveBuffer()
    {
        assert((descriptors[receiveDescriptorAllocatedIndex].DESC3 & ETH_DMARXNDESCWBF_OWN) == 0);

        infra::ByteRange buffer = ethernetMac.GetObserver().RequestReceiveBuffer();
        if (buffer.empty())
            return false;

        descriptors[receiveDescriptorAllocatedIndex].DESC0 = reinterpret_cast<uint32_t>(buffer.begin());
        descriptors[receiveDescriptorAllocatedIndex].BackupAddr0 = reinterpret_cast<uint32_t>(buffer.begin());
        descriptors[receiveDescriptorAllocatedIndex].DESC3 = ETH_DMARXNDESCRF_OWN | ETH_DMARXNDESCRF_IOC | ETH_DMARXNDESCRF_BUF1V;

        // DMB instruction to avoid race condition
        __DMB();

        // Set tail pointer to last element
        peripheralEthernet[0]->DMACRDTPR = reinterpret_cast<uint32_t>(&descriptors[receiveDescriptorAllocatedIndex]);

        ++receivedFramesAllocated;
        ++receiveDescriptorAllocatedIndex;
        if (receiveDescriptorAllocatedIndex == descriptors.size())
            receiveDescriptorAllocatedIndex = 0;

        return true;
    }

    EthernetMacStm::SendDescriptors::SendDescriptors(EthernetMacStm& ethernetMac)
        : ethernetMac(ethernetMac)
    {
        for (auto& descriptor : descriptors)
        {
            descriptor.DESC0 = 0;
            descriptor.DESC1 = 0;
            descriptor.DESC2 = 0;
            descriptor.DESC3 = 0;
        }
    }

    void EthernetMacStm::SendDescriptors::SendBuffer(infra::ConstByteRange data, bool last)
    {
        assert((descriptors[sendDescriptorIndex].DESC3 & ETH_DMATXNDESCRF_OWN) == 0);
        descriptors[sendDescriptorIndex].DESC0 = reinterpret_cast<uint32_t>(data.begin());
        descriptors[sendDescriptorIndex].DESC1 = 0;
        descriptors[sendDescriptorIndex].DESC2 = data.size() | ETH_DMATXNDESCRF_IOC;
        if (last)
            descriptors[sendDescriptorIndex].DESC2 |= ETH_DMATXNDESCRF_IOC; // Set IOC bit on last
        MODIFY_REG(descriptors[sendDescriptorIndex].DESC3, ETH_DMATXNDESCRF_B2L, 0);

        if (sendFirst)
            descriptors[sendDescriptorIndex].DESC3 |= ETH_DMATXNDESCRF_FD;
        else
            descriptors[sendDescriptorIndex].DESC3 &= ~ETH_DMATXNDESCRF_FD;

        if (last)
            descriptors[sendDescriptorIndex].DESC3 |= ETH_DMATXNDESCRF_LD;
        else
            descriptors[sendDescriptorIndex].DESC3 &= ~ETH_DMATXNDESCRF_LD;

        // Clear status bits
        descriptors[sendDescriptorIndex].DESC3 &= ~(ETH_DMATXNDESCWBF_DB | ETH_DMATXNDESCWBF_UF | ETH_DMATXNDESCWBF_ED | ETH_DMATXNDESCWBF_CC | ETH_DMATXNDESCWBF_EC | ETH_DMATXNDESCWBF_LCO | ETH_DMATXNDESCWBF_NC | ETH_DMATXNDESCWBF_LCA | ETH_DMATXNDESCWBF_PCE | ETH_DMATXNDESCWBF_FF | ETH_DMATXNDESCWBF_JT | ETH_DMATXNDESCWBF_ES | ETH_DMATXNDESCWBF_IHE);
        descriptors[sendDescriptorIndex].DESC3 |= ETH_DMATXNDESCRF_CIC_IPHDR_PAYLOAD_INSERT_PHDR_CALC;

        __DMB();

        if (sendFirst)
            sendDescriptorIndexFirst = sendDescriptorIndex;
        else
            descriptors[sendDescriptorIndex].DESC3 |= ETH_DMATXNDESCRF_OWN;
        if (last)
            descriptors[sendDescriptorIndexFirst].DESC3 |= ETH_DMATXNDESCRF_OWN;

        __DSB();

        sendFirst = last;

        ++sendDescriptorIndex;
        if (sendDescriptorIndex == descriptors.size())
            sendDescriptorIndex = 0;

        // Start transmission -> issue a poll command to Tx DMA by writing address of next immediate free descriptor
        peripheralEthernet[0]->DMACTDTPR = reinterpret_cast<uint32_t>(&descriptors[sendDescriptorIndex]);
    }

    void EthernetMacStm::SendDescriptors::SentFrame()
    {
        uint32_t previousDescriptor = sendDescriptorIndex != 0 ? sendDescriptorIndex - 1 : descriptors.size() - 1;

        bool sentDone = (descriptors[previousDescriptor].DESC3 & ETH_DMATXCDESC_OWN) == 0;
        assert(sentDone);
        if (sentDone)
        {
            descriptors[previousDescriptor].DESC3 &= ~ETH_DMATXNDESCRF_LD;
            ethernetMac.GetObserver().SentFrame();
        }
    }
}
