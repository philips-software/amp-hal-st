#ifndef HAL_SYNCHRONOUS_RANDOM_DATA_GENERATOR_STM_HPP
#define HAL_SYNCHRONOUS_RANDOM_DATA_GENERATOR_STM_HPP

#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal/synchronous_interfaces/SynchronousRandomDataGenerator.hpp"

#if defined(HAS_PERIPHERAL_RNG)

namespace hal
{
    class SynchronousRandomDataGeneratorStm
        : public SynchronousRandomDataGenerator
    {
    public:
        SynchronousRandomDataGeneratorStm();
        ~SynchronousRandomDataGeneratorStm();

        virtual void GenerateRandomData(infra::ByteRange result) override;
    };
}

#endif

#endif
