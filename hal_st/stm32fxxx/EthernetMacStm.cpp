#include "hal_st/stm32fxxx/EthernetMacStm.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/BitLogic.hpp"

#if defined(HAS_PERIPHERAL_ETHERNET)

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
        peripheralEthernet[0]->MACA0LR = reinterpret_cast<const uint32_t*>(macAddress.data())[0];
        peripheralEthernet[0]->MACA0HR = reinterpret_cast<const uint32_t*>(macAddress.data())[1] & 0xffff;

        peripheralEthernet[0]->DMAIER = ETH_DMAIER_TIE | ETH_DMAIER_RIE | ETH_DMAIER_RPSIE | ETH_DMAIER_FBEIE | ETH_DMAIER_AISE | ETH_DMAIER_NISE;
        peripheralEthernet[0]->DMABMR = ETH_DMABMR_EDE | ETH_DMABMR_PBL_1Beat;

        peripheralEthernet[0]->MACCR = ((linkSpeed == LinkSpeed::fullDuplex100MHz || linkSpeed == LinkSpeed::halfDuplex100MHz) ? ETH_MACCR_FES : 0) | ((linkSpeed == LinkSpeed::fullDuplex100MHz || linkSpeed == LinkSpeed::fullDuplex10MHz) ? ETH_MACCR_DM : 0) | ETH_MACCR_IPCO | ETH_MACCR_TE | ETH_MACCR_RE;

        peripheralEthernet[0]->DMAOMR = ETH_DMAOMR_SR | ETH_DMAOMR_ST | ETH_DMAOMR_TSF | ETH_DMAOMR_DFRF | ETH_DMAOMR_RSF | ETH_DMAOMR_FEF;
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
        peripheralEthernet[0]->DMABMR |= ETH_DMABMR_SR;
        while ((peripheralEthernet[0]->DMABMR & ETH_DMABMR_SR) != 0)
        {}
    }

    void EthernetMacStm::Interrupt()
    {
        // Normal interrupt summary
        if ((peripheralEthernet[0]->DMASR & ETH_DMASR_NIS) != 0)
        {
            peripheralEthernet[0]->DMASR = ETH_DMASR_NIS;
            // Transmit status
            if ((peripheralEthernet[0]->DMASR & ETH_DMASR_TS) != 0)
            {
                peripheralEthernet[0]->DMASR = ETH_DMASR_TS;
                sendDescriptors.SentFrame();
            }

            // Receive status
            if ((peripheralEthernet[0]->DMASR & ETH_DMASR_RS) != 0)
            {
                peripheralEthernet[0]->DMASR = ETH_DMASR_RS;
                receiveDescriptors.ReceivedFrame();
            }
        }

        // Abnormal interrupt summary
        if ((peripheralEthernet[0]->DMASR & ETH_DMASR_AIS) != 0)
        {
            // Receiver process stopped: Indicates an error in our logic
            if ((peripheralEthernet[0]->DMASR & ETH_DMASR_RPSS) != 0)
                std::abort();

            // Fatal bus error by ethernet DMA: Indicates an error in setting up descriptors
            if ((peripheralEthernet[0]->DMASR & ETH_DMASR_FBES) != 0)
                std::abort();

            peripheralEthernet[0]->DMASR = ETH_DMASR_AIS;
        }
    }

    EthernetMacStm::ReceiveDescriptors::ReceiveDescriptors(EthernetMacStm& ethernetMac)
        : ethernetMac(ethernetMac)
    {
        for (auto& descriptor : descriptors)
        {
            descriptor.DESC0 = ETH_DMARXDESC_RCH;
            descriptor.DESC1 = 0;
            descriptor.DESC3 = reinterpret_cast<uint32_t>(&descriptor + 1);
            descriptor.DESC4 = 0;
        }
        descriptors.back().DESC1 |= ETH_DMARXDESC_RER;
        descriptors.back().DESC3 = reinterpret_cast<uint32_t>(&descriptors.front());

        peripheralEthernet[0]->DMARDLAR = reinterpret_cast<uint32_t>(descriptors.data());

        infra::EventDispatcher::Instance().Schedule([this]()
            {
                // This is scheduled so that the observer is instantiated
                RequestReceiveBuffers();
            });
    }

    void EthernetMacStm::ReceiveDescriptors::ReceivedFrame()
    {
        while (receivedFramesAllocated != 0 && (descriptors[receiveDescriptorReceiveIndex].DESC0 & ETH_DMARXDESC_OWN) == 0)
        {
            bool receiveDone = (descriptors[receiveDescriptorReceiveIndex].DESC0 & ETH_DMARXDESC_OWN) == 0 && (descriptors[receiveDescriptorReceiveIndex].DESC0 & ETH_DMARXDESC_LS) != 0;
            uint16_t frameSize = (descriptors[receiveDescriptorReceiveIndex].DESC0 & ETH_DMARXDESC_FL) >> 16;
            bool errorFrame = (descriptors[receiveDescriptorReceiveIndex].DESC0 & ETH_DMARXDESC_ES) != 0 && (descriptors[receiveDescriptorReceiveIndex].DESC0 & ETH_DMARXDESC_LS) != 0;
            descriptors[receiveDescriptorReceiveIndex].DESC2 = 0;
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
    }

    bool EthernetMacStm::ReceiveDescriptors::RequestReceiveBuffer()
    {
        assert((descriptors[receiveDescriptorAllocatedIndex].DESC0 & ETH_DMARXDESC_OWN) == 0);

        infra::ByteRange buffer = ethernetMac.GetObserver().RequestReceiveBuffer();
        if (buffer.empty())
            return false;

        descriptors[receiveDescriptorAllocatedIndex].DESC0 &= ~(ETH_DMARXDESC_MAMPCE | ETH_DMARXDESC_CE | ETH_DMARXDESC_DBE | ETH_DMARXDESC_RE | ETH_DMARXDESC_RWT | ETH_DMARXDESC_LC | ETH_DMARXDESC_IPV4HCE | ETH_DMARXDESC_LS | ETH_DMARXDESC_VLAN | ETH_DMARXDESC_OE | ETH_DMARXDESC_LE | ETH_DMARXDESC_SAF | ETH_DMARXDESC_DE | ETH_DMARXDESC_ES | ETH_DMARXDESC_FL | ETH_DMARXDESC_AFM);
        descriptors[receiveDescriptorAllocatedIndex].DESC1 = buffer.size() | ETH_DMARXDESC_RCH;
        descriptors[receiveDescriptorAllocatedIndex].DESC2 = reinterpret_cast<uint32_t>(buffer.begin());
        descriptors[receiveDescriptorAllocatedIndex].DESC0 |= ETH_DMARXDESC_OWN;

        __DSB();
        peripheralEthernet[0]->DMASR = ETH_DMASR_RBUS;
        peripheralEthernet[0]->DMARPDR = 1;

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
            descriptor.DESC0 = ETH_DMATXDESC_TCH | ETH_DMATXDESC_CIC_TCPUDPICMP_FULL | ETH_DMATXDESC_IC;
            descriptor.DESC3 = reinterpret_cast<uint32_t>(&descriptor + 1);
        }
        descriptors.back().DESC0 |= ETH_DMATXDESC_TER;
        descriptors.back().DESC3 = reinterpret_cast<uint32_t>(&descriptors.front());

        peripheralEthernet[0]->DMATDLAR = reinterpret_cast<uint32_t>(descriptors.data());
    }

    void EthernetMacStm::SendDescriptors::SendBuffer(infra::ConstByteRange data, bool last)
    {
        assert((descriptors[sendDescriptorIndex].DESC0 & ETH_DMATXDESC_OWN) == 0);
        descriptors[sendDescriptorIndex].DESC1 = data.size();
        descriptors[sendDescriptorIndex].DESC2 = reinterpret_cast<uint32_t>(data.begin());

        if (sendFirst)
            descriptors[sendDescriptorIndex].DESC0 |= ETH_DMATXDESC_FS;
        else
            descriptors[sendDescriptorIndex].DESC0 &= ~ETH_DMATXDESC_FS;

        if (last)
            descriptors[sendDescriptorIndex].DESC0 |= ETH_DMATXDESC_LS;
        else
            descriptors[sendDescriptorIndex].DESC0 &= ~ETH_DMATXDESC_LS;

        descriptors[sendDescriptorIndex].DESC0 &= ~(ETH_DMATXDESC_DB | ETH_DMATXDESC_UF | ETH_DMATXDESC_ED | ETH_DMATXDESC_CC | ETH_DMATXDESC_EC | ETH_DMATXDESC_LCO | ETH_DMATXDESC_NC | ETH_DMATXDESC_LCA | ETH_DMATXDESC_PCE | ETH_DMATXDESC_FF | ETH_DMATXDESC_JT | ETH_DMATXDESC_ES | ETH_DMATXDESC_IHE);

        if (sendFirst)
            sendDescriptorIndexFirst = sendDescriptorIndex;
        else
            descriptors[sendDescriptorIndex].DESC0 |= ETH_DMATXDESC_OWN;
        if (last)
            descriptors[sendDescriptorIndexFirst].DESC0 |= ETH_DMATXDESC_OWN;
        __DSB();
        sendFirst = last;

        peripheralEthernet[0]->DMATPDR = 1;

        ++sendDescriptorIndex;
        if (sendDescriptorIndex == descriptors.size())
            sendDescriptorIndex = 0;
    }

    void EthernetMacStm::SendDescriptors::SentFrame()
    {
        uint32_t previousDescriptor = sendDescriptorIndex != 0 ? sendDescriptorIndex - 1 : descriptors.size() - 1;

        bool sentDone = (descriptors[previousDescriptor].DESC0 & ETH_DMATXDESC_LS) != 0 && (descriptors[previousDescriptor].DESC0 & ETH_DMATXDESC_OWN) == 0;
        assert(sentDone);
        if (sentDone)
        {
            descriptors[previousDescriptor].DESC0 &= ~ETH_DMATXDESC_LS;
            ethernetMac.GetObserver().SentFrame();
        }
    }
}

#endif
