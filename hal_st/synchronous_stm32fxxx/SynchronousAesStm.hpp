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
        enum class OperationMode : uint8_t
        {
            encrypt,
            decrypt,
        };

        struct Config
        {
            bool enableReverse = false;
            bool enableSwap = false;
            OperationMode operationMode = OperationMode::encrypt;
        };

        explicit SynchronousAesEcbStm(const Config& config);
        ~SynchronousAesEcbStm();

        void SetKey(const std::array<uint8_t, 16>& key) const;
        void Encrypt(infra::ConstByteRange input, infra::ByteRange output) const;

    private:
        const std::size_t index = 0;
        Config config;

    private:
        void SetDirectMode(const std::array<uint8_t, 16>& key) const;
        void SetReverseMode(const std::array<uint8_t, 16>& key) const;
        void ConfigureEncryption() const;
    };
}

#endif

#endif
