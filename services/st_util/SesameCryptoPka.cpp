#include "services/st_util/SesameCryptoPka.hpp"
#include "infra/util/ReallyAssert.hpp"
#include "mbedtls/asn1.h"
#include "mbedtls/md.h"
#include "mbedtls/pk.h"
#include "mbedtls/sha256.h"
#include "mbedtls/x509_crt.h"
#include "services/util/MbedTlsRandomDataGeneratorWrapper.hpp"
#include <algorithm>
#include <utility>

namespace services
{
    namespace
    {
        // clang-format off
        constexpr std::array<uint8_t, 32> modulus = {
            0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
        };
        constexpr std::array<uint8_t, 32> absoluteA = {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03
        };
        constexpr std::array<uint8_t, 32> coefficientB = {
            0x5A, 0xC6, 0x35, 0xD8, 0xAA, 0x3A, 0x93, 0xE7, 0xB3, 0xEB, 0xBD, 0x55, 0x76, 0x98, 0x86, 0xBC,
            0x65, 0x1D, 0x06, 0xB0, 0xCC, 0x53, 0xB0, 0xF6, 0x3B, 0xCE, 0x3C, 0x3E, 0x27, 0xD2, 0x60, 0x4B
        };
        constexpr std::array<uint8_t, 32> generatorX = {
            0x6B, 0x17, 0xD1, 0xF2, 0xE1, 0x2C, 0x42, 0x47, 0xF8, 0xBC, 0xE6, 0xE5, 0x63, 0xA4, 0x40, 0xF2,
            0x77, 0x03, 0x7D, 0x81, 0x2D, 0xEB, 0x33, 0xA0, 0xF4, 0xA1, 0x39, 0x45, 0xD8, 0x98, 0xC2, 0x96
        };
        constexpr std::array<uint8_t, 32> generatorY = {
            0x4F, 0xE3, 0x42, 0xE2, 0xFE, 0x1A, 0x7F, 0x9B, 0x8E, 0xE7, 0xEB, 0x4A, 0x7C, 0x0F, 0x9E, 0x16,
            0x2B, 0xCE, 0x33, 0x57, 0x6B, 0x31, 0x5E, 0xCE, 0xCB, 0xB6, 0x40, 0x68, 0x37, 0xBF, 0x51, 0xF5
        };
        constexpr std::array<uint8_t, 32> order = {
            0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xBC, 0xE6, 0xFA, 0xAD, 0xA7, 0x17, 0x9E, 0x84, 0xF3, 0xB9, 0xCA, 0xC2, 0xFC, 0x63, 0x25, 0x51
        };
        constexpr std::array<uint8_t, 32> montgomeryParameter = {
            0xFF, 0xFF, 0xFF, 0xFC, 0xFF, 0xFF, 0xFF, 0xFC, 0xFF, 0xFF, 0xFF, 0xFB, 0xFF, 0xFF, 0xFF, 0xF9,
            0xFF, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x02
        };
        // clang-format on

        const hal::PkaStm::Curve secp256r1{ infra::MakeRange(modulus), infra::MakeRange(absoluteA), false, infra::MakeRange(coefficientB),
            infra::MakeRange(generatorX), infra::MakeRange(generatorY), infra::MakeRange(order), infra::MakeRange(montgomeryParameter) };

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
                   !LessThan(infra::MakeRange(scalar), infra::MakeRange(order)));
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
                pka.ScalarMultiplication(secp256r1, infra::MakeRange(privateKey), secp256r1.gX, secp256r1.gY, infra::MakeRange(resultX), infra::MakeRange(resultY), [this](bool success)
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
        really_assert(LessThan(infra::MakeRange(x), infra::MakeRange(modulus)) && LessThan(infra::MakeRange(y), infra::MakeRange(modulus)));

        onPublicKey = nullptr;
        onSharedSecret = onDone;

        runner.Run([this]()
            {
                pka.CheckPointOnCurve(secp256r1, infra::MakeRange(x), infra::MakeRange(y), [this](bool onCurve)
                    {
                        if (!runner.Continue())
                            return;

                        really_assert(onCurve);
                        pka.ScalarMultiplication(secp256r1, infra::MakeRange(privateKey), infra::MakeRange(x), infra::MakeRange(y), infra::MakeRange(resultX), infra::MakeRange(resultY), [this](bool success)
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
        pka.EcdsaSign(secp256r1, infra::MakeRange(privateKey), infra::MakeRange(nonce), infra::MakeRange(hash), infra::MakeRange(r), infra::MakeRange(s), [this](bool success)
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

        pka.EcdsaVerify(secp256r1, infra::MakeRange(keyX), infra::MakeRange(keyY), infra::MakeRange(r), infra::MakeRange(s), infra::MakeRange(hash), [this, certificateVerification](bool valid)
            {
                if (!runner.Finish())
                    return;

                if (certificateVerification)
                    certificateValid = valid;

                std::exchange(onVerified, nullptr)(valid);
            });
    }
}
