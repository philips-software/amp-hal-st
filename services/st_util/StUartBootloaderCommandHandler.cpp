#include "services/st_util/StUartBootloaderCommandHandler.hpp"
#include "infra/util/Optional.hpp"
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

        AddCommandAction<TransmitWithTwosComplementChecksum>(getCommand);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<ReceiveSmallBuffer>(commands);
        AddCommandAction<ReceiveAck>();

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

        AddCommandAction<TransmitWithTwosComplementChecksum>(getVersion);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<ReceivePredefinedBuffer>(internalRange, 3);
        AddCommandAction<ReceiveAck>();

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

        AddCommandAction<TransmitWithTwosComplementChecksum>(getId);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<ReceiveSmallBuffer>(internalRange);
        AddCommandAction<ReceiveAck>();

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

        AddCommandAction<TransmitWithTwosComplementChecksum>(readMemory);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitChecksummedBuffer>(infra::MakeConstByteRange(this->address));
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitWithTwosComplementChecksum>(data.size() - 1);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<ReceivePredefinedBuffer>(data, data.size());

        SetCommandTimeout("Timeout reading memory");
        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::Go(uint32_t address, const infra::Function<void()>& onDone)
    {
        this->address = address;

        AddCommandAction<TransmitWithTwosComplementChecksum>(go);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitChecksummedBuffer>(infra::MakeConstByteRange(this->address));
        AddCommandAction<ReceiveAck>();

        SetCommandTimeout("Timeout go");
        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::WriteMemory(uint32_t address, infra::ConstByteRange data, const infra::Function<void()>& onDone)
    {
        really_assert(data.size() <= 256);

        this->address = address;

        AddCommandAction<TransmitWithTwosComplementChecksum>(writeMemory);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitChecksummedBuffer>(infra::MakeConstByteRange(this->address));
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitSmallBuffer>(data);
        AddCommandAction<ReceiveAck>();

        SetCommandTimeout("Timeout writing memory");
        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::MassErase(const infra::Function<void()>& onDone)
    {
        constexpr uint8_t globalEraseSubCommand = 0xff;

        AddCommandAction<TransmitWithTwosComplementChecksum>(erase);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitWithTwosComplementChecksum>(globalEraseSubCommand);
        AddCommandAction<ReceiveAck>();

        SetCommandTimeout("Timeout erasing memory");
        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::Erase(infra::ConstByteRange pages, const infra::Function<void()>& onDone)
    {
        really_assert(pages.size() <= 255);

        AddCommandAction<TransmitWithTwosComplementChecksum>(erase);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitSmallBuffer>(pages);
        AddCommandAction<ReceiveAck>();

        SetCommandTimeout("Timeout erasing memory");
        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::ExtendedMassErase(MassEraseSubcommand subcommand, const infra::Function<void()>& onDone)
    {
        this->subcommand = static_cast<uint16_t>(subcommand);

        AddCommandAction<TransmitWithTwosComplementChecksum>(extendedErase);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitChecksummedBuffer>(infra::MakeConstByteRange(this->subcommand));
        AddCommandAction<ReceiveAck>();

        SetCommandTimeout("Timeout extended erasing memory");
        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::ExtendedErase(infra::ConstByteRange pages, const infra::Function<void()>& onDone)
    {
        really_assert(pages.size() <= 256);
        really_assert(pages.size() % 2 == 0);

        AddCommandAction<TransmitWithTwosComplementChecksum>(extendedErase);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitBigBuffer>((pages.size() / 2) - 1, pages);
        AddCommandAction<ReceiveAck>();

        SetCommandTimeout("Timeout extended erasing memory");
        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::Special(uint16_t subcommand, infra::ConstByteRange txData, infra::ByteRange& rxData, infra::ByteRange& rxStatus, const infra::Function<void()>& onDone)
    {
        really_assert(txData.size() <= 128);

        this->subcommand = subcommand;

        AddCommandAction<TransmitWithTwosComplementChecksum>(special);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitChecksummedBuffer>(infra::MakeByteRange(this->subcommand));
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitBigBuffer>(txData.size(), txData);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<ReceiveBigBuffer>(rxData);
        AddCommandAction<ReceiveBigBuffer>(rxStatus);
        AddCommandAction<ReceiveAck>();

        SetCommandTimeout("Timeout special command");
        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::ExtendedSpecial(uint16_t subcommand, infra::ConstByteRange txData1, infra::ConstByteRange txData2, infra::ByteRange& rxData, const infra::Function<void()>& onDone)
    {
        really_assert(txData1.size() <= 128);
        really_assert(txData2.size() <= 1024);

        this->subcommand = subcommand;

        AddCommandAction<TransmitWithTwosComplementChecksum>(extendedSpecial);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitChecksummedBuffer>(infra::MakeByteRange(this->subcommand));
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitBigBuffer>(txData1.size(), txData1);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitBigBuffer>(txData2.size(), txData2);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<ReceiveBigBuffer>(rxData);
        AddCommandAction<ReceiveAck>();

        SetCommandTimeout("Timeout extended special command");
        ExecuteCommand(onDone);
    }

    void StUartBootloaderCommandHandler::InitializeUartBootloader()
    {
        AddCommandAction<TransmitRaw>(infra::MakeByteRange(uartInitializationData));
        AddCommandAction<ReceiveAck>();

        SetCommandTimeout("Timeout waiting for bootloader initialization");
        ExecuteCommand([this]()
            {
                onInitialized();
            });
    }

    template<class T, class... Args>
    void StUartBootloaderCommandHandler::AddCommandAction(Args&&... args)
    {
        commandActions.emplace_back(infra::InPlaceType<T>(), *this, std::forward<Args>(args)...);
    }

    void StUartBootloaderCommandHandler::ExecuteCommand(const infra::Function<void(), sizeof(StUartBootloaderCommandHandler*) + sizeof(infra::Function<void()>) + sizeof(infra::ByteRange)>& onCommandExecuted)
    {
        this->onCommandExecuted = onCommandExecuted;
        StartAction();
    }

    void StUartBootloaderCommandHandler::StartAction()
    {
        commandActions.front()->Start();
        TryHandleDataReceived();
    }

    void StUartBootloaderCommandHandler::TryHandleDataReceived()
    {
        if (!queue.Empty())
            commandActions.front()->DataReceived();
    }

    void StUartBootloaderCommandHandler::OnCommandExecuted()
    {
        timeout.Cancel();
        onCommandExecuted();
    }

    void StUartBootloaderCommandHandler::OnActionExecuted()
    {
        commandActions.pop_front();

        if (commandActions.empty())
            OnCommandExecuted();
        else
            StartAction();
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
                OnActionExecuted();
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

    StUartBootloaderCommandHandler::Action::Action(StUartBootloaderCommandHandler& handler)
        : handler(handler)
    {}

    void StUartBootloaderCommandHandler::Action::Start()
    {}

    void StUartBootloaderCommandHandler::Action::DataReceived()
    {}

    void StUartBootloaderCommandHandler::ReceiveAck::DataReceived()
    {
        auto byte = handler.queue.Get();

        if (byte == ack)
            handler.OnActionExecuted();
        else
            handler.OnError("NACK received");
    }

    StUartBootloaderCommandHandler::ReceiveBuffer::ReceiveBuffer(StUartBootloaderCommandHandler& handler, infra::ByteRange& data)
        : StUartBootloaderCommandHandler::Action(handler)
        , data(data)
    {}

    void StUartBootloaderCommandHandler::ReceiveBuffer::DataReceived()
    {
        infra::QueueForOneReaderOneIrqWriter<uint8_t>::StreamReader reader(handler.queue);
        infra::DataInputStream::WithErrorPolicy stream(reader, infra::noFail);

        if (nBytesTotal == infra::none)
        {
            TryExtractNumberOfBytes(stream);
            if (nBytesTotal == infra::none)
                return;
        }

        auto buffer = infra::Head(infra::DiscardHead(data, nBytesReceived), std::min(stream.Available(), *nBytesTotal - nBytesReceived));
        stream >> buffer;
        reader.Commit();

        nBytesReceived += buffer.size();
        if (nBytesReceived == nBytesTotal)
        {
            data.shrink_from_back_to(nBytesReceived);
            handler.OnActionExecuted();
        }
    }

    StUartBootloaderCommandHandler::ReceivePredefinedBuffer::ReceivePredefinedBuffer(StUartBootloaderCommandHandler& handler, infra::ByteRange& data, const std::size_t size)
        : StUartBootloaderCommandHandler::ReceiveBuffer(handler, data)
        , size(size)
    {}

    void StUartBootloaderCommandHandler::ReceivePredefinedBuffer::TryExtractNumberOfBytes([[maybe_unused]] infra::DataInputStream& stream)
    {
        nBytesTotal.Emplace(size);
    }

    void StUartBootloaderCommandHandler::ReceiveSmallBuffer::TryExtractNumberOfBytes(infra::DataInputStream& stream)
    {
        nBytesTotal.Emplace(stream.Extract<uint8_t>() + 1);
    }

    void StUartBootloaderCommandHandler::ReceiveBigBuffer::TryExtractNumberOfBytes(infra::DataInputStream& stream)
    {
        if (stream.Available() >= sizeof(uint16_t))
            nBytesTotal.Emplace(stream.Extract<infra::BigEndian<uint16_t>>());
    }

    StUartBootloaderCommandHandler::TransmitRaw::TransmitRaw(StUartBootloaderCommandHandler& handler, infra::ConstByteRange data)
        : StUartBootloaderCommandHandler::Action(handler)
        , data(data)
    {}

    void StUartBootloaderCommandHandler::TransmitRaw::Start()
    {
        handler.SendData(data);
    }

    StUartBootloaderCommandHandler::TransmitWithTwosComplementChecksum::TransmitWithTwosComplementChecksum(StUartBootloaderCommandHandler& handler, uint8_t data)
        : StUartBootloaderCommandHandler::Action(handler)
        , data(data)
    {}

    void StUartBootloaderCommandHandler::TransmitWithTwosComplementChecksum::Start()
    {
        auto checksum = data ^ 0xff;
        handler.SendData(infra::MakeConstByteRange(data), checksum);
    }

    StUartBootloaderCommandHandler::TransmitChecksummedBuffer::TransmitChecksummedBuffer(StUartBootloaderCommandHandler& handler, infra::ConstByteRange data)
        : StUartBootloaderCommandHandler::Action(handler)
        , data(data)
    {
        for (auto& b : data)
            AddToChecksum(b);
    }

    void StUartBootloaderCommandHandler::TransmitChecksummedBuffer::AddToChecksum(const uint8_t& byte)
    {
        checksum ^= byte;
    }

    void StUartBootloaderCommandHandler::TransmitChecksummedBuffer::Start()
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

    void StUartBootloaderCommandHandler::TransmitSmallBuffer::Start()
    {
        handler.serial.SendData(infra::MakeConstByteRange(size), [this]()
            {
                StUartBootloaderCommandHandler::TransmitChecksummedBuffer::Start();
            });
    }

    StUartBootloaderCommandHandler::TransmitBigBuffer::TransmitBigBuffer(StUartBootloaderCommandHandler& handler, uint16_t size, infra::ConstByteRange data)
        : StUartBootloaderCommandHandler::TransmitChecksummedBuffer(handler, data)
        , size(size)
    {
        AddToChecksum(static_cast<uint8_t>(size >> 8));
        AddToChecksum(static_cast<uint8_t>(size & 0xff));
    }

    void StUartBootloaderCommandHandler::TransmitBigBuffer::Start()
    {
        handler.serial.SendData(infra::MakeConstByteRange(size), [this]()
            {
                StUartBootloaderCommandHandler::TransmitChecksummedBuffer::Start();
            });
    }
}
