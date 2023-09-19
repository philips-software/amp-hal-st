#ifndef SERVICES_ST_UTIL_FLASH_ON_ST_UART_PROGRAMMER_HPP
#define SERVICES_ST_UTIL_FLASH_ON_ST_UART_PROGRAMMER_HPP

#include "hal/interfaces/Flash.hpp"
#include "services/st_util/StUartProgrammer.hpp"

namespace services
{
    class FlashOnStUartProgrammer
        : public hal::Flash
    {
    public:
        FlashOnStUartProgrammer(infra::MemoryRange<uint32_t> sectorSizes, StUartProgrammer& programmer);

        virtual uint32_t NumberOfSectors() const override;
        virtual uint32_t SizeOfSector(uint32_t sectorIndex) const override;
        virtual uint32_t SectorOfAddress(uint32_t address) const override;
        virtual uint32_t AddressOfSector(uint32_t sectorIndex) const override;
        virtual void WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone) override;
        virtual void ReadBuffer(infra::ByteRange buffer, uint32_t address, infra::Function<void()> onDone) override;
        virtual void EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone) override;

    private:
        infra::MemoryRange<uint32_t> sectorSizes;
        StUartProgrammer& programmer;

        infra::Function<void()> onDone;
        infra::ConstByteRange writeTail;
        infra::ByteRange readTail;
        uint32_t tailAddress = 0;
        uint32_t beginIndex = 0;
        uint32_t endIndex = 0;
    };
}

#endif
