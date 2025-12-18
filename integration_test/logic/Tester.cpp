#include "integration_test/logic/Tester.hpp"
#include <chrono>

namespace application
{
    Tester::Tester(services::Echo& echo, hal::GpioPin& resetTestedPin, hal::GpioPin& boot0TestedPin, EchoToTestedCreator& echoToTestedCreator, FlashTestedCreator& flashTestedCreator)
        : testing::Tester(echo)
        , echoToTestedCreator(echoToTestedCreator)
        , resetTested(resetTestedPin)
        , boot0Tested(boot0TestedPin)
        , testerObserverProxy(echo)
        , flashTestedCreator(flashTestedCreator)
    {}

    void Tester::SetTestedMode(testing::TestedMode mode)
    {
        switch (mode)
        {
            case testing::TestedMode::reset:
                boot0Tested.SetAsInput();
                resetTested.Set(false);
                flash.reset();
                Peripherals::Reset();
                echoToTested.reset();
                testerObserverProxy.RequestSend([this]()
                    {
                        testerObserverProxy.TestedModeSet({});
                        MethodDone();
                    });
                break;
            case testing::TestedMode::programming:
                assert(!resetTested.GetOutputLatch());
                boot0Tested.Set(true);
                testedResetDelay.Start(std::chrono::milliseconds(1), [this]()
                    {
                        resetTested.Set(true);
                        testedResetDelay.Start(std::chrono::milliseconds(100), [this]()
                            {
                                resetTested.SetAsInput();
                                flash.emplace(flashTestedCreator, Rpc(), [this](infra::BoundedConstString result)
                                    {
                                        this->result = result;
                                        testerObserverProxy.RequestSend([this]()
                                            {
                                                testerObserverProxy.TestedModeSet(this->result);
                                                MethodDone();
                                            });
                                    });
                            });
                    });
                break;
            case testing::TestedMode::active:
                assert(!resetTested.GetOutputLatch());
                echoToTested.emplace(echoToTestedCreator);
                resetTested.Set(true);
                testedResetDelay.Start(std::chrono::milliseconds(1), [this]()
                    {
                        resetTested.SetAsInput();
                        testerObserverProxy.RequestSend([this]()
                            {
                                testerObserverProxy.TestedModeSet({});
                                MethodDone();
                            });
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
