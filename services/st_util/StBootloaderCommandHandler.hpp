#ifndef SERVICES_ST_UTIL_ST_BOOTLOADER_COMMAND_HANDLER_HPP
#define SERVICES_ST_UTIL_ST_BOOTLOADER_COMMAND_HANDLER_HPP

#include "infra/util//Function.hpp"

namespace services
{
    enum class MassEraseSubcommand
    {
        global = 0xffff,
        bankOne = 0xfffE,
        bankTwo = 0xfffD,
    };

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
        virtual void ReadMemory(uint32_t address, infra::ByteRange& data, const infra::Function<void()>& onDone) = 0;
        virtual void Go(uint32_t address, const infra::Function<void()>& onDone) = 0;
        virtual void WriteMemory(uint32_t address, infra::ConstByteRange data, const infra::Function<void()>& onDone) = 0;
        virtual void MassErase(const infra::Function<void()>& onDone) = 0;
        virtual void Erase(infra::ConstByteRange pages, const infra::Function<void()>& onDone) = 0;
        virtual void ExtendedMassErase(MassEraseSubcommand subcommand, const infra::Function<void()>& onDone) = 0;
        virtual void ExtendedErase(infra::ConstByteRange pages, const infra::Function<void()>& onDone) = 0;
        virtual void Special(uint16_t subcommand, infra::ConstByteRange txData, infra::ByteRange& rxData, infra::ByteRange& rxStatus, const infra::Function<void()>& onDone) = 0;
        virtual void ExtendedSpecial(uint16_t subcommand, infra::ConstByteRange txData1, infra::ConstByteRange txData2, infra::ByteRange& rxData, const infra::Function<void()>& onDone) = 0;
    };
}

#endif
