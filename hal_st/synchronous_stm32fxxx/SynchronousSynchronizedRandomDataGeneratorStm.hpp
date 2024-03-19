#ifndef SYNCHRONOUS_HAL_SYNCHRONOUS_SYNCHRONIZED_RANDOM_DATA_GENERATOR_HPP
#define SYNCHRONOUS_HAL_SYNCHRONOUS_SYNCHRONIZED_RANDOM_DATA_GENERATOR_HPP

#include "hal/synchronous_interfaces/SynchronousRandomDataGenerator.hpp"
#include "hal_st/synchronous_stm32fxxx/SynchronousHardwareSemaphoreStm.hpp"
#include "infra/util/ProxyCreator.hpp"

namespace hal
{
    class SynchronousSynchronizedRandomDataGeneratorStm
        : public hal::SynchronousRandomDataGenerator
    {
    public:
        SynchronousSynchronizedRandomDataGeneratorStm(infra::CreatorBase<hal::SynchronousRandomDataGenerator, void()>& hwRngCreator, hal::SynchronousHardwareSemaphoreMasterStm& synchronousHardwareSemaphoreMasterStm);

        void GenerateRandomData(infra::ByteRange result) override;

    private:
        infra::DelayedProxyCreator<hal::SynchronousRandomDataGenerator, void()> hwRngCreator;
        hal::SynchronousHardwareSemaphoreMasterStm& synchronousHardwareSemaphoreMasterStm;
    };
}

#endif
