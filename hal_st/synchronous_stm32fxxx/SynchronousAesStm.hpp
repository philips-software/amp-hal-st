#ifndef SYNCHRONOUS_HAL_INSTANTIATIONS_AES_STM_HPP
#define SYNCHRONOUS_HAL_INSTANTIATIONS_AES_STM_HPP

#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/util/ByteRange.hpp"
#include "services/util/Aes.hpp"

#if defined(HAS_PERIPHERAL_AES)

namespace hal
{
    namespace detail
    {
        /*
            The AES peripheral can be configured to perform a bit-, a byte-, a half-word-, or no
            swapping on the input data word in the AES_DINR register, before loading it to the AES
            processing core, and on the data output from the AES processing core, before sending it to
            the AES_DOUTR register.
        */
        enum class DataSwapping : uint8_t
        {
            disabled,
            halfWord,
            byte,
            bit,
        };

        struct SynchronousAes128EcbConfig
        {
            DataSwapping dataSwapping = DataSwapping::byte;
        };
    }

    class SynchronousAes128EcbStm
        : public services::Aes128Ecb
    {
    public:
        using Config = detail::SynchronousAes128EcbConfig;

        explicit SynchronousAes128EcbStm(const Config& config);
        ~SynchronousAes128EcbStm();

        void SetKey(const std::array<uint8_t, 16>& key) const override;
        void Encrypt(infra::ConstByteRange input, infra::ByteRange output) const override;
        void Decrypt(infra::ConstByteRange input, infra::ByteRange output) const override;

    private:
        const std::size_t aesIndex = 0;
        Config config;
        mutable std::array<uint8_t, 16> localKey;

    private:
        void Enable() const;
        void Disable() const;
        void Reset() const;
        void SetKey() const;
        void ConfigureDataSwapping() const;
        void ConfigureDecrypt() const;
        void Prepare() const;
        void Start(infra::ConstByteRange input, infra::ByteRange output) const;
    };
}

#endif
#endif
