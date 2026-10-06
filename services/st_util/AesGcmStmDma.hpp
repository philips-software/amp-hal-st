#ifndef SERVICES_AES_GCM_STM_DMA_HPP
#define SERVICES_AES_GCM_STM_DMA_HPP

#include "hal_st/stm32fxxx/AesStmDma.hpp"
#include "infra/event/ClaimableResource.hpp"
#include "services/util/SesameCrypto.hpp"
#include <array>

namespace services
{
    class AesGcmStmDma
        : public AesGcmEncryption
    {
    public:
        explicit AesGcmStmDma(hal::AesStmDma& engine);
        ~AesGcmStmDma();

        void SetEncryptKey(infra::ConstByteRange key) override;
        void SetDecryptKey(infra::ConstByteRange key) override;
        void Process(infra::ConstByteRange iv, infra::ByteRange data, infra::ByteRange mac, const infra::Function<void()>& onDone) override;

    private:
        void SetKey(infra::ConstByteRange newKey, hal::AesStmDma::Direction newDirection);
        void Processed();

    private:
        hal::AesStmDma& engine;
        infra::ClaimableResource::Claimer claimer;
        hal::AesStmDma::Direction direction = hal::AesStmDma::Direction::encrypt;
        std::array<uint8_t, hal::AesStmDma::keySize> key{};
        std::array<uint8_t, hal::AesStmDma::ivSize> iv{};
        infra::ByteRange data;
        infra::ByteRange mac;
        infra::Function<void()> onDone;
    };
}

#endif
