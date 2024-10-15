#ifndef SERVICES_ST_UTIL_ST_BOOTLOADER_COMMUNICATOR_HPP
#define SERVICES_ST_UTIL_ST_BOOTLOADER_COMMUNICATOR_HPP

#include "infra/util/ByteRange.hpp"
#include "infra/util/Endian.hpp"
#include "infra/util/Function.hpp"
#include <cstdint>

namespace services
{
    enum class MassEraseSubcommand
    {
        global = 0xffff,
        bankOne = 0xfffE,
        bankTwo = 0xfffD,
    };

    class StBootloaderCommunicator
    {
    protected:
        StBootloaderCommunicator() = default;
        StBootloaderCommunicator(const StBootloaderCommunicator& other) = delete;
        StBootloaderCommunicator& operator=(const StBootloaderCommunicator& other) = delete;
        ~StBootloaderCommunicator() = default;

    public:
        // commands.size() >= 14 to accomodate for all possibly received commands.
        virtual void GetCommand(infra::ByteRange& commands, const infra::Function<void(uint8_t major, uint8_t minor)>& onDone) = 0;

        virtual void GetVersion(const infra::Function<void(uint8_t major, uint8_t minor)>& onDone) = 0;
        virtual void GetId(const infra::Function<void(uint16_t id)>& onDone) = 0;

        // 0 < data.size() <= 256
        virtual void ReadMemory(uint32_t address, infra::ByteRange& data, const infra::Function<void()>& onDone) = 0;

        virtual void Go(uint32_t address, const infra::Function<void()>& onDone) = 0;

        // 0 < data.size() <= 256, and data.size() must be 4-byte aligned
        virtual void WriteMemory(uint32_t address, infra::ConstByteRange data, const infra::Function<void()>& onDone) = 0;

        virtual void MassErase(const infra::Function<void()>& onDone) = 0;

        // 0 < pages.size() <= 255
        virtual void Erase(infra::ConstByteRange pages, const infra::Function<void()>& onDone) = 0;

        virtual void ExtendedMassErase(MassEraseSubcommand subcommand, const infra::Function<void()>& onDone) = 0;

        // 0 < pages.size()
        virtual void ExtendedErase(const infra::MemoryRange<infra::BigEndian<uint16_t>> pages, const infra::Function<void()>& onDone) = 0;

        // data.size() <= 128
        virtual void Special(uint16_t subcommand, infra::ConstByteRange txData, infra::ByteRange& rxData, infra::ByteRange& rxStatus, const infra::Function<void()>& onDone) = 0;

        // txData1.size() <= 128, txData2.size() <= 1024
        virtual void ExtendedSpecial(uint16_t subcommand, infra::ConstByteRange txData1, infra::ConstByteRange txData2, infra::ByteRange& rxData, const infra::Function<void()>& onDone) = 0;
    };
}

#endif
