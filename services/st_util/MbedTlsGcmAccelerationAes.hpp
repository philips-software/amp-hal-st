#ifndef SERVICES_MBED_TLS_GCM_ACCELERATION_AES_HPP
#define SERVICES_MBED_TLS_GCM_ACCELERATION_AES_HPP

#include "hal_st/stm32fxxx/AesStmDma.hpp"
#include "infra/util/InterfaceConnector.hpp"
#include "mbedtls/gcm.h"
#include <array>
#include <cstdint>

namespace services
{
    // Runs mbedtls AES-128-GCM one-shot operations on the AES peripheral, falling back to software when the peripheral is busy.
    // The executable must be linked with -Wl,--wrap= for mbedtls_gcm_setkey, mbedtls_gcm_free, mbedtls_gcm_crypt_and_tag and mbedtls_gcm_auth_decrypt
    class MbedTlsGcmAccelerationAes
        : public infra::InterfaceConnector<MbedTlsGcmAccelerationAes>
    {
    public:
        static constexpr std::size_t maxContexts = 8;

        struct Statistics
        {
            bool selfTestPassed = false;
            uint32_t hardware = 0;
            uint32_t software = 0;
        };

        // Compares the peripheral with the software implementation and keeps everything in software when they differ
        explicit MbedTlsGcmAccelerationAes(hal::AesStmDma& aes);
        ~MbedTlsGcmAccelerationAes();

        void KeySet(const mbedtls_gcm_context& context, infra::ConstByteRange key);
        void Forget(const mbedtls_gcm_context& context);
        bool TryProcess(const mbedtls_gcm_context& context, hal::AesStmDma::Direction direction, infra::ConstByteRange iv, infra::ConstByteRange additionalData, infra::ConstByteRange input, infra::ByteRange output, infra::ByteRange tag);
        void CountSoftware();
        const Statistics& GetStatistics() const;

    private:
        struct Entry
        {
            const mbedtls_gcm_context* context = nullptr;
            std::array<uint8_t, hal::AesStmDma::keySize> key{};
        };

        Entry* Find(const mbedtls_gcm_context* context);
        void Clear(Entry& entry);

    private:
        hal::AesStmDma& aes;
        std::array<Entry, maxContexts> entries;
        Statistics statistics;
    };
}

#endif
