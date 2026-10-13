#ifndef SERVICES_MBED_TLS_ECP_ACCELERATION_PKA_HPP
#define SERVICES_MBED_TLS_ECP_ACCELERATION_PKA_HPP

#include "hal_st/stm32fxxx/PkaStm.hpp"
#include "infra/util/InterfaceConnector.hpp"
#include "mbedtls/ecp.h"
#include <cstdint>

namespace services
{
    // Runs mbedtls secp256r1 scalar multiplications on the PKA, falling back to software when the PKA is busy.
    // The executable must be linked with -Wl,--wrap= for mbedtls_ecp_gen_key, mbedtls_ecp_mul and mbedtls_ecp_mul_restartable
    class MbedTlsEcpAccelerationPka
        : public infra::InterfaceConnector<MbedTlsEcpAccelerationPka>
    {
    public:
        struct Statistics
        {
            uint32_t hardware = 0;
            uint32_t software = 0;
        };

        explicit MbedTlsEcpAccelerationPka(hal::PkaStm& pka);

        bool TryMultiply(const mbedtls_ecp_group& group, mbedtls_ecp_point& result, const mbedtls_mpi& scalar, const mbedtls_ecp_point& point);
        void CountSoftware();
        const Statistics& GetStatistics() const;

    private:
        hal::PkaStm& pka;
        Statistics statistics;
    };
}

#endif
