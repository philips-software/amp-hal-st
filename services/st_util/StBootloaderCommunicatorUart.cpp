#include "services/st_util/StBootloaderCommunicatorUart.hpp"
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

    constexpr uint8_t ack = 0x79;
    constexpr uint8_t nack = 0x1f;
}

namespace services
{
    StBootloaderCommunicatorUart::StBootloaderCommunicatorUart(hal::SerialCommunication& serial, const infra::Function<void()>& onInitialized, const infra::Function<void(StBootloaderCommunicatorError error)>& onError)
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

    StBootloaderCommunicatorUart::~StBootloaderCommunicatorUart()
    {
        serial.ReceiveData([](auto data) {});
    }

    void StBootloaderCommunicatorUart::Stop()
    {
        serial.ReceiveData([](auto data) {});
    }

    void StBootloaderCommunicatorUart::GetCommand(infra::ByteRange& commands, const infra::Function<void(uint8_t major, uint8_t minor)>& onDone)
    {
        really_assert(commandActions.empty());
        really_assert(commands.size() >= 14);

        AddCommandAction<TransmitWithTwosComplementChecksum>(getCommand);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<ReceiveSmallBuffer>(commands);
        AddCommandAction<ReceiveAck>();

        SetCommandTimeout(StBootloaderCommunicatorError::commandtimeout);
        ExecuteCommand([onDone, &commands]()
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

    void StBootloaderCommunicatorUart::GetVersion(const infra::Function<void(uint8_t major, uint8_t minor)>& onDone)
    {
        really_assert(commandActions.empty());
        AddCommandAction<TransmitWithTwosComplementChecksum>(getVersion);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<ReceivePredefinedBuffer>(internalRange, 3);
        AddCommandAction<ReceiveAck>();

        SetCommandTimeout(StBootloaderCommunicatorError::commandtimeout);
        ExecuteCommand([this, onDone]()
            {
                uint8_t major = internalBuffer[0] >> 4;
                uint8_t minor = internalBuffer[0] & 0xf;
                onDone(major, minor);
            });
    }

    void StBootloaderCommunicatorUart::GetId(const infra::Function<void(uint16_t id)>& onDone)
    {
        really_assert(commandActions.empty());
        AddCommandAction<TransmitWithTwosComplementChecksum>(getId);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<ReceiveSmallBuffer>(internalRange);
        AddCommandAction<ReceiveAck>();

        SetCommandTimeout(StBootloaderCommunicatorError::commandtimeout);
        ExecuteCommand([this, onDone]()
            {
                uint16_t id = (internalBuffer[0] << 8) | internalBuffer[1];
                onDone(id);
            });
    }

    void StBootloaderCommunicatorUart::ReadMemory(uint32_t address, infra::ByteRange& data, const infra::Function<void()>& onDone)
    {
        really_assert(commandActions.empty());
        really_assert(data.size() <= 256);
        really_assert(data.size() > 0);

        this->address = address;

        AddCommandAction<TransmitWithTwosComplementChecksum>(readMemory);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitChecksummedBuffer>(infra::MakeConstByteRange(this->address));
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitWithTwosComplementChecksum>(data.size() - 1);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<ReceivePredefinedBuffer>(data, data.size());

        SetCommandTimeout(StBootloaderCommunicatorError::commandtimeout);
        ExecuteCommand(onDone);
    }

    void StBootloaderCommunicatorUart::Go(uint32_t address, const infra::Function<void()>& onDone)
    {
        really_assert(commandActions.empty());
        this->address = address;

        AddCommandAction<TransmitWithTwosComplementChecksum>(go);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitChecksummedBuffer>(infra::MakeConstByteRange(this->address));
        AddCommandAction<ReceiveAck>();

        SetCommandTimeout(StBootloaderCommunicatorError::commandtimeout);
        ExecuteCommand(onDone);
    }

    void StBootloaderCommunicatorUart::WriteMemory(uint32_t address, infra::ConstByteRange data, const infra::Function<void()>& onDone)
    {
        really_assert(commandActions.empty());
        really_assert(data.size() <= 256);
        really_assert(data.size() > 0);
        really_assert(data.size() % 4 == 0);

        this->address = address;

        AddCommandAction<TransmitWithTwosComplementChecksum>(writeMemory);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitChecksummedBuffer>(infra::MakeConstByteRange(this->address));
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitSmallBuffer>(data);
        AddCommandAction<ReceiveAck>();

        SetCommandTimeout(StBootloaderCommunicatorError::commandtimeout);
        ExecuteCommand(onDone);
    }

    void StBootloaderCommunicatorUart::MassErase(const infra::Function<void()>& onDone)
    {
        really_assert(commandActions.empty());
        constexpr uint8_t globalEraseSubCommand = 0xff;

        AddCommandAction<TransmitWithTwosComplementChecksum>(erase);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitWithTwosComplementChecksum>(globalEraseSubCommand);
        AddCommandAction<ReceiveAck>();

        SetCommandTimeout(StBootloaderCommunicatorError::commandtimeout);
        ExecuteCommand(onDone);
    }

    void StBootloaderCommunicatorUart::Erase(infra::ConstByteRange pages, const infra::Function<void()>& onDone)
    {
        really_assert(commandActions.empty());
        really_assert(pages.size() <= 255);
        really_assert(pages.size() > 0);

        AddCommandAction<TransmitWithTwosComplementChecksum>(erase);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitSmallBuffer>(pages);
        AddCommandAction<ReceiveAck>();

        SetCommandTimeout(StBootloaderCommunicatorError::commandtimeout);
        ExecuteCommand(onDone);
    }

    void StBootloaderCommunicatorUart::ExtendedMassErase(MassEraseSubcommand subcommand, const infra::Function<void()>& onDone)
    {
        really_assert(commandActions.empty());
        this->subcommand = static_cast<uint16_t>(subcommand);

        AddCommandAction<TransmitWithTwosComplementChecksum>(extendedErase);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitChecksummedBuffer>(infra::MakeConstByteRange(this->subcommand));
        AddCommandAction<ReceiveAck>();

        SetCommandTimeout(StBootloaderCommunicatorError::commandtimeout);
        ExecuteCommand(onDone);
    }

    void StBootloaderCommunicatorUart::ExtendedErase(const infra::MemoryRange<infra::BigEndian<uint16_t>> pages, const infra::Function<void()>& onDone)
    {
        really_assert(commandActions.empty());
        really_assert(pages.size() > 0);

        AddCommandAction<TransmitWithTwosComplementChecksum>(extendedErase);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitBigBuffer>(infra::ReinterpretCastByteRange(pages), pages.size() - 1);
        AddCommandAction<ReceiveAck>();

        SetCommandTimeout(StBootloaderCommunicatorError::commandtimeout);
        ExecuteCommand(onDone);
    }

    void StBootloaderCommunicatorUart::Special(uint16_t subcommand, infra::ConstByteRange txData, infra::ByteRange& rxData, infra::ByteRange& rxStatus, const infra::Function<void()>& onDone)
    {
        really_assert(commandActions.empty());
        really_assert(txData.size() <= 128);

        this->subcommand = subcommand;

        AddCommandAction<TransmitWithTwosComplementChecksum>(special);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitChecksummedBuffer>(infra::MakeByteRange(this->subcommand));
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitBigBuffer>(txData, txData.size());
        AddCommandAction<ReceiveAck>();
        AddCommandAction<ReceiveBigBuffer>(rxData);
        AddCommandAction<ReceiveBigBuffer>(rxStatus);
        AddCommandAction<ReceiveAck>();

        SetCommandTimeout(StBootloaderCommunicatorError::commandtimeout);
        ExecuteCommand(onDone);
    }

    void StBootloaderCommunicatorUart::ExtendedSpecial(uint16_t subcommand, infra::ConstByteRange txData1, infra::ConstByteRange txData2, infra::ByteRange& rxData, const infra::Function<void()>& onDone)
    {
        really_assert(commandActions.empty());
        really_assert(txData1.size() <= 128);
        really_assert(txData2.size() <= 1024);

        this->subcommand = subcommand;

        AddCommandAction<TransmitWithTwosComplementChecksum>(extendedSpecial);
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitChecksummedBuffer>(infra::MakeByteRange(this->subcommand));
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitBigBuffer>(txData1, txData1.size());
        AddCommandAction<ReceiveAck>();
        AddCommandAction<TransmitBigBuffer>(txData2, txData2.size());
        AddCommandAction<ReceiveAck>();
        AddCommandAction<ReceiveBigBuffer>(rxData);
        AddCommandAction<ReceiveAck>();

        SetCommandTimeout(StBootloaderCommunicatorError::commandtimeout);
        ExecuteCommand(onDone);
    }

    void StBootloaderCommunicatorUart::InitializeUartBootloader()
    {
        really_assert(commandActions.empty());
        AddCommandAction<TransmitRaw>(infra::MakeByteRange(uartInitializationData));
        AddCommandAction<ReceiveAck>();

        SetInitializationTimeoutWithRetry();
        ExecuteCommand([this]()
            {
                initRetryCounter = 0;
                onInitialized();
            });
    }

    template<class T, class... Args>
    void StBootloaderCommunicatorUart::AddCommandAction(Args&&... args)
    {
        commandActions.emplace_back(infra::InPlaceType<T>(), *this, std::forward<Args>(args)...);
    }

    void StBootloaderCommunicatorUart::ExecuteCommand(const infra::Function<void(), sizeof(StBootloaderCommunicatorUart*) + sizeof(infra::Function<void()>) + sizeof(infra::ByteRange)>& onCommandExecuted)
    {
        this->onCommandExecuted = onCommandExecuted;
        StartCurrentAction();
    }

    void StBootloaderCommunicatorUart::StartCurrentAction()
    {
        commandActions.front()->Start();
        TryHandleDataReceived();
    }

    void StBootloaderCommunicatorUart::TryHandleDataReceived()
    {
        if (!queue.Empty() && !commandActions.empty())
            commandActions.front()->DataReceived();
    }

    void StBootloaderCommunicatorUart::OnCommandExecuted()
    {
        timeout.Cancel();
        onCommandExecuted();
    }

    void StBootloaderCommunicatorUart::OnActionExecuted()
    {
        commandActions.pop_front();

        if (commandActions.empty())
            OnCommandExecuted();
        else
            StartCurrentAction();
    }

    void StBootloaderCommunicatorUart::SetCommandTimeout(StBootloaderCommunicatorError error)
    {
        timeoutError = error;
        timeout.Start(commandTimeout, [this]()
            {
                OnError(timeoutError);
            });
    }

    void StBootloaderCommunicatorUart::SetInitializationTimeoutWithRetry()
    {
        timeout.Start(commandTimeout, [this]()
            {
                if (++initRetryCounter < 10)
                {
                    commandActions.clear();
                    InitializeUartBootloader();
                }
                else
                    OnError(StBootloaderCommunicatorError::initializationTimeout);
            });
    }

    void StBootloaderCommunicatorUart::OnError(StBootloaderCommunicatorError error)
    {
        timeout.Cancel();
        serial.ReceiveData([](auto data) {});
        if (onError != nullptr)
            onError(error);
    }

    void StBootloaderCommunicatorUart::SendData(infra::ConstByteRange data, uint8_t checksum)
    {
        this->checksum = checksum;
        serial.SendData(data, [this]()
            {
                SendData(infra::MakeConstByteRange(this->checksum));
            });
    }

    void StBootloaderCommunicatorUart::SendData(infra::ConstByteRange data)
    {
        serial.SendData(data, [this]()
            {
                OnActionExecuted();
            });
    }

    StBootloaderCommunicatorUart::Action::Action(StBootloaderCommunicatorUart& handler)
        : handler(handler)
    {}

    void StBootloaderCommunicatorUart::Action::Start()
    {}

    void StBootloaderCommunicatorUart::Action::DataReceived()
    {}

    void StBootloaderCommunicatorUart::ReceiveAck::DataReceived()
    {
        while (!handler.queue.Empty())
        {
            auto byte = handler.queue.Get();
            if (byte == ack)
            {
                handler.OnActionExecuted();
                return;
            }

            if (byte == nack)
            {
                handler.OnError(StBootloaderCommunicatorError::nackReceived);
                return;
            }
        }
    }

    StBootloaderCommunicatorUart::ReceiveBuffer::ReceiveBuffer(StBootloaderCommunicatorUart& handler, infra::ByteRange& data)
        : StBootloaderCommunicatorUart::Action(handler)
        , data(data)
    {}

    void StBootloaderCommunicatorUart::ReceiveBuffer::DataReceived()
    {
        infra::QueueForOneReaderOneIrqWriter<uint8_t>::StreamReader reader(handler.queue);
        infra::DataInputStream::WithErrorPolicy stream(reader, infra::noFail);

        if (!TotalNumberOfBytesAvailable(stream))
            return;

        RetreiveData(reader, stream);
        CheckActionExecuted();
    }

    bool StBootloaderCommunicatorUart::ReceiveBuffer::TotalNumberOfBytesAvailable(infra::DataInputStream& stream)
    {
        if (nBytesTotal == infra::none)
            TryRetreiveNumberOfBytes(stream);

        return nBytesTotal != infra::none;
    }

    void StBootloaderCommunicatorUart::ReceiveBuffer::RetreiveData(infra::QueueForOneReaderOneIrqWriter<uint8_t>::StreamReader& reader, infra::DataInputStream& stream)
    {
        auto buffer = infra::Head(infra::DiscardHead(data, nBytesReceived), std::min(stream.Available(), *nBytesTotal - nBytesReceived));
        stream >> buffer;
        reader.Commit();
        nBytesReceived += buffer.size();
    }

    void StBootloaderCommunicatorUart::ReceiveBuffer::CheckActionExecuted()
    {
        if (nBytesReceived == nBytesTotal)
        {
            data.shrink_from_back_to(nBytesReceived);
            handler.OnActionExecuted();
        }
    }

    StBootloaderCommunicatorUart::ReceivePredefinedBuffer::ReceivePredefinedBuffer(StBootloaderCommunicatorUart& handler, infra::ByteRange& data, const std::size_t size)
        : StBootloaderCommunicatorUart::ReceiveBuffer(handler, data)
        , size(size)
    {}

    void StBootloaderCommunicatorUart::ReceivePredefinedBuffer::TryRetreiveNumberOfBytes([[maybe_unused]] infra::DataInputStream& stream)
    {
        nBytesTotal.Emplace(size);
    }

    void StBootloaderCommunicatorUart::ReceiveSmallBuffer::TryRetreiveNumberOfBytes(infra::DataInputStream& stream)
    {
        nBytesTotal.Emplace(stream.Extract<uint8_t>() + 1);
    }

    void StBootloaderCommunicatorUart::ReceiveBigBuffer::TryRetreiveNumberOfBytes(infra::DataInputStream& stream)
    {
        if (stream.Available() >= sizeof(uint16_t))
            nBytesTotal.Emplace(stream.Extract<infra::BigEndian<uint16_t>>());
    }

    StBootloaderCommunicatorUart::TransmitRaw::TransmitRaw(StBootloaderCommunicatorUart& handler, infra::ConstByteRange data)
        : StBootloaderCommunicatorUart::Action(handler)
        , data(data)
    {}

    void StBootloaderCommunicatorUart::TransmitRaw::Start()
    {
        handler.SendData(data);
    }

    StBootloaderCommunicatorUart::TransmitWithTwosComplementChecksum::TransmitWithTwosComplementChecksum(StBootloaderCommunicatorUart& handler, uint8_t data)
        : StBootloaderCommunicatorUart::Action(handler)
        , data(data)
    {}

    void StBootloaderCommunicatorUart::TransmitWithTwosComplementChecksum::Start()
    {
        checksum = data ^ 0xff;
        handler.SendData(infra::MakeConstByteRange(data), checksum);
    }

    StBootloaderCommunicatorUart::TransmitChecksummedBuffer::TransmitChecksummedBuffer(StBootloaderCommunicatorUart& handler, infra::ConstByteRange data)
        : StBootloaderCommunicatorUart::Action(handler)
        , data(data)
    {
        for (auto& byte : data)
            AddToChecksum(byte);
    }

    void StBootloaderCommunicatorUart::TransmitChecksummedBuffer::AddToChecksum(const uint8_t& byte)
    {
        checksum ^= byte;
    }

    void StBootloaderCommunicatorUart::TransmitChecksummedBuffer::Start()
    {
        if (data.empty())
            handler.SendData(infra::MakeConstByteRange(checksum));
        else
            handler.SendData(data, checksum);
    }

    StBootloaderCommunicatorUart::TransmitSmallBuffer::TransmitSmallBuffer(StBootloaderCommunicatorUart& handler, infra::ConstByteRange data)
        : StBootloaderCommunicatorUart::TransmitChecksummedBuffer(handler, data)
    {
        size = data.size() - 1;
        AddToChecksum(size);
    }

    void StBootloaderCommunicatorUart::TransmitSmallBuffer::Start()
    {
        handler.serial.SendData(infra::MakeConstByteRange(size), [this]()
            {
                StBootloaderCommunicatorUart::TransmitChecksummedBuffer::Start();
            });
    }

    StBootloaderCommunicatorUart::TransmitBigBuffer::TransmitBigBuffer(StBootloaderCommunicatorUart& handler, infra::ConstByteRange data, uint16_t size)
        : StBootloaderCommunicatorUart::TransmitChecksummedBuffer(handler, data)
        , size(size)
    {
        AddToChecksum(static_cast<uint8_t>(size >> 8));
        AddToChecksum(static_cast<uint8_t>(size & 0xff));
    }

    void StBootloaderCommunicatorUart::TransmitBigBuffer::Start()
    {
        handler.serial.SendData(infra::MakeConstByteRange(size), [this]()
            {
                StBootloaderCommunicatorUart::TransmitChecksummedBuffer::Start();
            });
    }
}
