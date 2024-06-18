#ifndef SERVICES_ST_UTIL_ST_UART_BOOTLOADER_COMMAND_HANDLER_HPP
#define SERVICES_ST_UTIL_ST_UART_BOOTLOADER_COMMAND_HANDLER_HPP

#include "hal/interfaces/SerialCommunication.hpp"
#include "infra/event/QueueForOneReaderOneIrqWriter.hpp"
#include "infra/timer/Timer.hpp"
#include "infra/util/AutoResetFunction.hpp"
#include "infra/util/BoundedDeque.hpp"
#include "infra/util/BoundedString.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/Endian.hpp"
#include "infra/util/PolymorphicVariant.hpp"
#include "services/st_util/StBootloaderCommunicator.hpp"

namespace services
{
    class StBootloaderCommunicatorUart
        : protected StBootloaderCommunicator
    {
    public:
        StBootloaderCommunicatorUart(hal::SerialCommunication& serial, const infra::Function<void()>& onInitialized, const infra::Function<void(infra::BoundedConstString reason)>& onError);
        virtual ~StBootloaderCommunicatorUart();

        // Implementation of StBootloaderCommunicator
        void GetCommand(infra::ByteRange& commands, const infra::Function<void(uint8_t major, uint8_t minor)>& onDone) override;
        void GetVersion(const infra::Function<void(uint8_t major, uint8_t minor)>& onDone) override;
        void GetId(const infra::Function<void(uint16_t id)>& onDone) override;
        void ReadMemory(uint32_t address, infra::ByteRange& data, const infra::Function<void()>& onDone) override;
        void Go(uint32_t address, const infra::Function<void()>& onDone) override;
        void WriteMemory(uint32_t address, infra::ConstByteRange data, const infra::Function<void()>& onDone) override;
        void MassErase(const infra::Function<void()>& onDone) override;
        void Erase(infra::ConstByteRange pages, const infra::Function<void()>& onDone) override;
        void ExtendedMassErase(MassEraseSubcommand subcommand, const infra::Function<void()>& onDone) override;
        void ExtendedErase(infra::ConstByteRange pages, const infra::Function<void()>& onDone) override;
        void Special(uint16_t subcommand, infra::ConstByteRange txData, infra::ByteRange& rxData, infra::ByteRange& rxStatus, const infra::Function<void()>& onDone) override;
        void ExtendedSpecial(uint16_t subcommand, infra::ConstByteRange txData1, infra::ConstByteRange txData2, infra::ByteRange& rxData, const infra::Function<void()>& onDone) override;

    private:
        void InitializeUartBootloader();

        template<class T, class... Args>
        void AddCommandAction(Args&&... args);

        void ExecuteCommand(const infra::Function<void(), sizeof(StBootloaderCommunicatorUart*) + sizeof(infra::Function<void()>) + sizeof(infra::ByteRange)>& onCommandExecuted);
        void StartCurrentAction();
        void TryHandleDataReceived();
        void OnCommandExecuted();
        void OnActionExecuted();
        void SetCommandTimeout(infra::BoundedConstString reason);
        void OnError(infra::BoundedConstString reason);
        void SendData(infra::ConstByteRange data, uint8_t checksum);
        void SendData(infra::ConstByteRange data);

    private:
        class Action
        {
        public:
            Action(StBootloaderCommunicatorUart& handler);
            Action(const Action& other) = delete;
            Action& operator=(const Action& other) = delete;
            virtual ~Action() = default;

            virtual void Start();
            virtual void DataReceived();

        protected:
            StBootloaderCommunicatorUart& handler;
        };

        class ReceiveAck
            : public Action
        {
        public:
            using Action::Action;

            void DataReceived() override;
        };

        class ReceiveBuffer
            : public Action
        {
        public:
            ReceiveBuffer(StBootloaderCommunicatorUart& handler, infra::ByteRange& data);

            void DataReceived() override;

        protected:
            virtual void TryRetreiveNumberOfBytes(infra::DataInputStream& stream) = 0;

        private:
            bool TotalNumberOfBytesAvailable(infra::DataInputStream& stream);
            void RetreiveData(infra::QueueForOneReaderOneIrqWriter<uint8_t>::StreamReader& reader, infra::DataInputStream& stream);
            void CheckActionExecuted();

        protected:
            infra::ByteRange& data;
            std::optional<std::size_t> nBytesTotal;
            std::size_t nBytesReceived = 0;
        };

        class ReceivePredefinedBuffer
            : public ReceiveBuffer
        {
        public:
            ReceivePredefinedBuffer(StBootloaderCommunicatorUart& handler, infra::ByteRange& data, const std::size_t size);

        protected:
            void TryRetreiveNumberOfBytes(infra::DataInputStream& stream) override;

        private:
            const std::size_t size;
        };

        class ReceiveSmallBuffer
            : public ReceiveBuffer
        {
        public:
            using ReceiveBuffer::ReceiveBuffer;

        protected:
            void TryRetreiveNumberOfBytes(infra::DataInputStream& stream) override;
        };

        class ReceiveBigBuffer
            : public ReceiveBuffer
        {
        public:
            using ReceiveBuffer::ReceiveBuffer;

        protected:
            void TryRetreiveNumberOfBytes(infra::DataInputStream& stream) override;
        };

        class TransmitRaw
            : public Action
        {
        public:
            TransmitRaw(StBootloaderCommunicatorUart& handler, infra::ConstByteRange data);

            void Start() override;

        private:
            infra::ConstByteRange data;
        };

        class TransmitWithTwosComplementChecksum
            : public Action
        {
        public:
            TransmitWithTwosComplementChecksum(StBootloaderCommunicatorUart& handler, uint8_t data);

            void Start() override;

        private:
            uint8_t data;
        };

        class TransmitChecksummedBuffer
            : public Action
        {
        public:
            TransmitChecksummedBuffer(StBootloaderCommunicatorUart& handler, infra::ConstByteRange data);

            void Start() override;

        protected:
            void AddToChecksum(const uint8_t& byte);

        private:
            infra::ConstByteRange data;
            uint8_t checksum = 0;
        };

        class TransmitSmallBuffer
            : public TransmitChecksummedBuffer
        {
        public:
            TransmitSmallBuffer(StBootloaderCommunicatorUart& handler, infra::ConstByteRange data);

            void Start() override;

        private:
            uint8_t size;
        };

        class TransmitBigBuffer
            : public TransmitChecksummedBuffer
        {
        public:
            TransmitBigBuffer(StBootloaderCommunicatorUart& handler, infra::ConstByteRange data, uint16_t size);

            void Start() override;

        private:
            infra::BigEndian<uint16_t> size;
        };

    private:
        hal::SerialCommunication& serial;
        infra::AutoResetFunction<void()> onInitialized;
        infra::AutoResetFunction<void(infra::BoundedConstString reason)> onError;
        infra::QueueForOneReaderOneIrqWriter<uint8_t>::WithStorage<257> queue;

        infra::BoundedDeque<infra::PolymorphicVariant<Action, ReceiveAck, ReceivePredefinedBuffer, ReceiveSmallBuffer, ReceiveBigBuffer, TransmitRaw, TransmitWithTwosComplementChecksum, TransmitChecksummedBuffer, TransmitSmallBuffer, TransmitBigBuffer>>::WithMaxSize<12> commandActions;
        infra::AutoResetFunction<void(), sizeof(StBootloaderCommunicatorUart*) + sizeof(infra::Function<void()>) + sizeof(infra::ByteRange)> onCommandExecuted;
        infra::BoundedString::WithStorage<46> timeoutReason;
        infra::TimerSingleShot timeout;

        std::array<uint8_t, 3> internalBuffer;
        infra::ByteRange internalRange{ internalBuffer };
        infra::BigEndian<uint32_t> address;
        infra::BigEndian<uint16_t> subcommand;
    };
}

#endif
