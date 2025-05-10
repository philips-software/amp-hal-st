#include "cucumber_cpp/library/Steps.hpp"
#include "generated/echo/Testing.pb.hpp"
#include "infra/timer/Waiting.hpp"
#include "integration_test/logic/Tested.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace
{
    class UartObserver
        : public testing::UartObserver
    {
    public:
        UartObserver(services::Echo& echo)
            : testing::UartObserver(echo)
        {}

        void TesterReceivedData(infra::ConstByteRange data) override
        {
            testerData.insert(testerData.end(), data.begin(), data.end());
            MethodDone();
        }

        void TestedReceivedData(infra::ConstByteRange data) override
        {
            testedData.insert(testedData.end(), data.begin(), data.end());
            MethodDone();
        }

        std::vector<uint8_t> testerData;
        std::vector<uint8_t> testedData;
    };
}

STEP("uart peripherals are enabled")
{
    EXPECT_TRUE(infra::WaitUntilDone([&](const infra::Function<void()>& done)
        {
            context.Emplace<UartObserver>(context.Get<services::Echo>());

            context.Get<testing::TesterProxy>().RequestSend([&]()
                {
                    context.Get<testing::TesterProxy>().EnablePeripheral(testing::Peripheral::uart);
                    done();
                });
        }));

    EXPECT_TRUE(infra::WaitUntilDone([&](const infra::Function<void()>& done)
        {
            context.Get<testing::TestedProxy>().RequestSend([&]()
                {
                    context.Get<testing::TestedProxy>().EnablePeripheral(testing::Peripheral::uart);
                    context.Get<testing::TestedProxy>().RequestSend([&]()
                        {
                            context.Get<testing::TestedProxy>().Ping();
                            done();
                        });
                });
        }));

    EXPECT_TRUE(infra::WaitFor([&]()
        {
            return context.Get<application::TestedObserver>().ReceivedPong();
        }));

    context.Get<UartObserver>().testerData.clear(); // Remove spurious 0 byte due to initialisation order

    context.Emplace<testing::UartTesterProxy>(context.Get<services::Echo>());
    context.Emplace<testing::UartTestedProxy>(context.Get<services::Echo>());
}

STEP("uart duplex dma peripherals are enabled")
{
    EXPECT_TRUE(infra::WaitUntilDone([&](const infra::Function<void()>& done)
        {
            context.Emplace<UartObserver>(context.Get<services::Echo>());

            context.Get<testing::TesterProxy>().RequestSend([&]()
                {
                    context.Get<testing::TesterProxy>().EnablePeripheral(testing::Peripheral::uartDuplexDma);
                    done();
                });
        }));

    EXPECT_TRUE(infra::WaitUntilDone([&](const infra::Function<void()>& done)
        {
            context.Get<testing::TestedProxy>().RequestSend([&]()
                {
                    context.Get<testing::TestedProxy>().EnablePeripheral(testing::Peripheral::uartDuplexDma);
                    context.Get<testing::TestedProxy>().RequestSend([&]()
                        {
                            context.Get<testing::TestedProxy>().Ping();
                            done();
                        });
                });
        }));

    EXPECT_TRUE(infra::WaitFor([&]()
        {
            return context.Get<application::TestedObserver>().ReceivedPong();
        }));

    context.Emplace<testing::UartTesterProxy>(context.Get<services::Echo>());
    context.Emplace<testing::UartTestedProxy>(context.Get<services::Echo>());
}

STEP("the tester sends UART data")
{
    static const infra::BoundedVector<uint8_t>::WithMaxSize<32> expectedData{ { 1, 2, 3, 4, 5, 6 } };

    EXPECT_TRUE(infra::WaitUntilDone([&](const infra::Function<void()>& done)
        {
            context.Get<testing::UartTesterProxy>().RequestSend([&]()
                {
                    context.Get<testing::UartTesterProxy>().SendData(expectedData.range());
                    done();
                });
        }));
}

STEP("the tested sends UART data")
{
    static const infra::BoundedVector<uint8_t>::WithMaxSize<32> expectedData{ { 1, 2, 3, 4, 5, 6 } };

    EXPECT_TRUE(infra::WaitUntilDone([&](const infra::Function<void()>& done)
        {
            context.Get<testing::UartTestedProxy>().RequestSend([&]()
                {
                    context.Get<testing::UartTestedProxy>().SendData(expectedData.range());
                    done();
                });
        }));
}

STEP("the tester sees UART data")
{
    static const std::vector<uint8_t> expectedData{ 1, 2, 3, 4, 5, 6 };

    EXPECT_TRUE(infra::WaitFor([&]()
        {
            return context.Get<UartObserver>().testerData == expectedData;
        }));
}

STEP("the tested sees UART data")
{
    static const std::vector<uint8_t> expectedData{ 1, 2, 3, 4, 5, 6 };

    EXPECT_TRUE(infra::WaitFor([&]()
        {
            return context.Get<UartObserver>().testedData == expectedData;
        }));
}
