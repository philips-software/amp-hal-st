#include "services/st_util/MbedTlsEcpAccelerationPka.hpp"
#include "services/st_util/Secp256r1Pka.hpp"
#include <array>

namespace services
{
    namespace
    {
        constexpr std::size_t coordinateSize = 32;

        void Wipe(infra::ByteRange range)
        {
            volatile uint8_t* bytes = range.begin();
            for (std::size_t i = 0; i != range.size(); ++i)
                bytes[i] = 0;
        }

        bool IsAffine(const mbedtls_ecp_point& point)
        {
            return mbedtls_mpi_cmp_int(&point.MBEDTLS_PRIVATE(Z), 1) == 0;
        }
    }

    MbedTlsEcpAccelerationPka::MbedTlsEcpAccelerationPka(hal::PkaStm& pka)
        : pka(pka)
    {}

    bool MbedTlsEcpAccelerationPka::TryMultiply(const mbedtls_ecp_group& group, mbedtls_ecp_point& result, const mbedtls_mpi& scalar, const mbedtls_ecp_point& point)
    {
        if (group.id != MBEDTLS_ECP_DP_SECP256R1 || !IsAffine(point))
            return false;

        std::array<uint8_t, coordinateSize> scalarBytes{};
        std::array<uint8_t, coordinateSize> x{};
        std::array<uint8_t, coordinateSize> y{};
        std::array<uint8_t, coordinateSize> resultX{};
        std::array<uint8_t, coordinateSize> resultY{};

        bool success = mbedtls_mpi_write_binary(&scalar, scalarBytes.data(), scalarBytes.size()) == 0 &&
                       mbedtls_mpi_write_binary(&point.MBEDTLS_PRIVATE(X), x.data(), x.size()) == 0 &&
                       mbedtls_mpi_write_binary(&point.MBEDTLS_PRIVATE(Y), y.data(), y.size()) == 0 &&
                       pka.TryScalarMultiplicationBlocking(Secp256r1Pka(), infra::MakeRange(scalarBytes), infra::MakeRange(x), infra::MakeRange(y), infra::MakeRange(resultX), infra::MakeRange(resultY));

        Wipe(infra::MakeRange(scalarBytes));

        mbedtls_ecp_point computed;
        mbedtls_ecp_point_init(&computed);

        // The result is checked to be on the curve to detect faults before mbedtls uses it
        success = success &&
                  mbedtls_mpi_read_binary(&computed.MBEDTLS_PRIVATE(X), resultX.data(), resultX.size()) == 0 &&
                  mbedtls_mpi_read_binary(&computed.MBEDTLS_PRIVATE(Y), resultY.data(), resultY.size()) == 0 &&
                  mbedtls_mpi_lset(&computed.MBEDTLS_PRIVATE(Z), 1) == 0 &&
                  mbedtls_ecp_check_pubkey(&group, &computed) == 0 &&
                  mbedtls_ecp_copy(&result, &computed) == 0;

        mbedtls_ecp_point_free(&computed);
        Wipe(infra::MakeRange(resultX));
        Wipe(infra::MakeRange(resultY));

        if (success)
            ++statistics.hardware;

        return success;
    }

    void MbedTlsEcpAccelerationPka::CountSoftware()
    {
        ++statistics.software;
    }

    const MbedTlsEcpAccelerationPka::Statistics& MbedTlsEcpAccelerationPka::GetStatistics() const
    {
        return statistics;
    }
}

namespace
{
    using RandomFunction = int (*)(void*, unsigned char*, size_t);

    // mbedtls_ecp_mul_restartable() validates its inputs before multiplying; the same is done here so that invalid input
    // is rejected by the software implementation with its regular error codes
    bool TryMultiplyInHardware(mbedtls_ecp_group* grp, mbedtls_ecp_point* R, const mbedtls_mpi* m, const mbedtls_ecp_point* P)
    {
        if (!services::MbedTlsEcpAccelerationPka::InstanceSet())
            return false;

        auto& acceleration = services::MbedTlsEcpAccelerationPka::Instance();
        if (mbedtls_ecp_check_privkey(grp, m) == 0 && mbedtls_ecp_check_pubkey(grp, P) == 0 && acceleration.TryMultiply(*grp, *R, *m, *P))
            return true;

        acceleration.CountSoftware();
        return false;
    }
}

extern "C"
{
    int __real_mbedtls_ecp_gen_key(mbedtls_ecp_group_id grp_id, mbedtls_ecp_keypair* key, RandomFunction f_rng, void* p_rng);
    int __real_mbedtls_ecp_mul(mbedtls_ecp_group* grp, mbedtls_ecp_point* R, const mbedtls_mpi* m, const mbedtls_ecp_point* P, RandomFunction f_rng, void* p_rng);
    int __real_mbedtls_ecp_mul_restartable(mbedtls_ecp_group* grp, mbedtls_ecp_point* R, const mbedtls_mpi* m, const mbedtls_ecp_point* P, RandomFunction f_rng, void* p_rng, mbedtls_ecp_restart_ctx* rs_ctx);

    int __wrap_mbedtls_ecp_gen_key(mbedtls_ecp_group_id grp_id, mbedtls_ecp_keypair* key, RandomFunction f_rng, void* p_rng)
    {
        if (grp_id != MBEDTLS_ECP_DP_SECP256R1 || !services::MbedTlsEcpAccelerationPka::InstanceSet())
            return __real_mbedtls_ecp_gen_key(grp_id, key, f_rng, p_rng);

        auto& group = key->MBEDTLS_PRIVATE(grp);
        auto& privateKey = key->MBEDTLS_PRIVATE(d);
        auto& publicKey = key->MBEDTLS_PRIVATE(Q);

        if (int result = mbedtls_ecp_group_load(&group, grp_id); result != 0)
            return result;

        if (int result = mbedtls_ecp_gen_privkey(&group, &privateKey, f_rng, p_rng); result != 0)
            return result;

        if (TryMultiplyInHardware(&group, &publicKey, &privateKey, &group.G))
            return 0;

        return __real_mbedtls_ecp_mul(&group, &publicKey, &privateKey, &group.G, f_rng, p_rng);
    }

    int __wrap_mbedtls_ecp_mul(mbedtls_ecp_group* grp, mbedtls_ecp_point* R, const mbedtls_mpi* m, const mbedtls_ecp_point* P, RandomFunction f_rng, void* p_rng)
    {
        if (TryMultiplyInHardware(grp, R, m, P))
            return 0;

        return __real_mbedtls_ecp_mul(grp, R, m, P, f_rng, p_rng);
    }

    int __wrap_mbedtls_ecp_mul_restartable(mbedtls_ecp_group* grp, mbedtls_ecp_point* R, const mbedtls_mpi* m, const mbedtls_ecp_point* P, RandomFunction f_rng, void* p_rng, mbedtls_ecp_restart_ctx* rs_ctx)
    {
        if (rs_ctx == nullptr && TryMultiplyInHardware(grp, R, m, P))
            return 0;

        return __real_mbedtls_ecp_mul_restartable(grp, R, m, P, f_rng, p_rng, rs_ctx);
    }
}
