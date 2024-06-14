#include "hal_st/synchronous_stm32fxxx/SynchronousAesStm.hpp"
#include "infra/util/MemoryRange.hpp"
#include "infra/util/ReallyAssert.hpp"

#if defined(HAS_PERIPHERAL_AES)

namespace hal
{
    SynchronousAesEcbStm::SynchronousAesEcbStm(const Config& config)
        : config(config)
    {}

    SynchronousAesEcbStm::~SynchronousAesEcbStm()
    {
        Disable();
    }

    void SynchronousAesEcbStm::SetKey(const std::array<uint8_t, 16>& key) const
    {
        std::copy_n(key.begin(), key.size(), localKey.begin());
    }

    void SynchronousAesEcbStm::Encrypt(infra::ConstByteRange input, infra::ByteRange output) const
    {
        Enable();
        SetKey();
        Prepare();
        Start(input, output);
    }

    void SynchronousAesEcbStm::Decrypt(infra::ConstByteRange input, infra::ByteRange output) const
    {
        Enable();
        SetKey();
        ConfigureDecrypt();
        Prepare();
        Start(input, output);
    }

    void SynchronousAesEcbStm::ConfigureDecrypt() const
    {
        peripheralAes[aesIndex]->CR = AES_CR_MODE_0;
        peripheralAes[aesIndex]->CR |= AES_CR_EN;

        while (!(peripheralAes[aesIndex]->SR & AES_SR_CCF))
        {}

        peripheralAes[aesIndex]->ICR |= AES_ICR_CCF;
        peripheralAes[aesIndex]->CR = AES_CR_MODE_1;
    }

    void SynchronousAesEcbStm::SetKey() const
    {
        auto key = infra::ReinterpretCastMemoryRange<uint32_t>(infra::MakeRange(localKey));

        peripheralAes[aesIndex]->KEYR3 = key[0];
        peripheralAes[aesIndex]->KEYR2 = key[1];
        peripheralAes[aesIndex]->KEYR1 = key[2];
        peripheralAes[aesIndex]->KEYR0 = key[3];
    }

    void SynchronousAesEcbStm::ConfigureDataSwapping() const
    {
        if (config.dataSwapping == DataSwapping::halfWord)
            peripheralAes[aesIndex]->CR |= AES_CR_DATATYPE_0;

        if (config.dataSwapping == DataSwapping::byte)
            peripheralAes[aesIndex]->CR |= AES_CR_DATATYPE_1;
    }

    void SynchronousAesEcbStm::Prepare() const
    {
        ConfigureDataSwapping();

        while (!(peripheralAes[aesIndex]->SR & AES_SR_KEYVALID))
        {}
    }

    void SynchronousAesEcbStm::Start(infra::ConstByteRange input, infra::ByteRange output) const
    {
        really_assert(input.size() == output.size() && input.size() % 16 == 0);

        auto length = input.size() / sizeof(uint32_t);
        auto inputRaw = reinterpret_cast<const uint32_t *>(input.begin());
        auto outputRaw = reinterpret_cast<uint32_t *>(output.begin());

        peripheralAes[aesIndex]->CR |= AES_CR_EN;

        for (auto index = 0; index < length; index += sizeof(uint32_t))
        {
            for (auto copyBuffIndex = 0; copyBuffIndex < sizeof(uint32_t); ++copyBuffIndex)
                peripheralAes[aesIndex]->DINR = inputRaw[index + copyBuffIndex];

            while (!(peripheralAes[aesIndex]->SR & AES_SR_CCF))
            {}
            peripheralAes[aesIndex]->ICR |= AES_ICR_CCF;

            for (auto copyBuffIndex = 0; copyBuffIndex < sizeof(uint32_t); ++copyBuffIndex)
                outputRaw[index + copyBuffIndex] = peripheralAes[aesIndex]->DOUTR;
        }

        Disable();
    }

    void SynchronousAesEcbStm::Enable() const
    {
        EnableClockAes(aesIndex);

        peripheralAes[aesIndex]->CR &=~ AES_CR_EN;
        peripheralAes[aesIndex]->CR |= AES_CR_IPRST;
        peripheralAes[aesIndex]->CR = 0;
    }

    void SynchronousAesEcbStm::Disable() const
    {
        peripheralAes[aesIndex]->CR &=~ AES_CR_EN;
        DisableClockAes(aesIndex);
    }
}

#endif
