#include "services/st_util/SesameCryptoPka.hpp"
#include "infra/util/ReallyAssert.hpp"
#include "mbedtls/asn1.h"
#include "mbedtls/md.h"
#include "mbedtls/pk.h"
#include "mbedtls/sha256.h"
#include "mbedtls/x509_crt.h"
#include "services/st_util/Secp256r1Pka.hpp"
#include "services/util/MbedTlsRandomDataGeneratorWrapper.hpp"
#include <algorithm>
#include <utility>

namespace services
{
    namespace
    {
        constexpr uint8_t uncompressedPoint = 0x04;

        void Wipe(infra::ByteRange range)
        {
            volatile uint8_t* bytes = range.begin();
            for (std::size_t i = 0; i != range.size(); ++i)
                bytes[i] = 0;
        }

        bool LessThan(infra::ConstByteRange a, infra::ConstByteRange b)
        {
            return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
        }

        void GenerateScalar(hal::SynchronousRandomDataGenerator& randomDataGenerator, std::array<uint8_t, 32>& scalar)
        {
            do
                randomDataGenerator.GenerateRandomData(infra::MakeRange(scalar));
            while (std::all_of(scalar.begin(), scalar.end(), [](uint8_t byte)
                       {
                           return byte == 0;
                       }) ||
                   !LessThan(infra::MakeRange(scalar), Secp256r1Pka().n));
        }

        void Sha256(infra::ConstByteRange data, std::array<uint8_t, 32>& hash)
        {
            really_assert(mbedtls_sha256(data.begin(), data.size(), hash.data(), 0) == 0);
        }

        void ReadPublicKey(const mbedtls_pk_context& pk, std::array<uint8_t, 32>& x, std::array<uint8_t, 32>& y)
        {
            really_assert(mbedtls_pk_get_type(&pk) == MBEDTLS_PK_ECKEY);

            mbedtls_ecp_group group;
            mbedtls_mpi unusedPrivateKey;
            mbedtls_ecp_point publicKey;
            mbedtls_ecp_group_init(&group);
            mbedtls_mpi_init(&unusedPrivateKey);
            mbedtls_ecp_point_init(&publicKey);

            std::array<uint8_t, 65> encoded;
            std::size_t size = 0;
            really_assert(mbedtls_ecp_export(mbedtls_pk_ec(pk), &group, &unusedPrivateKey, &publicKey) == 0);
            really_assert(mbedtls_ecp_point_write_binary(&group, &publicKey, MBEDTLS_ECP_PF_UNCOMPRESSED, &size, encoded.data(), encoded.size()) == 0 && size == encoded.size());

            std::copy_n(encoded.begin() + 1, x.size(), x.begin());
            std::copy_n(encoded.begin() + 1 + x.size(), y.size(), y.begin());

            mbedtls_ecp_point_free(&publicKey);
            mbedtls_mpi_free(&unusedPrivateKey);
            mbedtls_ecp_group_free(&group);
        }

        bool DecodeSignature(infra::ConstByteRange der, std::array<uint8_t, 32>& r, std::array<uint8_t, 32>& s)
        {
            auto position = const_cast<unsigned char*>(der.begin());
            const auto end = der.end();
            std::size_t length = 0;

            if (mbedtls_asn1_get_tag(&position, end, &length, MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE) != 0 || position + length != end)
                return false;

            mbedtls_mpi mpiR;
            mbedtls_mpi mpiS;
            mbedtls_mpi_init(&mpiR);
            mbedtls_mpi_init(&mpiS);

            const bool valid = mbedtls_asn1_get_mpi(&position, end, &mpiR) == 0 && mbedtls_asn1_get_mpi(&position, end, &mpiS) == 0 && position == end &&
                               mbedtls_mpi_write_binary(&mpiR, r.data(), r.size()) == 0 && mbedtls_mpi_write_binary(&mpiS, s.data(), s.size()) == 0;

            mbedtls_mpi_free(&mpiS);
            mbedtls_mpi_free(&mpiR);
            return valid;
        }
    }

    namespace detail
    {
        PkaOperationRunner::PkaOperationRunner(hal::PkaStm& pka)
            : claimer(pka.Resource())
        {}

        void PkaOperationRunner::Run(const infra::Function<void()>& operation)
        {
            this->operation = operation;

            if (running)
                superseded = true;
            else if (!claimer.IsQueued())
                Claim();
        }

        bool PkaOperationRunner::Continue()
        {
            if (!superseded)
                return true;

            superseded = false;
            running = false;
            claimer.Release();
            Claim();
            return false;
        }

        bool PkaOperationRunner::Finish()
        {
            if (!Continue())
                return false;

            running = false;
            claimer.Release();
            return true;
        }

        void PkaOperationRunner::Claim()
        {
            claimer.Claim([this]()
                {
                    running = true;
                    operation();
                });
        }
    }

    EcSecP256r1DiffieHellmanPka::EcSecP256r1DiffieHellmanPka(hal::PkaStm& pka, hal::SynchronousRandomDataGenerator& randomDataGenerator)
        : pka(pka)
        , randomDataGenerator(randomDataGenerator)
        , runner(pka)
    {}

    EcSecP256r1DiffieHellmanPka::~EcSecP256r1DiffieHellmanPka()
    {
        Wipe(infra::MakeRange(privateKey));
        Wipe(infra::MakeRange(resultX));
    }

    void EcSecP256r1DiffieHellmanPka::GenerateKeyPair(const infra::Function<void(const std::array<uint8_t, 65>& publicKey)>& onDone)
    {
        onSharedSecret = nullptr;
        onPublicKey = onDone;

        runner.Run([this]()
            {
                GenerateScalar(randomDataGenerator, privateKey);
                pka.ScalarMultiplication(Secp256r1Pka(), infra::MakeRange(privateKey), Secp256r1Pka().gX, Secp256r1Pka().gY, infra::MakeRange(resultX), infra::MakeRange(resultY), [this](bool success)
                    {
                        really_assert(success);
                        if (!runner.Finish())
                            return;

                        publicKey[0] = uncompressedPoint;
                        std::copy(resultX.begin(), resultX.end(), publicKey.begin() + 1);
                        std::copy(resultY.begin(), resultY.end(), publicKey.begin() + 1 + resultX.size());
                        std::exchange(onPublicKey, nullptr)(publicKey);
                    });
            });
    }

    void EcSecP256r1DiffieHellmanPka::SharedSecret(infra::ConstByteRange otherPublicKey, const infra::Function<void(const std::array<uint8_t, 32>& sharedSecret)>& onDone)
    {
        really_assert(otherPublicKey.size() == publicKey.size() && otherPublicKey[0] == uncompressedPoint);
        std::copy_n(otherPublicKey.begin() + 1, x.size(), x.begin());
        std::copy_n(otherPublicKey.begin() + 1 + x.size(), y.size(), y.begin());
        really_assert(LessThan(infra::MakeRange(x), Secp256r1Pka().p) && LessThan(infra::MakeRange(y), Secp256r1Pka().p));

        onPublicKey = nullptr;
        onSharedSecret = onDone;

        runner.Run([this]()
            {
                pka.CheckPointOnCurve(Secp256r1Pka(), infra::MakeRange(x), infra::MakeRange(y), [this](bool onCurve)
                    {
                        if (!runner.Continue())
                            return;

                        really_assert(onCurve);
                        pka.ScalarMultiplication(Secp256r1Pka(), infra::MakeRange(privateKey), infra::MakeRange(x), infra::MakeRange(y), infra::MakeRange(resultX), infra::MakeRange(resultY), [this](bool success)
                            {
                                really_assert(success);
                                if (runner.Finish())
                                    std::exchange(onSharedSecret, nullptr)(resultX);
                            });
                    });
            });
    }

    EcSecP256r1DsaSignerPka::EcSecP256r1DsaSignerPka(hal::PkaStm& pka, infra::ConstByteRange dsaCertificatePrivateKey, hal::SynchronousRandomDataGenerator& randomDataGenerator)
        : pka(pka)
        , randomDataGenerator(randomDataGenerator)
        , runner(pka)
    {
        mbedtls_pk_context context;
        mbedtls_pk_init(&context);
        really_assert(mbedtls_pk_parse_key(&context, dsaCertificatePrivateKey.begin(), dsaCertificatePrivateKey.size(), nullptr, 0, &MbedTlsRandomDataGeneratorWrapper, &randomDataGenerator) == 0);
        really_assert(mbedtls_pk_get_type(&context) == MBEDTLS_PK_ECKEY);

        mbedtls_ecp_group group;
        mbedtls_mpi key;
        mbedtls_ecp_point unusedPublicKey;
        mbedtls_ecp_group_init(&group);
        mbedtls_mpi_init(&key);
        mbedtls_ecp_point_init(&unusedPublicKey);

        really_assert(mbedtls_ecp_export(mbedtls_pk_ec(context), &group, &key, &unusedPublicKey) == 0);
        really_assert(mbedtls_mpi_write_binary(&key, privateKey.data(), privateKey.size()) == 0);

        mbedtls_ecp_point_free(&unusedPublicKey);
        mbedtls_mpi_free(&key);
        mbedtls_ecp_group_free(&group);
        mbedtls_pk_free(&context);
    }

    EcSecP256r1DsaSignerPka::~EcSecP256r1DsaSignerPka()
    {
        Wipe(infra::MakeRange(privateKey));
        Wipe(infra::MakeRange(nonce));
    }

    void EcSecP256r1DsaSignerPka::Sign(infra::ConstByteRange data, const infra::Function<void(const std::array<uint8_t, 32>& r, const std::array<uint8_t, 32>& s)>& onDone)
    {
        Sha256(data, hash);
        onSigned = onDone;

        runner.Run([this]()
            {
                SignWithNewNonce();
            });
    }

    void EcSecP256r1DsaSignerPka::SignWithNewNonce()
    {
        GenerateScalar(randomDataGenerator, nonce);
        pka.EcdsaSign(Secp256r1Pka(), infra::MakeRange(privateKey), infra::MakeRange(nonce), infra::MakeRange(hash), infra::MakeRange(r), infra::MakeRange(s), [this](bool success)
            {
                Wipe(infra::MakeRange(nonce));

                if (!success)
                {
                    if (runner.Continue())
                        SignWithNewNonce();
                    return;
                }

                if (runner.Finish())
                    std::exchange(onSigned, nullptr)(r, s);
            });
    }

    EcSecP256r1DsaVerifierPka::EcSecP256r1DsaVerifierPka(hal::PkaStm& pka)
        : pka(pka)
        , runner(pka)
    {}

    void EcSecP256r1DsaVerifierPka::VerifyCertificate(infra::ConstByteRange dsaCertificate, infra::ConstByteRange rootCaCertificate, const infra::Function<void(bool valid)>& onDone)
    {
        mbedtls_x509_crt root;
        mbedtls_x509_crt certificate;
        mbedtls_x509_crt_init(&root);
        mbedtls_x509_crt_init(&certificate);

        really_assert(mbedtls_x509_crt_parse(&root, rootCaCertificate.begin(), rootCaCertificate.size()) == 0);
        really_assert(mbedtls_x509_crt_parse(&certificate, dsaCertificate.begin(), dsaCertificate.size()) == 0);

        ReadPublicKey(root.pk, rootKeyX, rootKeyY);
        ReadPublicKey(certificate.pk, certificateKeyX, certificateKeyY);
        really_assert(mbedtls_md(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), certificate.tbs.p, certificate.tbs.len, hash.data()) == 0);
        inputValid = DecodeSignature(infra::ConstByteRange(certificate.MBEDTLS_PRIVATE(sig).p, certificate.MBEDTLS_PRIVATE(sig).p + certificate.MBEDTLS_PRIVATE(sig).len), r, s);

        mbedtls_x509_crt_free(&certificate);
        mbedtls_x509_crt_free(&root);

        certificateValid = false;
        onVerified = onDone;
        runner.Run([this]()
            {
                VerifySignature(rootKeyX, rootKeyY, true);
            });
    }

    void EcSecP256r1DsaVerifierPka::Verify(infra::ConstByteRange data, infra::ConstByteRange r, infra::ConstByteRange s, const infra::Function<void(bool valid)>& onDone)
    {
        inputValid = certificateValid && r.size() == this->r.size() && s.size() == this->s.size();
        if (inputValid)
        {
            Sha256(data, hash);
            infra::Copy(r, infra::MakeRange(this->r));
            infra::Copy(s, infra::MakeRange(this->s));
        }

        onVerified = onDone;
        runner.Run([this]()
            {
                VerifySignature(certificateKeyX, certificateKeyY, false);
            });
    }

    void EcSecP256r1DsaVerifierPka::VerifySignature(const std::array<uint8_t, 32>& keyX, const std::array<uint8_t, 32>& keyY, bool certificateVerification)
    {
        if (!inputValid)
        {
            if (runner.Finish())
                std::exchange(onVerified, nullptr)(false);
            return;
        }

        pka.EcdsaVerify(Secp256r1Pka(), infra::MakeRange(keyX), infra::MakeRange(keyY), infra::MakeRange(r), infra::MakeRange(s), infra::MakeRange(hash), [this, certificateVerification](bool valid)
            {
                if (!runner.Finish())
                    return;

                if (certificateVerification)
                    certificateValid = valid;

                std::exchange(onVerified, nullptr)(valid);
            });
    }
}
