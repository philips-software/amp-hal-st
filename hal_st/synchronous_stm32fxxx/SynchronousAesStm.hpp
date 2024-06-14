#ifndef HAL_SYNCHRONOUS_AES_STM_HPP
#define HAL_SYNCHRONOUS_AES_STM_HPP

#include DEVICE_HEADER
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/util/ByteRange.hpp"

#if defined(HAS_PERIPHERAL_AES)

namespace hal
{
    class SynchronousAesEcbStm
    {
    public:
        enum class DataSwapping : uint8_t
        {
            disabled,
            halfWord,
            byte,
        };

        struct Config
        {
            DataSwapping dataSwapping = DataSwapping::disabled;
        };

        explicit SynchronousAesEcbStm(const Config& config);
        ~SynchronousAesEcbStm();

        void SetKey(const std::array<uint8_t, 16>& key) const;
        void Encrypt(infra::ConstByteRange input, infra::ByteRange output) const;
        void Decrypt(infra::ConstByteRange input, infra::ByteRange output) const;

    private:
        const std::size_t aesIndex = 0;
        Config config;
        mutable std::array<uint8_t, 16> localKey;

    private:
        void Enable() const;
        void Disable() const;
        void SetKey() const;
        void ConfigureDataSwapping() const;
        void ConfigureDecrypt() const;
        void Prepare() const;
        void Start(infra::ConstByteRange input, infra::ByteRange output) const;
    };
}

#endif

#endif
