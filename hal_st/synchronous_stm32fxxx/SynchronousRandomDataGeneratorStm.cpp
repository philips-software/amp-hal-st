#include "hal_st/synchronous_stm32fxxx/SynchronousRandomDataGeneratorStm.hpp"
#include DEVICE_HEADER

#if defined(HAS_PERIPHERAL_RNG)

namespace hal
{
    SynchronousRandomDataGeneratorStm::SynchronousRandomDataGeneratorStm()
    {
        __HAL_RCC_RNG_CLK_ENABLE();

#if defined(RNG_CR_CONDRST)
        RNG->CR |= RNG_CR_IE;
#endif
        RNG->CR |= RNG_CR_RNGEN;
#if defined(RNG_CR_CONDRST)
        if ((RNG->CR & RNG_CR_CONDRST) != 0)
            Reset();
#endif
    }

    SynchronousRandomDataGeneratorStm::~SynchronousRandomDataGeneratorStm()
    {
#if defined(RNG_CR_CONDRST)
        RNG->CR &= ~RNG_CR_IE;
#endif
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
                Reset();
            }

#if defined(RNG_CR_CONDRST)
            if ((RNG->CR & RNG_CR_CONDRST) != 0)
                Reset();
#endif

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

    void SynchronousRandomDataGeneratorStm::Reset()
    {
#if defined(RNG_CR_CONDRST)
        RNG->CR &= ~RNG_CR_IE;
        RNG->CR &= ~RNG_CR_RNGEN;
#if defined(RNG_SR_BUSY)
        RNG->SR &= ~RNG_SR_BUSY;
#endif
        RNG->CR |= RNG_CR_CONDRST;
        RNG->CR &= ~RNG_CR_CONDRST;

        while ((RNG->CR & RNG_CR_CONDRST) != 0)
        {
        }
        RNG->SR &= ~RNG_SR_SEIS;
        RNG->CR |= RNG_CR_IE;
        RNG->CR |= RNG_CR_RNGEN;
#else
        static_cast<void>(RNG->DR);
        RNG->SR &= ~RNG_SR_SEIS;
        RNG->CR &= ~RNG_CR_RNGEN;
        RNG->CR |= RNG_CR_RNGEN;
        RNG->CR |= RNG_CR_IE;
#endif
    }
}
#endif
