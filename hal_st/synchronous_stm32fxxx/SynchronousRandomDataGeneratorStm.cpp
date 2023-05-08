#include "hal_st/synchronous_stm32fxxx/SynchronousRandomDataGeneratorStm.hpp"
#include DEVICE_HEADER

#if defined(HAS_PERIPHERAL_RNG)

namespace hal
{
    SynchronousRandomDataGeneratorStm::SynchronousRandomDataGeneratorStm()
    {
        __HAL_RCC_RNG_CLK_ENABLE();
        RNG->CR |= RNG_CR_RNGEN;
    }

    SynchronousRandomDataGeneratorStm::~SynchronousRandomDataGeneratorStm()
    {
        RNG->CR &= ~RNG_CR_RNGEN;
        __HAL_RCC_RNG_CLK_DISABLE();
    }

    void SynchronousRandomDataGeneratorStm::GenerateRandomData(infra::ByteRange result)
    {
        while (!result.empty())
        {
            if ((RNG->SR & RNG_SR_CEIS) != 0)
                std::abort(); // RNG clock must be set to at least HCLK / 16

            if ((RNG->SR & RNG_SR_SEIS) != 0)
            {
                // Seed error, reset RNG and try again
                static_cast<void>(RNG->DR);
                RNG->SR &= ~RNG_SR_SEIS;
                RNG->CR &= ~RNG_CR_RNGEN;
                RNG->CR |= RNG_CR_RNGEN;
                RNG->CR |= RNG_CR_IE;
            }

            if ((RNG->SR & RNG_SR_DRDY) != 0)
            {
                uint32_t data = RNG->DR;
                infra::ByteRange dataRange = infra::MakeByteRange(data);
                dataRange.shrink_from_back_to(result.size());

                std::copy(dataRange.begin(), dataRange.end(), result.begin());
                result.pop_front(dataRange.size());
            }
        }
    }
}

#endif
