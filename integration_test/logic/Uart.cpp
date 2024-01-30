#include "integration_test/logic/Uart.hpp"

namespace application
{
    UartBase::UartBase(services::Echo& echo, hal::BufferedSerialCommunication& uart)
        : testing::UartObserverProxy(echo)
        , hal::BufferedSerialCommunicationObserver(uart)
    {}

    void UartBase::SendData(const infra::ConstByteRange& data, const infra::Function<void()>& onDone)
    {
        Subject().SendData(data, onDone);
    }

    void UartBase::DataReceived()
    {
        auto& reader = Subject().Reader();
        while (!reader.Empty())
        {
            auto range = infra::Head(reader.ExtractContiguousRange(std::numeric_limits<std::size_t>::max()), receivedData.max_size() - receivedData.size());
            receivedData.insert(receivedData.end(), range.begin(), range.end());
        }

        Subject().AckReceived();

        if (!std::exchange(sending, true))
            RequestSend([this]()
                {
                    sending = false;
                    ReceivedData(infra::MakeRange(receivedData));
                    receivedData.clear();
                });
    }

    UartTester::UartTester(services::Echo& echo, hal::BufferedSerialCommunication& uart)
        : UartBase(echo, uart)
        , testing::UartTester(echo)
    {}

    void UartTester::SendData(infra::ConstByteRange data)
    {
        UartBase::SendData(data, [this]()
            {
                MethodDone();
            });
    }

    void UartTester::ReceivedData(infra::ConstByteRange data)
    {
        TesterReceivedData(data);
    }

    UartTested::UartTested(services::Echo& echo, hal::BufferedSerialCommunication& uart)
        : UartBase(echo, uart)
        , testing::UartTested(echo)
    {}

    void UartTested::SendData(infra::ConstByteRange data)
    {
        UartBase::SendData(data, [this]()
            {
                MethodDone();
            });
    }

    void UartTested::ReceivedData(infra::ConstByteRange data)
    {
        TestedReceivedData(data);
    }
}
