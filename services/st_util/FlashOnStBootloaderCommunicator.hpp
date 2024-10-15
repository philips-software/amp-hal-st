#ifndef SERVICES_ST_UTIL_FLASH_ON_ST_BOOTLOADER_COMMUNICATOR_HPP
#define SERVICES_ST_UTIL_FLASH_ON_ST_BOOTLOADER_COMMUNICATOR_HPP

#include "hal/interfaces/FlashHeterogeneous.hpp"
#include "hal/interfaces/FlashHomogeneous.hpp"
#include "infra/util/AutoResetFunction.hpp"
#include "infra/util/Endian.hpp"
#include "services/st_util/StBootloaderCommunicator.hpp"
#include <array>
#include <cstdint>

namespace services
{
    class FlashOnStBootloaderCommunicatorBase
    {
    public:
        explicit FlashOnStBootloaderCommunicatorBase(hal::Flash& flash, StBootloaderCommunicator& communicator);

        void WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone);
        void ReadBuffer(infra::ByteRange buffer, uint32_t address, infra::Function<void()> onDone);
        void EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone);

    private:
        StBootloaderCommunicator& communicator;
        hal::Flash& flash;

        infra::AutoResetFunction<void()> onDone;
        infra::ConstByteRange writeTail;
        infra::ByteRange readTail;
        uint32_t tailAddress = 0;
        uint32_t beginIndex = 0;
        uint32_t endIndex = 0;
        infra::BigEndian<uint16_t> page = 0;
    };

    template<class T, class... Args>
    class FlashOnStBootloaderCommunicator
        : public T
    {
    public:
        FlashOnStBootloaderCommunicator(Args... args, StBootloaderCommunicator& communicator);

        // Implementation of T (a partially derived class of hal::FlashBase)
        void WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone) override;
        void ReadBuffer(infra::ByteRange buffer, uint32_t address, infra::Function<void()> onDone) override;
        void EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone) override;

    private:
        FlashOnStBootloaderCommunicatorBase flashOnStBootloaderCommunicator;
    };

    template<class T, class... Args>
    FlashOnStBootloaderCommunicator<T, Args...>::FlashOnStBootloaderCommunicator(Args... args, StBootloaderCommunicator& communicator)
        : T(std::forward<Args>(args)...)
        , flashOnStBootloaderCommunicator(*this, communicator)
    {}

    template<class T, class... Args>
    void FlashOnStBootloaderCommunicator<T, Args...>::WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        flashOnStBootloaderCommunicator.WriteBuffer(buffer, address + 0x08000000, onDone);
    }

    template<class T, class... Args>
    void FlashOnStBootloaderCommunicator<T, Args...>::ReadBuffer(infra::ByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        flashOnStBootloaderCommunicator.ReadBuffer(buffer, address + 0x08000000, onDone);
    }

    template<class T, class... Args>
    void FlashOnStBootloaderCommunicator<T, Args...>::EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone)
    {
        flashOnStBootloaderCommunicator.EraseSectors(beginIndex, endIndex, onDone);
    }

    using FlashHomogeneousOnStBootloaderCommunicator = FlashOnStBootloaderCommunicator<hal::FlashHomogeneous, uint32_t, uint32_t>;
    using FlashHeterogeneousOnStBootloaderCommunicator = FlashOnStBootloaderCommunicator<hal::FlashHeterogeneous, infra::MemoryRange<uint32_t>>;
}

#endif
