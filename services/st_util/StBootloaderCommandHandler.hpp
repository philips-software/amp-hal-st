#ifndef SERVICES_ST_UTIL_ST_BOOTLOADER_COMMAND_HANDLER_HPP
#define SERVICES_ST_UTIL_ST_BOOTLOADER_COMMAND_HANDLER_HPP

#include "infra/util//Function.hpp"

namespace services
{
    class StBootloaderCommandHandler
    {
    protected:
        StBootloaderCommandHandler() = default;
        StBootloaderCommandHandler(const StBootloaderCommandHandler& other) = delete;
        StBootloaderCommandHandler& operator=(const StBootloaderCommandHandler& other) = delete;
        ~StBootloaderCommandHandler() = default;

    public:
        virtual void GetCommand(infra::ByteRange& commands, const infra::Function<void(uint8_t major, uint8_t minor)>& onDone) = 0;
        virtual void GetVersion(const infra::Function<void(uint8_t major, uint8_t minor)>& onDone) = 0;
        virtual void GetId(const infra::Function<void(uint16_t id)>& onDone) = 0;
        virtual void ReadMemory(uint32_t address, uint8_t size, infra::ByteRange& data, const infra::Function<void()>& onDone) = 0;
        virtual void Go(uint32_t address, const infra::Function<void()>& onDone) = 0;
        virtual void WriteMemory(uint32_t address, infra::ConstByteRange data, const infra::Function<void()>& onDone) = 0;
        virtual void Erase(uint8_t subcommand, const infra::Function<void()>& onDone) = 0;
        virtual void Erase(uint8_t nPages, infra::ConstByteRange pages, const infra::Function<void()>& onDone) = 0;
        virtual void ExtendedErase(uint16_t subcommand, const infra::Function<void()>& onDone) = 0;
        virtual void ExtendedErase(uint8_t nPages, infra::ConstByteRange pages, const infra::Function<void()>& onDone) = 0;
        virtual void Special(uint16_t subcommand, infra::ConstByteRange txData, infra::ByteRange& rxData, infra::ByteRange& rxStatus, const infra::Function<void()>& onDone) = 0;
        virtual void ExtendedSpecial(uint16_t subcommand, infra::ConstByteRange txData1, infra::ConstByteRange txData2, infra::ByteRange& rxData, const infra::Function<void()>& onDone) = 0;
        virtual void WriteProtect(infra::ConstByteRange sectors, const infra::Function<void()>& onDone) = 0;
        virtual void WriteUnprotect(const infra::Function<void()>& onDone) = 0;
        virtual void ReadoutProtect(const infra::Function<void()>& onDone) = 0;
        virtual void ReadoutUnprotect(const infra::Function<void()>& onDone) = 0;
        virtual void GetChecksum(uint32_t address, uint32_t memAreaSize, uint32_t crcPolinomial, uint32_t crcInitialization, const infra::Function<void(uint32_t crc, uint8_t checksum)>& onDone) = 0;
    };
}

#endif
