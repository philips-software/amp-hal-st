#ifndef SERVICES_ST_UTIL_ST_UART_PROGRAMMER_HPP
#define SERVICES_ST_UTIL_ST_UART_PROGRAMMER_HPP

#include "hal/interfaces/SerialCommunication.hpp"
#include "infra/event/ClaimableResource.hpp"
#include "infra/event/QueueForOneReaderOneIrqWriter.hpp"
#include "infra/timer/Timer.hpp"
#include "infra/util/BoundedVector.hpp"
#include "infra/util/Endian.hpp"

namespace services
{
    class StUartProgrammer
    {
    public:
        StUartProgrammer(hal::SerialCommunication& serial, const infra::Function<void()>& onInitialized, const infra::Function<void(infra::BoundedConstString reason)>& onError);
        ~StUartProgrammer();

        void GetVersion(const infra::Function<void(uint8_t major, uint8_t minor)>& onDone);
        void GetId(const infra::Function<void(uint16_t id)>& onDone);
        void ReadMemory(uint32_t address, infra::ByteRange data, const infra::Function<void()>& onDone);       // 1 to 256 bytes
        void WriteMemory(uint32_t address, infra::ConstByteRange data, const infra::Function<void()>& onDone); // 1 to 256 bytes
        void EraseAll(const infra::Function<void()>& onDone);
        void ErasePage(uint16_t page, const infra::Function<void()>& onDone);

    private:
        void SendHello();
        void CommandSendData(uint8_t command, infra::ConstByteRange data, const infra::Function<void()>& onDone);
        void CommandSendDataSendData(uint8_t command, infra::ConstByteRange data, infra::ConstByteRange data2, const infra::Function<void()>& onDone);
        void CommandReceiveData(uint8_t command, infra::ByteRange receiveData, const infra::Function<void(), sizeof(StUartProgrammer*) + sizeof(infra::Function<void()>)>& onDone);
        void CommandSendDataSendSingleDataReceiveData(uint8_t command, infra::ConstByteRange sendData, uint8_t sendData2, infra::ByteRange receiveData, const infra::Function<void(), sizeof(infra::Function<void()>)>& onDone);

        void RawSendCommand(uint8_t command);
        void RawSendSingleData(uint8_t data);
        void RawSendData(infra::ConstByteRange data);
        void DataReceived(infra::QueueForOneReaderOneIrqWriter<uint8_t>::StreamReader& reader);
        void OnError(infra::BoundedConstString reason);

    private:
        hal::SerialCommunication& serial;
        infra::AutoResetFunction<void()> onInitialized;
        infra::AutoResetFunction<void(infra::BoundedConstString reason)> onError;
        infra::QueueForOneReaderOneIrqWriter<uint8_t>::WithStorage<257> queue;
        infra::ClaimableResource resource;
        infra::ClaimableResource::Claimer startClaimer{ resource };
        infra::ClaimableResource::Claimer::WithSize<2 * sizeof(StUartProgrammer*) + 3 * sizeof(infra::ConstByteRange)> claimer{ resource };

        std::array<uint8_t, 2> commandBytes;
        uint8_t checksum;
        uint8_t waitingForBytes = 0;

        infra::AutoResetFunction<void(), 2 * sizeof(StUartProgrammer*) + 3 * sizeof(infra::ConstByteRange)> onResponse;
        infra::AutoResetFunction<void(), sizeof(StUartProgrammer*) + sizeof(infra::Function<void()>)> onDone;
        infra::ByteRange receiveData;

        infra::BigEndian<uint32_t> address;
        infra::BoundedVector<uint8_t>::WithMaxSize<257> data;
        uint8_t size;

        infra::TimerSingleShot timeout;
    };
}

#endif
