#ifndef SERVICES_ST_UTIL_ST_BOOTLOADER_COMMUNICATOR_MOCK_HPP
#define SERVICES_ST_UTIL_ST_BOOTLOADER_COMMUNICATOR_MOCK_HPP

#include "services/st_util/StBootloaderCommunicator.hpp"
#include "gmock/gmock.h"

namespace services
{
    class StBootloaderCommunicatorMock
        : public StBootloaderCommunicator
    {
    public:
        MOCK_METHOD(void, GetCommand, (infra::ByteRange& commands, const infra::Function<void(uint8_t major, uint8_t minor)>& onDone));
        MOCK_METHOD(void, GetVersion, (const infra::Function<void(uint8_t major, uint8_t minor)>& onDone));
        MOCK_METHOD(void, GetId, (const infra::Function<void(uint16_t id)>& onDone));
        MOCK_METHOD(void, ReadMemory, (uint32_t address, infra::ByteRange& data, const infra::Function<void()>& onDone));
        MOCK_METHOD(void, Go, (uint32_t address, const infra::Function<void()>& onDone));
        MOCK_METHOD(void, WriteMemory, (uint32_t address, infra::ConstByteRange data, const infra::Function<void()>& onDone));
        MOCK_METHOD(void, MassErase, (const infra::Function<void()>& onDone));
        MOCK_METHOD(void, Erase, (infra::ConstByteRange pages, const infra::Function<void()>& onDone));
        MOCK_METHOD(void, ExtendedMassErase, (MassEraseSubcommand subcommand, const infra::Function<void()>& onDone));
        MOCK_METHOD(void, ExtendedErase, (infra::ConstByteRange pages, const infra::Function<void()>& onDone));
        MOCK_METHOD(void, Special, (uint16_t subcommand, infra::ConstByteRange txData, infra::ByteRange& rxData, infra::ByteRange& rxStatus, const infra::Function<void()>& onDone));
        MOCK_METHOD(void, ExtendedSpecial, (uint16_t subcommand, infra::ConstByteRange txData1, infra::ConstByteRange txData2, infra::ByteRange& rxData, const infra::Function<void()>& onDone));
    };
}

#endif
