#include "hal_st/middlewares/ble_middleware/platform/BlePlatform.hpp"

extern "C"
{
#include "bleplat.h"

    void BLEPLAT_Init()
    {
        really_assert(hal::BlePlatform::InstanceSet());
    }

    int BLEPLAT_NvmAdd(uint8_t type, const uint8_t* data, uint16_t size, const uint8_t* extra_data, uint16_t extra_size)
    {
        really_assert(hal::BlePlatform::InstanceSet());

        return BLEPLAT_OK;
    }

    int BLEPLAT_NvmGet(uint8_t mode, uint8_t type, uint16_t offset, uint8_t* data, uint16_t size)
    {
        really_assert(hal::BlePlatform::InstanceSet());

        return BLEPLAT_OK;
    }

    int BLEPLAT_NvmCompare(uint16_t offset, const uint8_t* data, uint16_t size)
    {
        really_assert(hal::BlePlatform::InstanceSet());

        return BLEPLAT_OK;
    }

    void BLEPLAT_NvmDiscard(uint8_t mode)
    {
        really_assert(hal::BlePlatform::InstanceSet());
    }

    int BLEPLAT_PkaStartP256Key(const uint32_t* local_private_key)
    {
        really_assert(hal::BlePlatform::InstanceSet());

        return BLEPLAT_OK;
    }

    void BLEPLAT_PkaReadP256Key(uint32_t* local_public_key)
    {
        really_assert(hal::BlePlatform::InstanceSet());
    }

    int BLEPLAT_PkaStartDhKey(const uint32_t* local_private_key, const uint32_t* remote_public_key)
    {
        really_assert(hal::BlePlatform::InstanceSet());

        return BLEPLAT_OK;
    }

    int BLEPLAT_PkaReadDhKey(uint32_t* dh_key)
    {
        really_assert(hal::BlePlatform::InstanceSet());

        return BLEPLAT_OK;
    }

    void BLEPLAT_AesEcbEncrypt(const uint8_t* key, const uint8_t* input, uint8_t* output)
    {
        really_assert(hal::BlePlatform::InstanceSet());
    }

    void BLEPLAT_AesCmacSetKey(const uint8_t* key)
    {
        really_assert(hal::BlePlatform::InstanceSet());
    }

    void BLEPLAT_AesCmacCompute(const uint8_t* input, uint32_t input_length, uint8_t* output_tag)
    {
        really_assert(hal::BlePlatform::InstanceSet());
    }

    void BLEPLAT_RngGet(uint8_t n, uint32_t* val)
    {
        really_assert(hal::BlePlatform::InstanceSet());
    }

    uint8_t BLEPLAT_TimerStart(uint8_t id, uint32_t timeout)
    {
        really_assert(hal::BlePlatform::InstanceSet());

        return 0;
    }

    void BLEPLAT_TimerStop(uint8_t id)
    {
        really_assert(hal::BlePlatform::InstanceSet());
    }
}
