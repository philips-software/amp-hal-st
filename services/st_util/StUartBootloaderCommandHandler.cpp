#include "services/st_util/StUartBootloaderCommandHandler.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/Endian.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/MemoryRange.hpp"
#include "infra/util/Optional.hpp"
#include <algorithm>
#include <cstddef>
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
                  DataReceived();
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
        if (commands.size() < 15)
            std::abort();

        commandActions.emplace_back(infra::InPlaceType<TransmitWithTwosComplementChecksum>(), *this, getCommand);
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);
        commandActions.emplace_back(infra::InPlaceType<ReceiveRegularBufferAction>(), *this, commands);
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);

        timeout.Start(commandTimeout, [this]()
            {
                OnError("Timeout getting commands");
            });

        ExecuteCommand([this, onDone, &commands]()
            {
                uint8_t major = commands.front() >> 4;
                uint8_t minor = commands.front() & 0xf;

                for (auto i = 0; i < commands.size() - 1; ++i)
                    commands[i] = commands[i + 1];

                commands.back() = 0;
                commands = infra::DiscardTail(commands, 1);

                onDone(major, minor);
            });
    }

    void StUartBootloaderCommandHandler::GetVersion(const infra::Function<void(uint8_t major, uint8_t minor)>& onDone)
    {
        internalRange = infra::MakeByteRange(internalBuffer);

        commandActions.emplace_back(infra::InPlaceType<TransmitWithTwosComplementChecksum>(), *this, getVersion);
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);
        commandActions.emplace_back(infra::InPlaceType<ReceiveRegularBufferAction>(), *this, internalRange, 3);
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);

        timeout.Start(commandTimeout, [this]()
            {
                OnError("Timeout getting version");
            });

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

        commandActions.emplace_back(infra::InPlaceType<TransmitWithTwosComplementChecksum>(), *this, getId);
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);
        commandActions.emplace_back(infra::InPlaceType<ReceiveRegularBufferAction>(), *this, internalRange);
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);

        timeout.Start(commandTimeout, [this]()
            {
                OnError("Timeout getting id");
            });

        ExecuteCommand([this, onDone]()
            {
                onDone((internalBuffer[0] << 8) | internalBuffer[1]);
            });
    }

    void StUartBootloaderCommandHandler::ReadMemory(uint32_t address, uint8_t size, infra::ByteRange& data, const infra::Function<void()>& onDone)
    {
        this->address = address;
        this->size = size - 1;

        commandActions.emplace_back(infra::InPlaceType<TransmitWithTwosComplementChecksum>(), *this, readMemory);
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);
        commandActions.emplace_back(infra::InPlaceType<TransmitWithChecksumAction>(), *this, infra::MakeByteRange(this->address));
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);
        commandActions.emplace_back(infra::InPlaceType<TransmitWithTwosComplementChecksum>(), *this, this->size);
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);
        commandActions.emplace_back(infra::InPlaceType<ReceiveRegularBufferAction>(), *this, data, size);

        timeout.Start(commandTimeout, [this]()
            {
                OnError("Timeout reading memory");
            });

        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::Go(uint32_t address, const infra::Function<void()>& onDone)
    {
        this->address = address;

        commandActions.emplace_back(infra::InPlaceType<TransmitWithTwosComplementChecksum>(), *this, go);
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);
        commandActions.emplace_back(infra::InPlaceType<TransmitWithChecksumAction>(), *this, infra::MakeByteRange(this->address));
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);

        timeout.Start(commandTimeout, [this]()
            {
                OnError("Timeout go");
            });

        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::WriteMemory(uint32_t address, infra::ConstByteRange data, const infra::Function<void()>& onDone)
    {
        if (data.size() > 256)
            std::abort();

        this->address = address;
        internalBuffer[0] = static_cast<uint8_t>(data.size() + 1);
        std::copy(data.begin(), data.begin() + data.size(), internalBuffer.begin() + 1);

        commandActions.emplace_back(infra::InPlaceType<TransmitWithTwosComplementChecksum>(), *this, writeMemory);
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);
        commandActions.emplace_back(infra::InPlaceType<TransmitWithChecksumAction>(), *this, infra::MakeByteRange(this->address));
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);
        commandActions.emplace_back(infra::InPlaceType<TransmitWithChecksumAction>(), *this, infra::MakeRange(internalBuffer.begin(), internalBuffer.begin() + internalBuffer[0]));
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);

        timeout.Start(commandTimeout, [this]()
            {
                OnError("Timeout writing memory");
            });

        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::Erase(uint8_t subcommand, const infra::Function<void()>& onDone)
    {
        commandActions.emplace_back(infra::InPlaceType<TransmitWithTwosComplementChecksum>(), *this, erase);
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);
        commandActions.emplace_back(infra::InPlaceType<TransmitWithTwosComplementChecksum>(), *this, subcommand);
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);

        timeout.Start(commandTimeout, [this]()
            {
                OnError("Timeout erasing memory");
            });

        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::Erase(uint8_t nPages, infra::ConstByteRange pages, const infra::Function<void()>& onDone)
    {
        if (pages.size() > 256)
            std::abort();

        internalBuffer[0] = static_cast<uint8_t>(pages.size() + 1);
        std::copy(pages.begin(), pages.begin() + pages.size(), internalBuffer.begin() + 1);

        commandActions.emplace_back(infra::InPlaceType<TransmitWithTwosComplementChecksum>(), *this, erase);
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);
        commandActions.emplace_back(infra::InPlaceType<TransmitWithChecksumAction>(), *this, infra::MakeRange(internalBuffer.begin(), internalBuffer.begin() + pages.size() + 1));
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);

        timeout.Start(commandTimeout, [this]()
            {
                OnError("Timeout erasing memory");
            });

        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::ExtendedErase(uint16_t subcommand, const infra::Function<void()>& onDone)
    {
        this->subcommand = subcommand;

        commandActions.emplace_back(infra::InPlaceType<TransmitWithTwosComplementChecksum>(), *this, extendedErase);
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);
        commandActions.emplace_back(infra::InPlaceType<TransmitWithChecksumAction>(), *this, infra::MakeByteRange(this->subcommand));
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);

        timeout.Start(commandTimeout, [this]()
            {
                OnError("Timeout extended erasing memory");
            });

        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::ExtendedErase(uint8_t nPages, infra::ConstByteRange pages, const infra::Function<void()>& onDone)
    {
        if (pages.size() > 256)
            std::abort();

        internalBuffer[0] = static_cast<uint8_t>(nPages >> 8);
        internalBuffer[1] = static_cast<uint8_t>(nPages & 0xff) + 1;
        std::copy(pages.begin(), pages.begin() + pages.size(), internalBuffer.begin() + 2);

        commandActions.emplace_back(infra::InPlaceType<TransmitWithTwosComplementChecksum>(), *this, extendedErase);
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);
        commandActions.emplace_back(infra::InPlaceType<TransmitWithChecksumAction>(), *this, infra::MakeRange(internalBuffer.begin(), internalBuffer.begin() + pages.size() + 2));
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);

        timeout.Start(commandTimeout, [this]()
            {
                OnError("Timeout extended erasing memory");
            });

        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::Special(uint16_t subcommand, infra::ConstByteRange txData, infra::ByteRange& rxData, infra::ByteRange& rxStatus, const infra::Function<void()>& onDone)
    {
        this->subcommand = subcommand;

        internalBuffer[0] = static_cast<uint8_t>(txData.size() >> 8);
        internalBuffer[1] = static_cast<uint8_t>(txData.size() & 0xff);

        commandActions.emplace_back(infra::InPlaceType<TransmitWithTwosComplementChecksum>(), *this, special);
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);
        commandActions.emplace_back(infra::InPlaceType<TransmitWithChecksumAction>(), *this, infra::MakeByteRange(this->subcommand));
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);
        commandActions.emplace_back(infra::InPlaceType<TransmitRawAction>(), *this, infra::MakeRange(internalBuffer.begin(), internalBuffer.begin() + 2));
        if (!txData.empty())
            commandActions.emplace_back(infra::InPlaceType<TransmitWithChecksumAction>(), *this, txData);
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);
        commandActions.emplace_back(infra::InPlaceType<ReceiveSpecialBufferAction>(), *this, rxData);
        commandActions.emplace_back(infra::InPlaceType<ReceiveSpecialBufferAction>(), *this, rxStatus);

        timeout.Start(commandTimeout, [this]()
            {
                OnError("Timeout special command");
            });

        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::ExtendedSpecial(uint16_t subcommand, infra::ConstByteRange txData1, infra::ConstByteRange txData2, infra::ByteRange& rxData, const infra::Function<void()>& onDone)
    {
        this->subcommand = subcommand;

        internalBuffer[0] = static_cast<uint8_t>(txData1.size() >> 8);
        internalBuffer[1] = static_cast<uint8_t>(txData1.size() & 0xff);
        internalBuffer[2] = static_cast<uint8_t>(txData2.size() >> 8);
        internalBuffer[3] = static_cast<uint8_t>(txData2.size() & 0xff);

        commandActions.emplace_back(infra::InPlaceType<TransmitWithTwosComplementChecksum>(), *this, extendedSpecial);
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);
        commandActions.emplace_back(infra::InPlaceType<TransmitWithChecksumAction>(), *this, infra::MakeByteRange(this->subcommand));
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);
        commandActions.emplace_back(infra::InPlaceType<TransmitRawAction>(), *this, infra::MakeRange(internalBuffer.begin(), internalBuffer.begin() + 2));
        if (!txData1.empty())
            commandActions.emplace_back(infra::InPlaceType<TransmitWithChecksumAction>(), *this, txData1);
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);
        commandActions.emplace_back(infra::InPlaceType<TransmitRawAction>(), *this, infra::MakeRange(internalBuffer.begin() + 2, internalBuffer.begin() + 4));
        if (!txData2.empty())
            commandActions.emplace_back(infra::InPlaceType<TransmitWithChecksumAction>(), *this, txData2);
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);
        commandActions.emplace_back(infra::InPlaceType<ReceiveSpecialBufferAction>(), *this, rxData);

        timeout.Start(commandTimeout, [this]()
            {
                OnError("Timeout extended special command");
            });

        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::WriteProtect(infra::ConstByteRange sectors, const infra::Function<void()>& onDone)
    {
        std::abort();
    }

    void StUartBootloaderCommandHandler::WriteUnprotect(const infra::Function<void()>& onDone)
    {
        std::abort();
    }

    void StUartBootloaderCommandHandler::ReadoutProtect(const infra::Function<void()>& onDone)
    {
        std::abort();
    }

    void StUartBootloaderCommandHandler::ReadoutUnprotect(const infra::Function<void()>& onDone)
    {
        std::abort();
    }

    void StUartBootloaderCommandHandler::GetChecksum(uint32_t address, uint32_t memAreaSize, uint32_t crcPolinomial, uint32_t crcInitialization, const infra::Function<void(uint32_t crc, uint8_t checksum)>& onDone)
    {
        std::abort();
    }

    void StUartBootloaderCommandHandler::InitializeUartBootloader()
    {
        commandActions.emplace_back(infra::InPlaceType<TransmitRawAction>(), *this, infra::MakeByteRange(uartInitializationData));
        commandActions.emplace_back(infra::InPlaceType<ReceiveAckAction>(), *this);

        timeout.Start(commandTimeout, [this]()
            {
                OnError("Timeout waiting for bootloader initialization");
            });

        ExecuteCommand([this]()
            {
                onInitialized();
            });
    }

    void StUartBootloaderCommandHandler::ExecuteCommand(const infra::Function<void(), sizeof(StUartBootloaderCommandHandler*) + sizeof(infra::Function<void()>) + sizeof(infra::ByteRange)>& onCommandExecuted)
    {
        this->onCommandExecuted = onCommandExecuted;
        TryHandleTransmitAction();
    }

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
            commandActions.front()->DataReceived();
        else
            TryHandleTransmitAction();
    }

    void StUartBootloaderCommandHandler::SendData(infra::ConstByteRange data, uint8_t checksum)
    {
        claimer.Claim([this, data, checksum]()
            {
                serial.SendData(data, [this, checksum]()
                    {
                        serial.SendData(infra::MakeByteRange(checksum), [this]()
                            {
                                claimer.Release();
                                TryHandleTransmitAction();
                            });
                    });
            });
    }

    void StUartBootloaderCommandHandler::SendData(infra::ConstByteRange data)
    {
        claimer.Claim([this, data]()
            {
                serial.SendData(data, [this]()
                    {
                        claimer.Release();
                        TryHandleTransmitAction();
                    });
            });
    }

    void StUartBootloaderCommandHandler::DataReceived()
    {
        if (!commandActions.empty())
            commandActions.front()->DataReceived();
    }

    void StUartBootloaderCommandHandler::OnError(infra::BoundedConstString reason)
    {
        timeout.Cancel();
        serial.ReceiveData([](auto data) {});
        if (onError)
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

    StUartBootloaderCommandHandler::ReceiveBufferAction::ReceiveBufferAction(StUartBootloaderCommandHandler& handler, infra::ByteRange& data, const std::size_t nBytes)
        : StUartBootloaderCommandHandler::ReceiveAction(handler)
        , data(data)
    {
        this->nBytes.Emplace(nBytes);
    }

    void StUartBootloaderCommandHandler::ReceiveBufferAction::DataReceived()
    {
        if (handler.queue.Empty())
            return;

        infra::ByteInputStream stream(handler.queue.ContiguousRange());

        if (!nBytes)
            ExtractNumberOfBytes(stream);

        auto buffer = infra::Head(infra::DiscardHead(data, nBytesReceived), std::min(stream.Available(), *nBytes - nBytesReceived));
        stream >> buffer;
        handler.queue.Consume(buffer.size());

        nBytesReceived += buffer.size();
        if (nBytesReceived == nBytes)
        {
            data.shrink_from_back_to(nBytesReceived);
            handler.TryHandleNextAction();
        }
    }

    void StUartBootloaderCommandHandler::ReceiveRegularBufferAction::ExtractNumberOfBytes(infra::ByteInputStream& stream)
    {
            nBytes.Emplace(stream.Extract<uint8_t>() + 1);
            handler.queue.Consume(sizeof(uint8_t));
    }

    void StUartBootloaderCommandHandler::ReceiveSpecialBufferAction::ExtractNumberOfBytes(infra::ByteInputStream& stream)
    {
            nBytes.Emplace(stream.Extract<infra::BigEndian<uint16_t>>());
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
        handler.SendData(infra::MakeByteRange(data), checksum);
    }

    StUartBootloaderCommandHandler::TransmitWithChecksumAction::TransmitWithChecksumAction(StUartBootloaderCommandHandler& handler, infra::ConstByteRange data)
        : StUartBootloaderCommandHandler::TransmitAction(handler)
        , data(data)
    {}

    void StUartBootloaderCommandHandler::TransmitWithChecksumAction::SendData()
    {
        auto checksum = 0;

        for (auto b : data)
            checksum ^= b;

        handler.SendData(data, checksum);
    }
}