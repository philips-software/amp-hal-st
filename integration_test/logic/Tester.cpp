#include "integration_test/logic/Tester.hpp"

namespace application
{
    Tester::Tester(services::Echo& echo, hal::GpioPin& resetTestedPin, EchoToTestedCreator& echoToTestedCreator, FlashTestedCreator& flashTestedCreator)
        : testing::Tester(echo)
        , echoToTestedCreator(echoToTestedCreator)
        , resetTested(resetTestedPin, true)
        , testerObserverProxy(echo)
        , flashTestedCreator(flashTestedCreator)
    {}

    void Tester::SetTestedMode(testing::TestedMode mode)
    {
        switch (mode)
        {
            case testing::TestedMode::reset:
                resetTested.Set(false);
                flash = infra::none;
                Peripherals::Reset();
                echoToTested = infra::none;
                testerObserverProxy.RequestSend([this]()
                    {
                        testerObserverProxy.TestedModeSet({});
                        MethodDone();
                    });
                break;
            case testing::TestedMode::programming:
                assert(!resetTested.GetOutputLatch());
                flash.Emplace(flashTestedCreator, Rpc(), [this](infra::BoundedConstString result)
                    {
                        this->result = result;
                        testerObserverProxy.RequestSend([this]()
                            {
                                testerObserverProxy.TestedModeSet(this->result);
                                MethodDone();
                            });
                    });
                resetTested.Set(true);
                break;
            case testing::TestedMode::active:
                assert(!resetTested.GetOutputLatch());
                echoToTested.Emplace(echoToTestedCreator);
                resetTested.Set(true);
                testerObserverProxy.RequestSend([this]()
                    {
                        testerObserverProxy.TestedModeSet({});
                        MethodDone();
                    });
                break;
            default:
                std::abort();
        }
    }

    void Tester::EnablePeripheral(testing::Peripheral type)
    {
        Peripherals::EnablePeripheral(type);

        MethodDone();
    }

    services::Echo& Tester::GetEcho() const
    {
        return testing::Tester::Subject();
    }

    TesterObserver::TesterObserver(services::Echo& echo)
        : testing::TesterObserver(echo)
    {}

    void TesterObserver::TestedModeSet(infra::BoundedConstString message)
    {
        receivedTestedModeSet = true;
        MethodDone();
    }

    bool TesterObserver::ReceivedTestedModeSet()
    {
        return std::exchange(receivedTestedModeSet, false);
    }
}
