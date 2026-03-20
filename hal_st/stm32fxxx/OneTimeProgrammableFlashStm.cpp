#include "hal_st/stm32fxxx/OneTimeProgrammableFlashStm.hpp"
#include "hal_st/stm32fxxx/FlashInternalStmDetail.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/ReallyAssert.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include DEVICE_HEADER

namespace
{
    using ConstHalfWordRange = hal::OneTimeProgrammableFlashWorker::ConstHalfWordRange;

    template<uint32_t address>
    constexpr auto ptr()
    {
        return reinterpret_cast<const uint16_t*>(address);
    }

    constexpr size_t halfWordSize = sizeof(uint16_t);
    constexpr uint32_t sectorNumber{ 32 };
    constexpr uint32_t sectorSize{ FLASH_OTP_SIZE / sectorNumber };
    const ConstHalfWordRange OtpRange{ ptr<FLASH_OTP_BASE>(), ptr<FLASH_OTP_BASE + FLASH_OTP_SIZE>() };

    void Copy(const uint16_t* first, const uint16_t* last, uint16_t* result)
    {
        for (; first != last; ++result, ++first)
            *result = *first;
    }
}

namespace hal
{
    OneTimeProgrammableFlashWorker::OneTimeProgrammableFlashWorker()
        : flashMemory{ OtpRange }
    {}

    void OneTimeProgrammableFlashWorker::ReadBuffer(infra::ByteRange buffer, uint32_t address)
    {
        really_assert(buffer.size() % sizeof(uint16_t) == 0);
        // address is byte-based, addressAdjusted is half-word-based
        auto addressAdjusted = address / halfWordSize;
        auto destination = infra::ReinterpretCastMemoryRange<uint16_t>(buffer);
        // explicitely copy data uint16_t aligned and prevent gcc from (falsely) optimizing to memmove which will reinterpret as arrays of uint8_t
        Copy(flashMemory.begin() + addressAdjusted, flashMemory.begin() + addressAdjusted + destination.size(), destination.begin());
    }

    void OneTimeProgrammableFlashWorker::WriteBuffer(infra::ConstByteRange buffer, uint32_t address)
    {
        AssertDestinationIsUntouched(buffer, address);

        HAL_FLASH_Unlock();

        detail::AlignedWriteBuffer<uint16_t, FLASH_TYPEPROGRAM_HALFWORD_OTP, true>(buffer, address, reinterpret_cast<uint32_t>(flashMemory.begin()));

        HAL_FLASH_Lock();
    }

    void OneTimeProgrammableFlashWorker::EraseSectors(uint32_t beginIndex, uint32_t endIndex)
    {
        // OTP cannot be erased
        std::abort();
    }

    uint32_t OneTimeProgrammableFlashWorker::SectorNumber()
    {
        return sectorNumber;
    }

    uint32_t OneTimeProgrammableFlashWorker::SectorSize()
    {
        return sectorSize;
    }

    infra::MemoryRange<volatile uint16_t> OneTimeProgrammableFlashWorker::Range()
    {
        auto* base = reinterpret_cast<volatile uint16_t*>(FLASH_OTP_BASE);
        auto* end = reinterpret_cast<volatile uint16_t*>(FLASH_OTP_BASE + FLASH_OTP_SIZE);
        return infra::MakeRange(base, end);
    }

    void OneTimeProgrammableFlashWorker::AssertDestinationIsUntouched(infra::ConstByteRange buffer, uint32_t address)
    {
        auto writeBuffer = infra::ReinterpretCastMemoryRange<const uint16_t>(buffer);
        auto otpDestination = infra::Head(infra::DiscardHead(Range(), address / halfWordSize), writeBuffer.size());
        // From RM0481 Rev 4:
        // Section 7.3.9 OTP and RO memory access:
        // Overwriting an already programmed 16-bit half-word can lead to data and ECC errors,
        // and is therefore not supported.
        for (size_t offset = 0; const auto& halfWord : otpDestination)
        {
            really_assert(halfWord == 0xFFFF || halfWord == writeBuffer[offset]);
            ++offset;
        }
    }

    OneTimeProgrammableFlashStm::OneTimeProgrammableFlashStm()
        : FlashHomogeneous{ OneTimeProgrammableFlashWorker::SectorNumber(), OneTimeProgrammableFlashWorker::SectorSize() }
        , OneTimeProgrammableFlashWorker{}
    {}

    void OneTimeProgrammableFlashStm::ReadBuffer(infra::ByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        OneTimeProgrammableFlashWorker::ReadBuffer(buffer, address);
        infra::EventDispatcher::Instance().Schedule(onDone);
    }

    void OneTimeProgrammableFlashStm::WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        OneTimeProgrammableFlashWorker::WriteBuffer(buffer, address);
        infra::EventDispatcher::Instance().Schedule(onDone);
    }

    void OneTimeProgrammableFlashStm::EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone)
    {
        OneTimeProgrammableFlashWorker::EraseSectors(beginIndex, endIndex);
        infra::EventDispatcher::Instance().Schedule(onDone);
    }
}
