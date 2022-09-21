#ifndef HAL_RANDOM_DATA_GENERATOR_STM_HPP
#define HAL_RANDOM_DATA_GENERATOR_STM_HPP

#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal/interfaces/RandomDataGenerator.hpp"

#if defined(HAS_PERIPHERAL_RNG)

namespace hal
{
    class RandomDataGeneratorStm
        : public RandomDataGenerator
        , private InterruptHandler
    {
    public:
        RandomDataGeneratorStm();
        ~RandomDataGeneratorStm();

        virtual void GenerateRandomData(infra::ByteRange result, const infra::Function<void()>& onDone) override;

    private:
        virtual void Invoke() override;

    private:
        infra::ByteRange result;
        infra::Function<void()> onDone;
    };
}

#endif

#endif
