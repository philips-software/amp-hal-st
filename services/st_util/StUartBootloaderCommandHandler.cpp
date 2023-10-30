#include "services/st_util/StUartBootloaderCommandHandler.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/ReallyAssert.hpp"
#include <cstdint>

namespace
{
    constexpr uint8_t uartInitializationData = 0x7f;

    constexpr uint8_t getCommand = 0x00;
    constexpr uint8_t getVersion = 0x01;
    constexpr uint8_t getId = 0x02;
    constexpr uint8_t readMemory = 0x11;
    constexpr uint8_t go = 0x21;
    constexpr uint8_t writeMemory = 0x31;
    constexpr uint8_t erase = 0x43;
    constexpr uint8_t extendedErase = 0x44;
    constexpr uint8_t special = 0x50;
    constexpr uint8_t extendedSpecial = 0x51;
    constexpr uint8_t writeProtect = 0x63;
    constexpr uint8_t writeUnprotect = 0x73;
    constexpr uint8_t readoutProtect = 0x82;
    constexpr uint8_t readoutUnprotect = 0x92;
    constexpr uint8_t getChecksum = 0xa1;

    constexpr uint8_t ack = 0x79;
    constexpr uint8_t nack = 0x1F;

    constexpr auto commandTimeout = std::chrono::seconds(1);
}

namespace services
{
    StUartBootloaderCommandHandler::StUartBootloaderCommandHandler(hal::SerialCommunication& serial, const infra::Function<void()>& onInitialized, const infra::Function<void(infra::BoundedConstString reason)>& onError)
        : serial(serial)
        , onInitialized(onInitialized)
        , onError(onError)
        , queue([this]()
              {
                  TryHandleDataReceived();
              })
    {
        serial.ReceiveData([this](auto data)
            {
                queue.AddFromInterrupt(data);
            });

        InitializeUartBootloader();
    }

    StUartBootloaderCommandHandler::~StUartBootloaderCommandHandler()
    {
        serial.ReceiveData([](auto data) {});
    }

    void StUartBootloaderCommandHandler::GetCommand(infra::ByteRange& commands, const infra::Function<void(uint8_t major, uint8_t minor)>& onDone)
    {
        really_assert(commands.size() >= 15);

        AddCommand<TransmitWithTwosComplementChecksum>(getCommand);
        AddCommand<ReceiveAckAction>();
        AddCommand<ReceiveSmallBufferAction>(commands);
        AddCommand<ReceiveAckAction>();

        SetCommandTimeout("Timeout getting commands");
        ExecuteCommand([this, onDone, &commands]()
            {
                uint8_t major = commands.front() >> 4;
                uint8_t minor = commands.front() & 0xf;

                for (auto i = 0; i < commands.size() - 1; ++i)
                    commands[i] = commands[i + 1];

                commands.back() = 0;
                commands.pop_back();

                onDone(major, minor);
            });
    }

    void StUartBootloaderCommandHandler::GetVersion(const infra::Function<void(uint8_t major, uint8_t minor)>& onDone)
    {
        internalRange = infra::MakeByteRange(internalBuffer);

        AddCommand<TransmitWithTwosComplementChecksum>(getVersion);
        AddCommand<ReceiveAckAction>();
        AddCommand<ReceiveSmallBufferAction>(internalRange, 3);
        AddCommand<ReceiveAckAction>();

        SetCommandTimeout("Timeout getting version");
        ExecuteCommand([this, onDone]()
            {
                uint8_t major = internalBuffer[0] >> 4;
                uint8_t minor = internalBuffer[0] & 0xf;
                onDone(major, minor);
            });
    }

    void StUartBootloaderCommandHandler::GetId(const infra::Function<void(uint16_t id)>& onDone)
    {
        internalRange = infra::MakeByteRange(internalBuffer);

        AddCommand<TransmitWithTwosComplementChecksum>(getId);
        AddCommand<ReceiveAckAction>();
        AddCommand<ReceiveSmallBufferAction>(internalRange);
        AddCommand<ReceiveAckAction>();

        SetCommandTimeout("Timeout getting id");
        ExecuteCommand([this, onDone]()
            {
                uint16_t id = (internalBuffer[0] << 8) | internalBuffer[1];
                onDone(id);
            });
    }

    void StUartBootloaderCommandHandler::ReadMemory(uint32_t address, infra::ByteRange& data, const infra::Function<void()>& onDone)
    {
        this->address = address;

        AddCommand<TransmitWithTwosComplementChecksum>(readMemory);
        AddCommand<ReceiveAckAction>();
        AddCommand<TransmitChecksummedBuffer>(infra::MakeConstByteRange(this->address));
        AddCommand<ReceiveAckAction>();
        AddCommand<TransmitWithTwosComplementChecksum>(data.size() - 1);
        AddCommand<ReceiveAckAction>();
        AddCommand<ReceiveSmallBufferAction>(data, data.size());

        SetCommandTimeout("Timeout reading memory");
        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::Go(uint32_t address, const infra::Function<void()>& onDone)
    {
        this->address = address;

        AddCommand<TransmitWithTwosComplementChecksum>(go);
        AddCommand<ReceiveAckAction>();
        AddCommand<TransmitChecksummedBuffer>(infra::MakeConstByteRange(this->address));
        AddCommand<ReceiveAckAction>();

        SetCommandTimeout("Timeout go");
        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::WriteMemory(uint32_t address, infra::ConstByteRange data, const infra::Function<void()>& onDone)
    {
        really_assert(data.size() <= 256);

        this->address = address;

        AddCommand<TransmitWithTwosComplementChecksum>(writeMemory);
        AddCommand<ReceiveAckAction>();
        AddCommand<TransmitChecksummedBuffer>(infra::MakeConstByteRange(this->address));
        AddCommand<ReceiveAckAction>();
        AddCommand<TransmitSmallBuffer>(data);
        AddCommand<ReceiveAckAction>();

        SetCommandTimeout("Timeout writing memory");
        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::GlobalErase(const infra::Function<void()>& onDone)
    {
        constexpr uint8_t globalEraseSubCommand = 0xff;

        AddCommand<TransmitWithTwosComplementChecksum>(erase);
        AddCommand<ReceiveAckAction>();
        AddCommand<TransmitWithTwosComplementChecksum>(globalEraseSubCommand);
        AddCommand<ReceiveAckAction>();

        SetCommandTimeout("Timeout erasing memory");
        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::Erase(infra::ConstByteRange pages, const infra::Function<void()>& onDone)
    {
        really_assert(pages.size() <= 255);

        AddCommand<TransmitWithTwosComplementChecksum>(erase);
        AddCommand<ReceiveAckAction>();
        AddCommand<TransmitSmallBuffer>(pages);
        AddCommand<ReceiveAckAction>();

        SetCommandTimeout("Timeout erasing memory");
        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::ExtendedErase(MassEraseSubcommand subcommand, const infra::Function<void()>& onDone)
    {
        this->subcommand = static_cast<uint16_t>(subcommand);

        AddCommand<TransmitWithTwosComplementChecksum>(extendedErase);
        AddCommand<ReceiveAckAction>();
        AddCommand<TransmitChecksummedBuffer>(infra::MakeConstByteRange(this->subcommand));
        AddCommand<ReceiveAckAction>();

        SetCommandTimeout("Timeout extended erasing memory");
        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::ExtendedErase(infra::ConstByteRange pages, const infra::Function<void()>& onDone)
    {
        really_assert(pages.size() <= 256);
        really_assert(pages.size() % 2 == 0);

        AddCommand<TransmitWithTwosComplementChecksum>(extendedErase);
        AddCommand<ReceiveAckAction>();
        AddCommand<TransmitBigBuffer>((pages.size() / 2) - 1, pages);
        AddCommand<ReceiveAckAction>();

        SetCommandTimeout("Timeout extended erasing memory");
        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::Special(uint16_t subcommand, infra::ConstByteRange txData, infra::ByteRange& rxData, infra::ByteRange& rxStatus, const infra::Function<void()>& onDone)
    {
        really_assert(txData.size() <= 128);

        this->subcommand = subcommand;

        AddCommand<TransmitWithTwosComplementChecksum>(special);
        AddCommand<ReceiveAckAction>();
        AddCommand<TransmitChecksummedBuffer>(infra::MakeByteRange(this->subcommand));
        AddCommand<ReceiveAckAction>();
        AddCommand<TransmitBigBuffer>(txData.size(), txData);
        AddCommand<ReceiveAckAction>();
        AddCommand<ReceiveBigBufferAction>(rxData);
        AddCommand<ReceiveBigBufferAction>(rxStatus);

        SetCommandTimeout("Timeout special command");
        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::ExtendedSpecial(uint16_t subcommand, infra::ConstByteRange txData1, infra::ConstByteRange txData2, infra::ByteRange& rxData, const infra::Function<void()>& onDone)
    {
        really_assert(txData1.size() <= 128);
        really_assert(txData2.size() <= 1024);

        this->subcommand = subcommand;

        AddCommand<TransmitWithTwosComplementChecksum>(extendedSpecial);
        AddCommand<ReceiveAckAction>();
        AddCommand<TransmitChecksummedBuffer>(infra::MakeByteRange(this->subcommand));
        AddCommand<ReceiveAckAction>();
        AddCommand<TransmitBigBuffer>(txData1.size(), txData1);
        AddCommand<ReceiveAckAction>();
        AddCommand<TransmitBigBuffer>(txData2.size(), txData2);
        AddCommand<ReceiveAckAction>();
        AddCommand<ReceiveBigBufferAction>(rxData);

        SetCommandTimeout("Timeout extended special command");
        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::InitializeUartBootloader()
    {
        AddCommand<TransmitRawAction>(infra::MakeByteRange(uartInitializationData));
        AddCommand<ReceiveAckAction>();

        SetCommandTimeout("Timeout waiting for bootloader initialization");
        ExecuteCommand([this]()
            {
                onInitialized();
            });
    }

    template<class T, class... Args>
    void StUartBootloaderCommandHandler::AddCommand(Args&&... args)
    {
        commandActions.emplace_back(infra::InPlaceType<T>(), *this, std::forward<Args>(args)...);
    }

    void StUartBootloaderCommandHandler::ExecuteCommand(const infra::Function<void(), sizeof(StUartBootloaderCommandHandler*) + sizeof(infra::Function<void()>) + sizeof(infra::ByteRange)>& onCommandExecuted)
    {
        this->onCommandExecuted = onCommandExecuted;
        TryHandleTransmitAction();
    }

    // Remove the aborts for each action, and leave them empty.
    // Get rid of ActionType
    void StUartBootloaderCommandHandler::TryHandleTransmitAction()
    {
        if (commandActions.front()->GetActionType() == ActionType::transmit)
        {
            static auto transmitaction = commandActions.front();
            transmitaction = commandActions.front();

            commandActions.pop_front();
            transmitaction->SendData();
        }
    }

    void StUartBootloaderCommandHandler::TryHandleDataReceived()
    {
        if (!queue.Empty())
            commandActions.front()->DataReceived();
    }

    void StUartBootloaderCommandHandler::TryHandleNextAction()
    {
        commandActions.pop_front();

        if (commandActions.empty())
        {
            timeout.Cancel();
            onCommandExecuted();
            return;
        }

        if (commandActions.front()->GetActionType() == ActionType::receive)
            TryHandleDataReceived();
        else
            TryHandleTransmitAction();
    }

    void StUartBootloaderCommandHandler::SendData(infra::ConstByteRange data, uint8_t checksum)
    {
        serial.SendData(data, [this, checksum]()
            {
                SendData(infra::MakeConstByteRange(checksum));
            });
    }

    void StUartBootloaderCommandHandler::SendData(infra::ConstByteRange data)
    {
        serial.SendData(data, [this]()
            {
                TryHandleTransmitAction();
            });
    }

    void StUartBootloaderCommandHandler::SetCommandTimeout(infra::BoundedConstString reason)
    {
        timeoutReason = reason;
        timeout.Start(commandTimeout, [this]()
            {
                OnError(timeoutReason);
            });
    }

    void StUartBootloaderCommandHandler::OnError(infra::BoundedConstString reason)
    {
        timeout.Cancel();
        serial.ReceiveData([](auto data) {});
        if (onError != nullptr)
            onError(reason);
    }

    StUartBootloaderCommandHandler::Action::Action(StUartBootloaderCommandHandler& handler, StUartBootloaderCommandHandler::ActionType type)
        : handler(handler)
        , type(type)
    {}

    void StUartBootloaderCommandHandler::Action::SendData()
    {
        std::abort();
    }

    void StUartBootloaderCommandHandler::Action::DataReceived()
    {
        std::abort();
    }

    StUartBootloaderCommandHandler::ActionType StUartBootloaderCommandHandler::Action::GetActionType() const
    {
        return type;
    }

    StUartBootloaderCommandHandler::ReceiveAction::ReceiveAction(StUartBootloaderCommandHandler& handler)
        : StUartBootloaderCommandHandler::Action(handler, StUartBootloaderCommandHandler::ActionType::receive)
    {}

    StUartBootloaderCommandHandler::TransmitAction::TransmitAction(StUartBootloaderCommandHandler& handler)
        : StUartBootloaderCommandHandler::Action(handler, StUartBootloaderCommandHandler::ActionType::transmit)
    {}

    void StUartBootloaderCommandHandler::ReceiveAckAction::DataReceived()
    {
        auto byte = handler.queue.Get();

        if (byte == ack)
            handler.TryHandleNextAction();
        else
            handler.OnError("NACK received");
    }

    StUartBootloaderCommandHandler::ReceiveBufferAction::ReceiveBufferAction(StUartBootloaderCommandHandler& handler, infra::ByteRange& data)
        : StUartBootloaderCommandHandler::ReceiveAction(handler)
        , data(data)
    {}

    StUartBootloaderCommandHandler::ReceiveBufferAction::ReceiveBufferAction(StUartBootloaderCommandHandler& handler, infra::ByteRange& data, const std::size_t nBytesTotal)
        : StUartBootloaderCommandHandler::ReceiveAction(handler)
        , data(data)
    {
        this->nBytesTotal.Emplace(nBytesTotal);
    }

    void StUartBootloaderCommandHandler::ReceiveBufferAction::DataReceived()
    {
        infra::ByteInputStream stream(handler.queue.ContiguousRange());

        if (!nBytesTotal)
            ExtractNumberOfBytes(stream);

        auto buffer = infra::Head(infra::DiscardHead(data, nBytesReceived), std::min(stream.Available(), *nBytesTotal - nBytesReceived));
        stream >> buffer;
        handler.queue.Consume(buffer.size());

        nBytesReceived += buffer.size();
        if (nBytesReceived == nBytesTotal)
        {
            data.shrink_from_back_to(nBytesReceived);
            handler.TryHandleNextAction();
        }
    }

    void StUartBootloaderCommandHandler::ReceiveSmallBufferAction::ExtractNumberOfBytes(infra::ByteInputStream& stream)
    {
        nBytesTotal.Emplace(stream.Extract<uint8_t>() + 1);
        handler.queue.Consume(sizeof(uint8_t));
    }

    void StUartBootloaderCommandHandler::ReceiveBigBufferAction::ExtractNumberOfBytes(infra::ByteInputStream& stream)
    {
        nBytesTotal.Emplace(stream.Extract<infra::BigEndian<uint16_t>>());
        handler.queue.Consume(sizeof(uint16_t));
    }

    StUartBootloaderCommandHandler::TransmitRawAction::TransmitRawAction(StUartBootloaderCommandHandler& handler, infra::ConstByteRange data)
        : StUartBootloaderCommandHandler::TransmitAction(handler)
        , data(data)
    {}

    void StUartBootloaderCommandHandler::TransmitRawAction::SendData()
    {
        handler.SendData(data);
    }

    StUartBootloaderCommandHandler::TransmitWithTwosComplementChecksum::TransmitWithTwosComplementChecksum(StUartBootloaderCommandHandler& handler, uint8_t data)
        : StUartBootloaderCommandHandler::TransmitAction(handler)
        , data(data)
    {}

    void StUartBootloaderCommandHandler::TransmitWithTwosComplementChecksum::SendData()
    {
        auto checksum = data ^ 0xff;
        handler.SendData(infra::MakeConstByteRange(data), checksum);
    }

    StUartBootloaderCommandHandler::TransmitChecksummedBuffer::TransmitChecksummedBuffer(StUartBootloaderCommandHandler& handler, infra::ConstByteRange data)
        : StUartBootloaderCommandHandler::TransmitAction(handler)
        , data(data)
    {
        for (auto& b : data)
            AddToChecksum(b);
    }

    void StUartBootloaderCommandHandler::TransmitChecksummedBuffer::AddToChecksum(const uint8_t& byte)
    {
        checksum ^= byte;
    }

    void StUartBootloaderCommandHandler::TransmitChecksummedBuffer::SendData()
    {
        if (data.empty())
            handler.SendData(infra::MakeConstByteRange(checksum));
        else
            handler.SendData(data, checksum);
    }

    StUartBootloaderCommandHandler::TransmitSmallBuffer::TransmitSmallBuffer(StUartBootloaderCommandHandler& handler, infra::ConstByteRange data)
        : StUartBootloaderCommandHandler::TransmitChecksummedBuffer(handler, data)
    {
        size = data.size() - 1;
        AddToChecksum(size);
    }

    void StUartBootloaderCommandHandler::TransmitSmallBuffer::SendData()
    {
        handler.serial.SendData(infra::MakeConstByteRange(size), [this]()
            {
                StUartBootloaderCommandHandler::TransmitChecksummedBuffer::SendData();
            });
    }

    StUartBootloaderCommandHandler::TransmitBigBuffer::TransmitBigBuffer(StUartBootloaderCommandHandler& handler, uint16_t size, infra::ConstByteRange data)
        : StUartBootloaderCommandHandler::TransmitChecksummedBuffer(handler, data)
        , size(size)
    {
        AddToChecksum(static_cast<uint8_t>(size >> 8));
        AddToChecksum(static_cast<uint8_t>(size & 0xff));
    }

    void StUartBootloaderCommandHandler::TransmitBigBuffer::SendData()
    {
        handler.serial.SendData(infra::MakeConstByteRange(size), [this]()
            {
                StUartBootloaderCommandHandler::TransmitChecksummedBuffer::SendData();
            });
    }
}
