#include "hal_st/synchronous_interfaces/SynchronousAesStm.hpp"
#include "infra/util/ReallyAssert.hpp"
#include DEVICE_HEADER

#if defined(HAS_PERIPHERAL_AES)

namespace hal
{
    SynchronousAes128EcbStm::SynchronousAes128EcbStm(const Config& config)
        : config(config)
    {
        Enable();
    }

    SynchronousAes128EcbStm::~SynchronousAes128EcbStm()
    {
        Disable();
    }

    void SynchronousAes128EcbStm::SetKey(const std::array<uint8_t, 16>& key) const
    {
        std::copy_n(key.begin(), key.size(), localKey.begin());
    }

    void SynchronousAes128EcbStm::Encrypt(infra::ConstByteRange input, infra::ByteRange output) const
    {
        Reset();
        SetKey();
        Prepare();
        Start(input, output);
    }

    void SynchronousAes128EcbStm::Decrypt(infra::ConstByteRange input, infra::ByteRange output) const
    {
        Reset();
        SetKey();
        ConfigureDecrypt();
        Prepare();
        Start(input, output);
    }

    void SynchronousAes128EcbStm::ConfigureDecrypt() const
    {
        peripheralAes[aesIndex]->CR = AES_CR_MODE_0;
        peripheralAes[aesIndex]->CR |= AES_CR_EN;

        while (!(peripheralAes[aesIndex]->SR & AES_SR_CCF))
            ;

#if defined(AES_ICR_CCF)
        peripheralAes[aesIndex]->ICR |= AES_ICR_CCF;
#elif defined(AES_CR_CCFC)
        peripheralAes[aesIndex]->CR |= AES_CR_CCFC;
#endif
        peripheralAes[aesIndex]->CR = AES_CR_MODE_1;
    }

    void SynchronousAes128EcbStm::SetKey() const
    {
        auto key = infra::ReinterpretCastMemoryRange<uint32_t>(infra::MakeRange(localKey));

        peripheralAes[aesIndex]->KEYR3 = __REV(key[0]);
        peripheralAes[aesIndex]->KEYR2 = __REV(key[1]);
        peripheralAes[aesIndex]->KEYR1 = __REV(key[2]);
        peripheralAes[aesIndex]->KEYR0 = __REV(key[3]);
    }

    void SynchronousAes128EcbStm::ConfigureDataSwapping() const
    {
        if (config.dataSwapping == DataSwapping::halfWord)
            peripheralAes[aesIndex]->CR |= AES_CR_DATATYPE_0;

        if (config.dataSwapping == DataSwapping::byte)
            peripheralAes[aesIndex]->CR |= AES_CR_DATATYPE_1;

        if (config.dataSwapping == DataSwapping::bit)
            peripheralAes[aesIndex]->CR |= AES_CR_DATATYPE_0 | AES_CR_DATATYPE_1;
    }

    void SynchronousAes128EcbStm::Prepare() const
    {
        ConfigureDataSwapping();

#if defined(AES_SR_KEYVALID)
        while (!(peripheralAes[aesIndex]->SR & AES_SR_KEYVALID))
            ;
#endif
    }

    void SynchronousAes128EcbStm::Start(infra::ConstByteRange input, infra::ByteRange output) const
    {
        really_assert(input.size() == output.size() && input.size() % 16 == 0);

        auto length = input.size() / sizeof(uint32_t);
        auto inputRaw = reinterpret_cast<const uint32_t*>(input.begin());
        auto outputRaw = reinterpret_cast<uint32_t*>(output.begin());

        peripheralAes[aesIndex]->CR |= AES_CR_EN;

        for (auto index = 0; index < length; index += sizeof(uint32_t))
        {
            for (auto copyBuffIndex = 0; copyBuffIndex != sizeof(uint32_t); ++copyBuffIndex)
                peripheralAes[aesIndex]->DINR = inputRaw[index + copyBuffIndex];

            while (!(peripheralAes[aesIndex]->SR & AES_SR_CCF))
                ;

            for (auto copyBuffIndex = 0; copyBuffIndex != sizeof(uint32_t); ++copyBuffIndex)
                outputRaw[index + copyBuffIndex] = peripheralAes[aesIndex]->DOUTR;

#if defined(AES_ICR_CCF)
            peripheralAes[aesIndex]->ICR |= AES_ICR_CCF;
#elif defined(AES_CR_CCFC)
            peripheralAes[aesIndex]->CR |= AES_CR_CCFC;
#endif
        }
    }

    void SynchronousAes128EcbStm::Enable() const
    {
        EnableClockAes(aesIndex);
        Reset();
    }

    void SynchronousAes128EcbStm::Disable() const
    {
        peripheralAes[aesIndex]->CR &= ~AES_CR_EN;
        DisableClockAes(aesIndex);
    }

    void SynchronousAes128EcbStm::Reset() const
    {
        peripheralAes[aesIndex]->CR &= ~AES_CR_EN;
#if defined(AES_CR_IPRST)
        peripheralAes[aesIndex]->CR |= AES_CR_IPRST;
#endif
        peripheralAes[aesIndex]->CR = 0;
    }
}

#endif
