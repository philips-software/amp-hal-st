#ifndef SERVICES_ST_UTIL_ST_UART_BOOTLOADER_COMMAND_HANDLER_HPP
#define SERVICES_ST_UTIL_ST_UART_BOOTLOADER_COMMAND_HANDLER_HPP

#include "hal/interfaces/SerialCommunication.hpp"
#include "infra/event/QueueForOneReaderOneIrqWriter.hpp"
#include "infra/stream/ByteInputStream.hpp"
#include "infra/timer/Timer.hpp"
#include "infra/util/AutoResetFunction.hpp"
#include "infra/util/BoundedDeque.hpp"
#include "infra/util/BoundedString.hpp"
#include "infra/util/BoundedVector.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/Endian.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/Optional.hpp"
#include "infra/util/PolymorphicVariant.hpp"
#include "services/st_util/StBootloaderCommandHandler.hpp"
#include <array>
#include <cstdint>

namespace services
{
    class StUartBootloaderCommandHandler
        : public StBootloaderCommandHandler
    {
    public:
        StUartBootloaderCommandHandler(hal::SerialCommunication& serial, const infra::Function<void()>& onInitialized, const infra::Function<void(infra::BoundedConstString reason)>& onError);
        ~StUartBootloaderCommandHandler();

        // Implementation of StBootloaderCommandHandler
        void GetCommand(infra::ByteRange& commands, const infra::Function<void(uint8_t major, uint8_t minor)>& onDone) override;
        void GetVersion(const infra::Function<void(uint8_t major, uint8_t minor)>& onDone) override;
        void GetId(const infra::Function<void(uint16_t id)>& onDone) override;
        void ReadMemory(uint32_t address, infra::ByteRange& data, const infra::Function<void()>& onDone) override;
        void Go(uint32_t address, const infra::Function<void()>& onDone) override;
        void WriteMemory(uint32_t address, infra::ConstByteRange data, const infra::Function<void()>& onDone) override;
        void GlobalErase(const infra::Function<void()>& onDone) override;
        void Erase(infra::ConstByteRange pages, const infra::Function<void()>& onDone) override;
        void ExtendedErase(MassEraseSubcommand subcommand, const infra::Function<void()>& onDone) override;
        void ExtendedErase(infra::ConstByteRange pages, const infra::Function<void()>& onDone) override;
        void Special(uint16_t subcommand, infra::ConstByteRange txData, infra::ByteRange& rxData, infra::ByteRange& rxStatus, const infra::Function<void()>& onDone) override;
        void ExtendedSpecial(uint16_t subcommand, infra::ConstByteRange txData1, infra::ConstByteRange txData2, infra::ByteRange& rxData, const infra::Function<void()>& onDone) override;

    private:
        void InitializeUartBootloader();

        template<class T, class... Args>
        void AddCommand(Args&&... args);

        void ExecuteCommand(const infra::Function<void(), sizeof(StUartBootloaderCommandHandler*) + sizeof(infra::Function<void()>) + sizeof(infra::ByteRange)>& onCommandExecuted);
        void TryHandleTransmitAction();
        void TryHandleDataReceived();
        void TryHandleNextAction();
        void SendData(infra::ConstByteRange data, uint8_t checksum);
        void SendData(infra::ConstByteRange data);
        void SetCommandTimeout(infra::BoundedConstString reason);
        void OnError(infra::BoundedConstString reason);

    private:
        enum class ActionType : uint8_t
        {
            receive,
            transmit,
        };

        class Action
        {
        public:
            Action(StUartBootloaderCommandHandler& handler, ActionType type);
            Action(const Action& other) = default;
            Action& operator=(const Action& other) = default;
            virtual ~Action() = default;

            virtual void SendData();
            virtual void DataReceived();

            ActionType GetActionType() const;

        protected:
            StUartBootloaderCommandHandler& handler;

        private:
            ActionType type;
        };

        class ReceiveAction
            : public Action
        {
        public:
            explicit ReceiveAction(StUartBootloaderCommandHandler& handler);
        };

        class ReceiveAckAction
            : public ReceiveAction
        {
        public:
            using ReceiveAction::ReceiveAction;

            void DataReceived() override;
        };

        class ReceiveBufferAction
            : public ReceiveAction
        {
        public:
            ReceiveBufferAction(StUartBootloaderCommandHandler& handler, infra::ByteRange& data);
            ReceiveBufferAction(StUartBootloaderCommandHandler& handler, infra::ByteRange& data, const std::size_t nBytesTotal);

            void DataReceived() override;

        protected:
            virtual void ExtractNumberOfBytes(infra::ByteInputStream& stream) = 0;

        protected:
            infra::ByteRange& data;
            infra::Optional<std::size_t> nBytesTotal;
            std::size_t nBytesReceived = 0;
        };

        class ReceiveSmallBufferAction
            : public ReceiveBufferAction
        {
        public:
            using ReceiveBufferAction::ReceiveBufferAction;

        protected:
            void ExtractNumberOfBytes(infra::ByteInputStream& stream) override;
        };

        class ReceiveBigBufferAction
            : public ReceiveBufferAction
        {
        public:
            using ReceiveBufferAction::ReceiveBufferAction;

        protected:
            void ExtractNumberOfBytes(infra::ByteInputStream& stream) override;
        };

        class TransmitAction
            : public Action
        {
        public:
            explicit TransmitAction(StUartBootloaderCommandHandler& handler);
        };

        class TransmitRawAction
            : public TransmitAction
        {
        public:
            TransmitRawAction(StUartBootloaderCommandHandler& handler, infra::ConstByteRange data);

            void SendData() override;

        private:
            infra::ConstByteRange data;
        };

        class TransmitWithTwosComplementChecksum
            : public TransmitAction
        {
        public:
            TransmitWithTwosComplementChecksum(StUartBootloaderCommandHandler& handler, uint8_t data);

            void SendData() override;

        private:
            uint8_t data;
        };

        class TransmitChecksummedBuffer
            : public TransmitAction
        {
        public:
            TransmitChecksummedBuffer(StUartBootloaderCommandHandler& handler, infra::ConstByteRange data);

            void AddToChecksum(const uint8_t& byte);
            void SendData() override;

        private:
            infra::ConstByteRange data;
            uint8_t checksum = 0;
        };

        class TransmitSmallBuffer
            : public TransmitChecksummedBuffer
        {
        public:
            TransmitSmallBuffer(StUartBootloaderCommandHandler& handler, infra::ConstByteRange data);

            void SendData() override;

        private:
            uint8_t size;
        };

        class TransmitBigBuffer
            : public TransmitChecksummedBuffer
        {
        public:
            TransmitBigBuffer(StUartBootloaderCommandHandler& handler, uint16_t size, infra::ConstByteRange data);

            void SendData() override;

        private:
            infra::BigEndian<uint16_t> size;
        };

    private:
        hal::SerialCommunication& serial;
        infra::AutoResetFunction<void()> onInitialized;
        infra::AutoResetFunction<void(infra::BoundedConstString reason)> onError;
        infra::QueueForOneReaderOneIrqWriter<uint8_t>::WithStorage<257> queue;

        infra::BoundedDeque<infra::PolymorphicVariant<Action, ReceiveAckAction, ReceiveSmallBufferAction, ReceiveBigBufferAction, TransmitRawAction, TransmitWithTwosComplementChecksum, TransmitChecksummedBuffer, TransmitSmallBuffer, TransmitBigBuffer>>::WithMaxSize<12> commandActions;
        infra::AutoResetFunction<void(), sizeof(StUartBootloaderCommandHandler*) + sizeof(infra::Function<void()>) + sizeof(infra::ByteRange)> onCommandExecuted;
        infra::BoundedString::WithStorage<46> timeoutReason;
        infra::TimerSingleShot timeout;

        std::array<uint8_t, 3> internalBuffer;
        infra::ByteRange internalRange;
        infra::BigEndian<uint32_t> address;
        infra::BigEndian<uint16_t> subcommand;
    };
}

#endif
