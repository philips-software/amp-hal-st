#include "generated/echo/Testing.pb.hpp"
#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "hal_st/stm32fxxx/DefaultClockNucleoF767ZI.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/UartStmDma.hpp"
#include "infra/timer/Timer.hpp"
#include "integration_test/instantiations_st/EchoOnSesame.hpp"
#include "services/util/DebouncedButton.hpp"
#include "services/util/DebugLed.hpp"
#include "services/util/GpioPinInverted.hpp"

// Tester    Tested Function
// ------    ------ --------
// CN8-9  -> CN8-9  E5V (NOTE: set JP3 on the Stim to E5V)
// CN8-11 -- CN8-11 ground
// CN8-10 <- CN8-12 ECHO on SESAME
// CN8-12 -> CN8-10 ECHO on SESAME
// CN9-20 -> CN8-5  nRst of Stim
// CN9-24 <- CN9-24 GPIO set by Tester
// CN9-26 -> CN9-26 Tester GPIO control

// Connect debug session to GDB, bring up with:
// JLinkGDBServer -select USB -device STM32F767ZI -endian little -if JTAG -speed auto -noir -LocalhostOnly -nologtofile

class Report
{
public:
    enum class Condition
    {
        Ready,
        Success,
        Timeout,
        FailedPrecondition,
        Failed
    };

    virtual void State(Condition condition) = 0;
};

class LedReport
    : public Report
{
public:
    LedReport(hal::GpioPin& redPin, hal::GpioPin& greenPin)
        : red(redPin)
        , green(greenPin)
    {}

    void State(Condition condition) override
    {
        red.Set(conditions[static_cast<int>(condition)].red);
        green.Set(conditions[static_cast<int>(condition)].green);
    }

private:
    hal::OutputPin red;
    hal::OutputPin green;

    static constexpr struct
    {
        bool red;
        bool green;
    } conditions[5] = {
        { true, true },
        { false, true },
        { true, false },
        { true, false },
        { true, false }
    };
};

class EchoReport
    : public Report
{
public:
    EchoReport(testing::ReponseProxy& response)
        : response(response)
    {}

    void State(Condition condition) override
    {
        response.RequestSend([this, condition]()
            { response.GpioTestResult(conditions[static_cast<int>(condition)].value, conditions[static_cast<int>(condition)].valueN); });
    }

private:
    testing::ReponseProxy& response;

    static constexpr struct
    {
        bool value;
        bool valueN;
    } conditions[5] = {
        { true, true },
        { false, true },
        { true, false },
        { true, false },
        { true, false }
    };
};

class Tester
{
public:
    Tester(hal::GpioPin& resetPin, hal::GpioPin& outPin, hal::GpioPin& inPin, Report& report)
        : reset(resetPin, false)
        , out(outPin, false)
        , in(inPin)
        , report(report)
    {}

    void ResetStim();
    void Run();

private:
    void Timeout();
    void Triggered();

private:
    hal::OutputPin reset;
    hal::OutputPin out;
    hal::InputPin in;
    Report& report;
    infra::TimerSingleShot gpioTimeout;
    infra::TimerSingleShot resetTimer;
};

void Tester::ResetStim()
{
    reset.Set(true);

    resetTimer.Start(std::chrono::milliseconds(3000), [this]()
        {
           reset.Set(false);
        });
}

void Tester::Run()
{
    if (in.Get())
    {
        report.State(Report::Condition::FailedPrecondition);
        return;
    }

    const bool expected = true;
    const auto expectedEdge = expected ? hal::InterruptTrigger::risingEdge : hal::InterruptTrigger::fallingEdge;
    gpioTimeout.Start(std::chrono::seconds(1), [this]()
        { Timeout(); });
    in.EnableInterrupt([this]()
        { Triggered(); },
        expectedEdge);
    out.Set(expected);
}

void Tester::Timeout()
{
    in.DisableInterrupt();
    report.State(Report::Condition::Timeout);
}

void Tester::Triggered()
{
    gpioTimeout.Cancel();
    report.State(Report::Condition::Success);
}

class Command
    : public testing::Command
{
public:
    Command(services::Echo& echo, Tester& testing, hal::GpioPin& led)
        : testing::Command(echo)
        , testing(testing)
        , led(led)
    {
    }

    void RunGpioTest(bool value) override
    {
        led.Set(true);
        testing.Run();
        MethodDone();
    }

private:
    Tester& testing;
    hal::OutputPin led;
};

unsigned int hse_value = 8000000;

int main()
{
    HAL_Init();
    ConfigureDefaultClockNucleo767ZI();

    static main_::StmEventInfrastructure eventInfrastructure;
    static main_::Nucleo144Ui ui;

    // I/O towards Stim
    static hal::GpioPinStm nResetStimPin(hal::Port::E, 6, hal::Drive::OpenDrain);
    static services::GpioPinInverted nResetStimPinInverted(nResetStimPin); // false: HiZ, true: Low

    static hal::GpioPinStm outPin(hal::Port::F, 7);
    static hal::GpioPinStm inPin(hal::Port::F, 8);

    // Echo infrastructure
    static hal::GpioPinStm hostUartTxPin(hal::Port::C, 10);
    static hal::GpioPinStm hostUartRxPin(hal::Port::C, 11);
    static hal::DmaStm dma;
    static hal::UartStmDma hostUart(dma, 4, hostUartTxPin, hostUartRxPin);
    static main_::EchoOnSerialCommunication<60> echo(hostUart);
    static testing::ReponseProxy response(echo);

    // Tester infrastucture
    static LedReport ledReport(ui.ledRed, ui.ledGreen);
    static EchoReport echoReport(response);
    static Tester tester(nResetStimPinInverted, outPin, inPin, echoReport);
    tester.ResetStim();
    static services::DebouncedButton button(ui.buttonOne, []()
        { tester.Run(); });

    // Event infrastructure
    static Command command(echo, tester, ui.ledGreen);
    static services::DebugLed debugLed(ui.ledBlue);

    static main_::EchoBetweenTesterAndTested echoToTested;

    eventInfrastructure.Run();
    __builtin_unreachable();
}
