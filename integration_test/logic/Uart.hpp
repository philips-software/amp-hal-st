#ifndef HAL_ST_INTEGRATION_TEST_UART_HPP
#define HAL_ST_INTEGRATION_TEST_UART_HPP

#include "generated/echo/Testing.pb.hpp"
#include "hal/interfaces/SerialCommunication.hpp"

namespace application
{
    class UartBase
        : public testing::UartObserverProxy
        , private hal::BufferedSerialCommunicationObserver
    {
    public:
        UartBase(services::Echo& echo, hal::BufferedSerialCommunication& uart);

        void SendData(const infra::ConstByteRange& data, const infra::Function<void()>& onDone);

    protected:
        virtual void ReceivedData(infra::ConstByteRange data) = 0;

    private:
        // Implementation of BufferedSerialCommunicationObserver
        void DataReceived() override;

    private:
        bool sending = false;
        infra::BoundedVector<uint8_t>::WithMaxSize<32> receivedData;
    };

    class UartTester
        : public UartBase
        , public testing::UartTester
    {
    public:
        UartTester(services::Echo& echo, hal::BufferedSerialCommunication& uart);

        // Implementation of UartTester
        void SendData(infra::ConstByteRange data) override;

    protected:
        virtual void ReceivedData(infra::ConstByteRange data) override;
    };

    class UartTested
        : public UartBase
        , public testing::UartTested
    {
    public:
        UartTested(services::Echo& echo, hal::BufferedSerialCommunication& uart);

        // Implementation of UartTested
        void SendData(infra::ConstByteRange data) override;

    protected:
        virtual void ReceivedData(infra::ConstByteRange data) override;
    };
}

#endif
