#include "services/st_util/MbedTlsGcmAccelerationAes.hpp"
#include "mbedtls/platform_util.h"
#include <algorithm>
#include <cstring>

extern "C"
{
    int __real_mbedtls_gcm_setkey(mbedtls_gcm_context* ctx, mbedtls_cipher_id_t cipher, const unsigned char* key, unsigned int keybits);
    void __real_mbedtls_gcm_free(mbedtls_gcm_context* ctx);
    int __real_mbedtls_gcm_crypt_and_tag(mbedtls_gcm_context* ctx, int mode, size_t length, const unsigned char* iv, size_t iv_len, const unsigned char* add, size_t add_len, const unsigned char* input, unsigned char* output, size_t tag_len, unsigned char* tag);
    int __real_mbedtls_gcm_auth_decrypt(mbedtls_gcm_context* ctx, size_t length, const unsigned char* iv, size_t iv_len, const unsigned char* add, size_t add_len, const unsigned char* tag, size_t tag_len, const unsigned char* input, unsigned char* output);
}

namespace services
{
    namespace
    {
        void Wipe(infra::ByteRange range)
        {
            volatile uint8_t* bytes = range.begin();
            for (std::size_t i = 0; i != range.size(); ++i)
                bytes[i] = 0;
        }

        template<std::size_t N>
        std::array<uint8_t, N> Pattern(uint8_t seed)
        {
            std::array<uint8_t, N> result;
            for (std::size_t i = 0; i != N; ++i)
                result[i] = static_cast<uint8_t>(seed + i * 7);
            return result;
        }

        // Compares the peripheral with mbedtls for header and payload sizes with and without partial blocks
        template<std::size_t AdditionalDataSize, std::size_t PayloadSize>
        bool MatchesSoftware(hal::AesStmDma& aes)
        {
            const auto key = Pattern<hal::AesStmDma::keySize>(0x11);
            const auto iv = Pattern<hal::AesStmDma::ivSize>(0x22);
            const auto additionalData = Pattern<AdditionalDataSize>(0x33);
            const auto plaintext = Pattern<PayloadSize>(0x44);

            std::array<uint8_t, PayloadSize> expectedCiphertext{};
            std::array<uint8_t, hal::AesStmDma::blockSize> expectedTag{};
            mbedtls_gcm_context context;
            mbedtls_gcm_init(&context);
            bool softwareOk = __real_mbedtls_gcm_setkey(&context, MBEDTLS_CIPHER_ID_AES, key.data(), key.size() * 8) == 0 &&
                              __real_mbedtls_gcm_crypt_and_tag(&context, MBEDTLS_GCM_ENCRYPT, plaintext.size(), iv.data(), iv.size(), additionalData.data(), additionalData.size(),
                                  plaintext.data(), expectedCiphertext.data(), expectedTag.size(), expectedTag.data()) == 0;
            __real_mbedtls_gcm_free(&context);

            std::array<uint8_t, PayloadSize> ciphertext{};
            std::array<uint8_t, PayloadSize> decrypted{};
            std::array<uint8_t, hal::AesStmDma::blockSize> encryptTag{};
            std::array<uint8_t, hal::AesStmDma::blockSize> decryptTag{};

            return softwareOk &&
                   aes.TryProcessGcmBlocking(hal::AesStmDma::Direction::encrypt, infra::MakeRange(key), infra::MakeRange(iv), infra::MakeRange(additionalData), infra::MakeRange(plaintext), infra::MakeRange(ciphertext), infra::MakeRange(encryptTag)) &&
                   aes.TryProcessGcmBlocking(hal::AesStmDma::Direction::decrypt, infra::MakeRange(key), infra::MakeRange(iv), infra::MakeRange(additionalData), infra::MakeRange(ciphertext), infra::MakeRange(decrypted), infra::MakeRange(decryptTag)) &&
                   ciphertext == expectedCiphertext && encryptTag == expectedTag && decryptTag == expectedTag && decrypted == plaintext;
        }
    }

    MbedTlsGcmAccelerationAes::MbedTlsGcmAccelerationAes(hal::AesStmDma& aes)
        : aes(aes)
    {
        statistics.selfTestPassed = MatchesSoftware<5, 37>(aes) && MatchesSoftware<21, 32>(aes) && MatchesSoftware<0, 1>(aes) && MatchesSoftware<13, 0>(aes);
    }

    MbedTlsGcmAccelerationAes::~MbedTlsGcmAccelerationAes()
    {
        for (auto& entry : entries)
            Clear(entry);
    }

    void MbedTlsGcmAccelerationAes::KeySet(const mbedtls_gcm_context& context, infra::ConstByteRange key)
    {
        auto entry = Find(&context);
        if (entry == nullptr)
            entry = Find(nullptr);

        // Without a free entry the context is processed in software
        if (entry == nullptr || key.size() != hal::AesStmDma::keySize)
        {
            Forget(context);
            return;
        }

        entry->context = &context;
        std::memcpy(entry->key.data(), key.begin(), key.size());
    }

    void MbedTlsGcmAccelerationAes::Forget(const mbedtls_gcm_context& context)
    {
        if (auto entry = Find(&context); entry != nullptr)
            Clear(*entry);
    }

    bool MbedTlsGcmAccelerationAes::TryProcess(const mbedtls_gcm_context& context, hal::AesStmDma::Direction direction, infra::ConstByteRange iv, infra::ConstByteRange additionalData, infra::ConstByteRange input, infra::ByteRange output, infra::ByteRange tag)
    {
        auto entry = Find(&context);
        if (!statistics.selfTestPassed || entry == nullptr || !aes.TryProcessGcmBlocking(direction, infra::MakeRange(entry->key), iv, additionalData, input, output, tag))
            return false;

        ++statistics.hardware;
        return true;
    }

    void MbedTlsGcmAccelerationAes::CountSoftware()
    {
        ++statistics.software;
    }

    const MbedTlsGcmAccelerationAes::Statistics& MbedTlsGcmAccelerationAes::GetStatistics() const
    {
        return statistics;
    }

    MbedTlsGcmAccelerationAes::Entry* MbedTlsGcmAccelerationAes::Find(const mbedtls_gcm_context* context)
    {
        for (auto& entry : entries)
            if (entry.context == context)
                return &entry;

        return nullptr;
    }

    void MbedTlsGcmAccelerationAes::Clear(Entry& entry)
    {
        Wipe(infra::MakeRange(entry.key));
        entry.context = nullptr;
    }
}

namespace
{
    constexpr std::size_t ivSize = 12;
    constexpr std::size_t minimumTagSize = 4;

    // Mirrors the argument checks of the software implementation, which handles everything that is rejected here
    bool TryProcessInHardware(mbedtls_gcm_context* ctx, hal::AesStmDma::Direction direction, size_t length, const unsigned char* iv, size_t iv_len, const unsigned char* add, size_t add_len, const unsigned char* input, unsigned char* output, size_t tag_len, infra::ByteRange tag)
    {
        if (!services::MbedTlsGcmAccelerationAes::InstanceSet())
            return false;

        auto& acceleration = services::MbedTlsGcmAccelerationAes::Instance();
        const bool overlappingOutput = output > input && static_cast<std::size_t>(output - input) < length;

        if (iv_len == ivSize && tag_len >= minimumTagSize && tag_len <= tag.size() && !overlappingOutput &&
            acceleration.TryProcess(*ctx, direction, infra::ConstByteRange(iv, iv + iv_len), infra::ConstByteRange(add, add + add_len), infra::ConstByteRange(input, input + length), infra::ByteRange(output, output + length), tag))
            return true;

        acceleration.CountSoftware();
        return false;
    }
}

extern "C"
{
    int __wrap_mbedtls_gcm_setkey(mbedtls_gcm_context* ctx, mbedtls_cipher_id_t cipher, const unsigned char* key, unsigned int keybits)
    {
        auto result = __real_mbedtls_gcm_setkey(ctx, cipher, key, keybits);

        if (services::MbedTlsGcmAccelerationAes::InstanceSet())
        {
            auto& acceleration = services::MbedTlsGcmAccelerationAes::Instance();
            if (result == 0 && cipher == MBEDTLS_CIPHER_ID_AES)
                acceleration.KeySet(*ctx, infra::ConstByteRange(key, key + keybits / 8));
            else
                acceleration.Forget(*ctx);
        }

        return result;
    }

    void __wrap_mbedtls_gcm_free(mbedtls_gcm_context* ctx)
    {
        if (ctx != nullptr && services::MbedTlsGcmAccelerationAes::InstanceSet())
            services::MbedTlsGcmAccelerationAes::Instance().Forget(*ctx);

        __real_mbedtls_gcm_free(ctx);
    }

    int __wrap_mbedtls_gcm_crypt_and_tag(mbedtls_gcm_context* ctx, int mode, size_t length, const unsigned char* iv, size_t iv_len, const unsigned char* add, size_t add_len, const unsigned char* input, unsigned char* output, size_t tag_len, unsigned char* tag)
    {
        std::array<uint8_t, hal::AesStmDma::blockSize> fullTag;
        const auto direction = mode == MBEDTLS_GCM_ENCRYPT ? hal::AesStmDma::Direction::encrypt : hal::AesStmDma::Direction::decrypt;

        if (!TryProcessInHardware(ctx, direction, length, iv, iv_len, add, add_len, input, output, tag_len, infra::MakeRange(fullTag)))
            return __real_mbedtls_gcm_crypt_and_tag(ctx, mode, length, iv, iv_len, add, add_len, input, output, tag_len, tag);

        std::memcpy(tag, fullTag.data(), tag_len);
        return 0;
    }

    int __wrap_mbedtls_gcm_auth_decrypt(mbedtls_gcm_context* ctx, size_t length, const unsigned char* iv, size_t iv_len, const unsigned char* add, size_t add_len, const unsigned char* tag, size_t tag_len, const unsigned char* input, unsigned char* output)
    {
        std::array<uint8_t, hal::AesStmDma::blockSize> computedTag;

        if (!TryProcessInHardware(ctx, hal::AesStmDma::Direction::decrypt, length, iv, iv_len, add, add_len, input, output, tag_len, infra::MakeRange(computedTag)))
            return __real_mbedtls_gcm_auth_decrypt(ctx, length, iv, iv_len, add, add_len, tag, tag_len, input, output);

        uint8_t difference = 0;
        for (std::size_t i = 0; i != tag_len; ++i)
            difference |= tag[i] ^ computedTag[i];

        if (difference != 0)
        {
            mbedtls_platform_zeroize(output, length);
            return MBEDTLS_ERR_GCM_AUTH_FAILED;
        }

        return 0;
    }
}
