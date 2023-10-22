#ifndef SERVICES_ST_UTIL_ST_UART_BOOTLOADER_COMMAND_HANDLER_HPP
#define SERVICES_ST_UTIL_ST_UART_BOOTLOADER_COMMAND_HANDLER_HPP

#include "hal/interfaces/SerialCommunication.hpp"
#include "infra/event/ClaimableResource.hpp"
#include "infra/event/QueueForOneReaderOneIrqWriter.hpp"
#include "infra/timer/Timer.hpp"
#include "infra/util/BoundedString.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/stream/ByteInputStream.hpp"
#include "infra/util/Optional.hpp"
#include "services/st_util/StBootloaderCommandHandler.hpp"
#include "infra/util/PolymorphicVariant.hpp"
#include <array>
#include <cstdint>
#include "infra/util/Endian.hpp"
#include <deque>

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
        void ReadMemory(uint32_t address, uint8_t size, infra::ByteRange& data, const infra::Function<void()>& onDone) override;
        void Go(uint32_t address, const infra::Function<void()>& onDone) override;
        void WriteMemory(uint32_t address, infra::ConstByteRange data, const infra::Function<void()>& onDone) override;
        void Erase(uint8_t subcommand, const infra::Function<void()>& onDone) override;
        void Erase(uint8_t nPages, infra::ConstByteRange pages, const infra::Function<void()>& onDone) override;
        void ExtendedErase(uint16_t subcommand, const infra::Function<void()>& onDone) override;
        void ExtendedErase(uint8_t nPages, infra::ConstByteRange pages, const infra::Function<void()>& onDone) override;
        void Special(uint16_t subcommand, infra::ConstByteRange txData, infra::ByteRange& rxData, infra::ByteRange& rxStatus, const infra::Function<void()>& onDone) override;
        void ExtendedSpecial(uint16_t subcommand, infra::ConstByteRange txData1, infra::ConstByteRange txData2, infra::ByteRange& rxData, const infra::Function<void()>& onDone) override;
        void WriteProtect(infra::ConstByteRange sectors, const infra::Function<void()>& onDone) override;
        void WriteUnprotect(const infra::Function<void()>& onDone) override;
        void ReadoutProtect(const infra::Function<void()>& onDone) override;
        void ReadoutUnprotect(const infra::Function<void()>& onDone) override;
        void GetChecksum(uint32_t address, uint32_t memAreaSize, uint32_t crcPolinomial, uint32_t crcInitialization, const infra::Function<void(uint32_t crc, uint8_t checksum)>& onDone) override;

    private:
        void InitializeUartBootloader();
        void ExecuteCommand(const infra::Function<void(), sizeof(StUartBootloaderCommandHandler*) + sizeof(infra::Function<void()>) + sizeof(infra::ByteRange)>& onCommandExecuted);
        void TryHandleTransmitAction();
        void TryHandleNextAction();
        void SendData(infra::ConstByteRange data, uint8_t checksum);
        void SendData(infra::ConstByteRange data);
        void DataReceived();
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
            ReceiveBufferAction(StUartBootloaderCommandHandler& handler, infra::ByteRange& data, const std::size_t nBytes);

            void DataReceived() override;

        protected:
            virtual void ExtractNumberOfBytes(infra::ByteInputStream& stream) {};

        protected:
            infra::ByteRange& data;
            infra::Optional<std::size_t> nBytes;
            std::size_t nBytesReceived = 0;
        };

        class ReceiveRegularBufferAction
            : public ReceiveBufferAction
        {
        public:
            using ReceiveBufferAction::ReceiveBufferAction;

        protected:
            void ExtractNumberOfBytes(infra::ByteInputStream& stream) override;
        };

        class ReceiveSpecialBufferAction
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

        class TransmitWithChecksumAction
            : public TransmitAction
        {
        public:
            TransmitWithChecksumAction(StUartBootloaderCommandHandler& handler, infra::ConstByteRange data);

            void SendData() override;

        private:
            infra::ConstByteRange data;
        };

    private:
        hal::SerialCommunication& serial;
        infra::AutoResetFunction<void()> onInitialized;
        infra::AutoResetFunction<void(infra::BoundedConstString reason)> onError;
        infra::QueueForOneReaderOneIrqWriter<uint8_t>::WithStorage<257> queue;
        infra::ClaimableResource resource;
        infra::ClaimableResource::Claimer startClaimer{ resource };
        infra::ClaimableResource::Claimer::WithSize<2 * sizeof(StUartBootloaderCommandHandler*) + 3 * sizeof(infra::ConstByteRange)> claimer{ resource };

        infra::TimerSingleShot timeout;
        std::array<uint8_t, 257> internalBuffer;
        infra::ByteRange internalRange;

        std::deque<infra::PolymorphicVariant<Action, ReceiveAckAction, ReceiveRegularBufferAction, ReceiveSpecialBufferAction, TransmitRawAction, TransmitWithTwosComplementChecksum, TransmitWithChecksumAction>> commandActions;
        infra::AutoResetFunction<void(), sizeof(StUartBootloaderCommandHandler*) + sizeof(infra::Function<void()>) + sizeof(infra::ByteRange)> onCommandExecuted;

        infra::BigEndian<uint32_t> address;
        infra::BigEndian<uint16_t> subcommand;
        uint8_t size;
    };
}

#endif
