#include "hal_st/stm32fxxx/RandomDataGeneratorStm.hpp"
#include "infra/event/EventDispatcher.hpp"

#if defined(HAS_PERIPHERAL_RNG)

namespace hal
{
    RandomDataGeneratorStm::RandomDataGeneratorStm()
    {
#if defined(STM32F7)
        Register(RNG_IRQn);
#else
        Register(HASH_RNG_IRQn);
#endif
        __HAL_RCC_RNG_CLK_ENABLE();
        RNG->CR |= RNG_CR_RNGEN;
    }

    RandomDataGeneratorStm::~RandomDataGeneratorStm()
    {
        RNG->CR &= ~RNG_CR_RNGEN;
        __HAL_RCC_RNG_CLK_DISABLE();
    }

    void RandomDataGeneratorStm::GenerateRandomData(infra::ByteRange result, const infra::Function<void()>& onDone)
    {
        assert(this->onDone == nullptr);

        if (result.empty())
            infra::EventDispatcher::Instance().Schedule(onDone);
        else
        {
            this->result = result;
            this->onDone = onDone;

            __DMB();
            RNG->CR |= RNG_CR_IE;
        }
    }

    void RandomDataGeneratorStm::Invoke()
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

            if (result.empty())
            {
                RNG->CR &= ~RNG_CR_IE;
                ClearPending();
                infra::EventDispatcher::Instance().Schedule(onDone);
                onDone = nullptr;
            }
        }
    }
}

#endif
