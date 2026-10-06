#ifndef SERVICES_SESAME_CRYPTO_PKA_HPP
#define SERVICES_SESAME_CRYPTO_PKA_HPP

#include "hal/synchronous_interfaces/SynchronousRandomDataGenerator.hpp"
#include "hal_st/stm32fxxx/PkaStm.hpp"
#include "infra/event/ClaimableResource.hpp"
#include "services/util/SesameCrypto.hpp"
#include <array>

namespace services
{
    namespace detail
    {
        // Runs one (possibly multi-step) PKA operation at a time; a new Run while one is pending replaces it
        class PkaOperationRunner
        {
        public:
            explicit PkaOperationRunner(hal::PkaStm& pka);

            void Run(const infra::Function<void()>& operation);
            // Call between steps; returns false when the operation was superseded and has been restarted
            bool Continue();
            // Call after the last step; returns true when the result must be reported
            bool Finish();

        private:
            void Claim();

        private:
            infra::ClaimableResource::Claimer claimer;
            infra::Function<void()> operation;
            bool running = false;
            bool superseded = false;
        };
    }

    class EcSecP256r1DiffieHellmanPka
        : public EcSecP256r1DiffieHellman
    {
    public:
        EcSecP256r1DiffieHellmanPka(hal::PkaStm& pka, hal::SynchronousRandomDataGenerator& randomDataGenerator);
        ~EcSecP256r1DiffieHellmanPka();

        void GenerateKeyPair(const infra::Function<void(const std::array<uint8_t, 65>& publicKey)>& onDone) override;
        void SharedSecret(infra::ConstByteRange otherPublicKey, const infra::Function<void(const std::array<uint8_t, 32>& sharedSecret)>& onDone) override;

    private:
        hal::PkaStm& pka;
        hal::SynchronousRandomDataGenerator& randomDataGenerator;
        detail::PkaOperationRunner runner;

        std::array<uint8_t, 32> privateKey{};
        std::array<uint8_t, 32> x{};
        std::array<uint8_t, 32> y{};
        std::array<uint8_t, 32> resultX{};
        std::array<uint8_t, 32> resultY{};
        std::array<uint8_t, 65> publicKey{};
        infra::Function<void(const std::array<uint8_t, 65>& publicKey)> onPublicKey;
        infra::Function<void(const std::array<uint8_t, 32>& sharedSecret)> onSharedSecret;
    };

    class EcSecP256r1DsaSignerPka
        : public EcSecP256r1DsaSigner
    {
    public:
        // dsaCertificatePrivateKey is DER encoded, as accepted by SynchronousEcSecP256r1DsaSignerMbedTls
        EcSecP256r1DsaSignerPka(hal::PkaStm& pka, infra::ConstByteRange dsaCertificatePrivateKey, hal::SynchronousRandomDataGenerator& randomDataGenerator);
        ~EcSecP256r1DsaSignerPka();

        void Sign(infra::ConstByteRange data, const infra::Function<void(const std::array<uint8_t, 32>& r, const std::array<uint8_t, 32>& s)>& onDone) override;

    private:
        void SignWithNewNonce();

    private:
        hal::PkaStm& pka;
        hal::SynchronousRandomDataGenerator& randomDataGenerator;
        detail::PkaOperationRunner runner;

        std::array<uint8_t, 32> privateKey{};
        std::array<uint8_t, 32> nonce{};
        std::array<uint8_t, 32> hash{};
        std::array<uint8_t, 32> r{};
        std::array<uint8_t, 32> s{};
        infra::Function<void(const std::array<uint8_t, 32>& r, const std::array<uint8_t, 32>& s)> onSigned;
    };

    class EcSecP256r1DsaVerifierPka
        : public EcSecP256r1DsaVerifier
    {
    public:
        explicit EcSecP256r1DsaVerifierPka(hal::PkaStm& pka);

        void VerifyCertificate(infra::ConstByteRange dsaCertificate, infra::ConstByteRange rootCaCertificate, const infra::Function<void(bool valid)>& onDone) override;
        void Verify(infra::ConstByteRange data, infra::ConstByteRange r, infra::ConstByteRange s, const infra::Function<void(bool valid)>& onDone) override;

    private:
        void VerifySignature(const std::array<uint8_t, 32>& keyX, const std::array<uint8_t, 32>& keyY, bool certificateVerification);

    private:
        hal::PkaStm& pka;
        detail::PkaOperationRunner runner;

        bool certificateValid = false;
        bool inputValid = false;
        std::array<uint8_t, 32> rootKeyX{};
        std::array<uint8_t, 32> rootKeyY{};
        std::array<uint8_t, 32> certificateKeyX{};
        std::array<uint8_t, 32> certificateKeyY{};
        std::array<uint8_t, 32> hash{};
        std::array<uint8_t, 32> r{};
        std::array<uint8_t, 32> s{};
        infra::Function<void(bool valid)> onVerified;
    };
}

#endif
