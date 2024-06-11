#include "hal_st/synchronous_stm32fxxx/SynchronousAesStm.hpp"

namespace hal
{
    SynchronousAesEcbStm::SynchronousAesEcbStm(const Config& config)
        : config(config)
    {
        EnableClockAes(index);
    }

    SynchronousAesEcbStm::~SynchronousAesEcbStm()
    {
        peripheralAes[index]->CR = AES_CR_CHMOD | AES_CR_KEYSIZE | AES_CR_DATATYPE;
        DisableClockAes(index);
    }

    void SynchronousAesEcbStm::SetKey(const std::array<uint8_t, 16>& key) const
    {
        peripheralAes[index]->CR = AES_CR_CHMOD | AES_CR_KEYSIZE | AES_CR_DATATYPE;

        if (!config.enableReverse)
            SetDirectMode(key);
        else
            SetReverseMode(key);

        if (config.operationMode == OperationMode::encrypt)
            ConfigureEncryption();

        if (config.enableSwap)
            peripheralAes[index]->CR |= AES_CR_DATATYPE_1;

        while (!(peripheralAes[index]->SR & AES_SR_KEYVALID))
        {}

        peripheralAes[index]->CR |= AES_CR_EN;
    }

    void SynchronousAesEcbStm::Encrypt(infra::ConstByteRange input, infra::ByteRange output) const
    {
        auto inputRaw = reinterpret_cast<const uint32_t *>(input.begin());
        auto outputRaw = reinterpret_cast<uint32_t *>(output.begin());

        /* Write the input block into the input FIFO */
        peripheralAes[index]->DINR = inputRaw[0];
        peripheralAes[index]->DINR = inputRaw[1];
        peripheralAes[index]->DINR = inputRaw[2];
        peripheralAes[index]->DINR = inputRaw[3];

        /* Wait for CCF flag to be raised */
        while (!(peripheralAes[index]->SR & AES_SR_CCF))
        {}

        /* Read the output block from the output FIFO */
        outputRaw[0] = peripheralAes[index]->DOUTR;
        outputRaw[1] = peripheralAes[index]->DOUTR;
        outputRaw[2] = peripheralAes[index]->DOUTR;
        outputRaw[3] = peripheralAes[index]->DOUTR;

        /* Clear CCF Flag */
        peripheralAes[index]->ICR |= AES_ICR_CCF;
    }

    void SynchronousAesEcbStm::SetDirectMode(const std::array<uint8_t, 16>& key) const
    {
        auto data = reinterpret_cast<const uint32_t *>(key.data());

        peripheralAes[index]->KEYR3 = __REV(data[0]);
        peripheralAes[index]->KEYR2 = __REV(data[1]);
        peripheralAes[index]->KEYR1 = __REV(data[2]);
        peripheralAes[index]->KEYR0 = __REV(data[3]);
    }

    void SynchronousAesEcbStm::SetReverseMode(const std::array<uint8_t, 16>& key) const
    {
        auto data = reinterpret_cast<const uint32_t *>(key.data());

        peripheralAes[index]->KEYR0 = data[0];
        peripheralAes[index]->KEYR1 = data[1];
        peripheralAes[index]->KEYR2 = data[2];
        peripheralAes[index]->KEYR3 = data[3];
    }

    void SynchronousAesEcbStm::ConfigureEncryption() const
    {
        peripheralAes[index]->CR = AES_CR_MODE_0;
        peripheralAes[index]->CR |= AES_CR_EN;

        while (!(peripheralAes[index]->SR & AES_SR_CCF))
        {}

        peripheralAes[index]->ICR |= AES_ICR_CCF;
        peripheralAes[index]->CR = AES_CR_MODE_1;
    }
}
