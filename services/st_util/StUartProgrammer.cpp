#include "services/st_util/StUartProgrammer.hpp"
#include "infra/stream/InputStream.hpp"

namespace services
{
    StUartProgrammer::StUartProgrammer(hal::SerialCommunication& serial, const infra::Function<void()>& onInitialized, const infra::Function<void(infra::BoundedConstString reason)>& onError)
        : serial(serial)
        , onInitialized(onInitialized)
        , onError(onError)
        , queue([this]()
              {
                  infra::QueueForOneReaderOneIrqWriter<uint8_t>::StreamReader reader(queue);
                  auto range = reader.PeekContiguousRange(0);
                  DataReceived(reader);
              })
    {
        serial.ReceiveData([this](auto data)
            {
                queue.AddFromInterrupt(data);
            });

        SendHello();
    }

    StUartProgrammer::~StUartProgrammer()
    {
        serial.ReceiveData([](auto data) {});
    }

    void StUartProgrammer::GetVersion(const infra::Function<void(uint8_t major, uint8_t minor)>& onDone)
    {
        timeout.Start(std::chrono::seconds(1), [this]()
            {
                OnError("Timeout getting version");
            });

        data.resize(4);
        CommandReceiveData(0x01, infra::MakeRange(data), [this, onDone]()
            {
                timeout.Cancel();
                onDone(data[0] >> 4, data[0] & 0xf);
            });
    }

    void StUartProgrammer::GetId(const infra::Function<void(uint16_t id)>& onDone)
    {
        timeout.Start(std::chrono::seconds(1), [this]()
            {
                OnError("Timeout getting id");
            });

        data.resize(4);
        CommandReceiveData(0x02, infra::MakeRange(data), [this, onDone]()
            {
                timeout.Cancel();
                onDone((data[1] << 8) | data[2]);
            });
    }

    void StUartProgrammer::ReadMemory(uint32_t address, infra::ByteRange data, const infra::Function<void()>& onDone)
    {
        timeout.Start(std::chrono::seconds(1), [this]()
            {
                OnError("Timeout reading memory");
            });

        this->address = address;
        CommandSendDataSendSingleDataReceiveData(0x11, infra::MakeByteRange(this->address), static_cast<uint8_t>(data.size() - 1), data, onDone);
    }

    void StUartProgrammer::WriteMemory(uint32_t address, infra::ConstByteRange data, const infra::Function<void()>& onDone)
    {
        timeout.Start(std::chrono::seconds(1), [this]()
            {
                OnError("Timeout writing memory");
            });

        this->address = address;
        this->data.clear();
        this->data.push_back(static_cast<uint8_t>(data.size() - 1));
        this->data.insert(this->data.end(), data.begin(), data.end());
        CommandSendDataSendData(0x31, infra::MakeByteRange(this->address), infra::MakeRange(this->data), onDone);
    }

    void StUartProgrammer::EraseAll(const infra::Function<void()>& onDone)
    {
        timeout.Start(std::chrono::seconds(60), [this]()
            {
                OnError("Timeout erasing chip");
            });

        data.clear();
        data.push_back(0xff);
        data.push_back(0xff);
        CommandSendData(0x44, infra::MakeRange(data), onDone);
    }

    void StUartProgrammer::ErasePage(uint16_t page, const infra::Function<void()>& onDone)
    {
        timeout.Start(std::chrono::minutes(1), [this]()
            {
                OnError("Timeout erasing page");
            });

        data.clear();
        data.push_back(0);
        data.push_back(0);
        data.push_back(page >> 8);
        data.push_back(page & 0xff);
        CommandSendData(0x44, infra::MakeRange(data), onDone);
    }

    void StUartProgrammer::SendHello()
    {
        static const uint8_t hello = 0x7f;

        onResponse = [this]()
        {
            startClaimer.Release();
            timeout.Cancel();
            onInitialized();
        };

        startClaimer.Claim([this]()
            {
                timeout.Start(std::chrono::milliseconds(30), [this]()
                    {
                        timeout.Start(std::chrono::seconds(1), [this]()
                            {
                                OnError("Timeout waiting for hello");
                            });
                        serial.SendData(infra::MakeByteRange(hello), [this]() {});
                    });
            });
    }

    void StUartProgrammer::CommandSendData(uint8_t command, infra::ConstByteRange data, const infra::Function<void()>& onDone)
    {
        this->onDone = onDone;
        claimer.Claim([this, command, data]()
            {
                onResponse = [this, data]()
                {
                    onResponse = [this]()
                    {
                        timeout.Cancel();
                        this->onDone();
                    };

                    RawSendData(data);
                };

                RawSendCommand(command);
            });
    }

    void StUartProgrammer::CommandSendDataSendData(uint8_t command, infra::ConstByteRange data, infra::ConstByteRange data2, const infra::Function<void()>& onDone)
    {
        this->onDone = onDone;
        claimer.Claim([this, command, data, data2]()
            {
                onResponse = [this, data, data2]()
                {
                    onResponse = [this, data2]()
                    {
                        onResponse = [this]()
                        {
                            timeout.Cancel();
                            this->onDone();
                        };

                        RawSendData(data2);
                    };

                    RawSendData(data);
                };

                RawSendCommand(command);
            });
    }

    void StUartProgrammer::CommandReceiveData(uint8_t command, infra::ByteRange receiveData, const infra::Function<void(), sizeof(StUartProgrammer*) + sizeof(infra::Function<void()>)>& onDone)
    {
        this->onDone = onDone;
        claimer.Claim([this, command, receiveData]()
            {
                this->receiveData = receiveData;

                onResponse = [this]()
                {
                    timeout.Cancel();
                    this->onDone();
                };

                RawSendCommand(command);
            });
    }

    void StUartProgrammer::CommandSendDataSendSingleDataReceiveData(uint8_t command, infra::ConstByteRange sendData, uint8_t sendData2, infra::ByteRange receiveData, const infra::Function<void(), sizeof(infra::Function<void()>)>& onDone)
    {
        this->onDone = onDone;
        claimer.Claim([this, command, sendData, sendData2, receiveData]()
            {
                onResponse = [this, sendData, sendData2, receiveData]()
                {
                    onResponse = [this, sendData2, receiveData]()
                    {
                        this->receiveData = receiveData;

                        onResponse = [this]()
                        {
                            timeout.Cancel();
                            this->onDone();
                        };

                        RawSendSingleData(sendData2);
                    };

                    RawSendData(sendData);
                };

                RawSendCommand(command);
            });
    }

    void StUartProgrammer::RawSendCommand(uint8_t command)
    {
        commandBytes[0] = command;
        commandBytes[1] = command ^ 0xff;

        serial.SendData(infra::MakeByteRange(commandBytes), [this]()
            {
                claimer.Release();
            });
    }

    void StUartProgrammer::RawSendSingleData(uint8_t data)
    {
        claimer.Claim([this, data]()
            {
                commandBytes[0] = data;
                commandBytes[1] = data ^ 0xff;

                serial.SendData(infra::MakeByteRange(commandBytes), [this]()
                    {
                        claimer.Release();
                    });
            });
    }

    void StUartProgrammer::RawSendData(infra::ConstByteRange data)
    {
        claimer.Claim([this, data]()
            {
                checksum = 0;
                for (auto b : data)
                    checksum ^= b;

                serial.SendData(data, [this]()
                    {
                        serial.SendData(infra::MakeByteRange(checksum), [this]()
                            {
                                claimer.Release();
                            });
                    });
            });
    }

    void StUartProgrammer::DataReceived(infra::QueueForOneReaderOneIrqWriter<uint8_t>::StreamReader& reader)
    {
        infra::DataInputStream::WithErrorPolicy stream(reader, infra::noFail);

        auto first = stream.Extract<uint8_t>();

        while (first != 0x79 && first != 0x1f)
        {
            reader.Commit();
            first = stream.Extract<uint8_t>();

            if (stream.Failed())
                return;
        }

        if (first != 0x79)
            OnError("NACK received");
        else if (!stream.Failed() && stream.Available() >= receiveData.size())
        {
            stream >> receiveData;

            reader.Commit();
            onResponse();
        }
    }

    void StUartProgrammer::OnError(infra::BoundedConstString reason)
    {
        timeout.Cancel();
        serial.ReceiveData([](auto data) {});
        onError(reason);
    }
}
