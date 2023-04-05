#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "services/util/DebugLed.hpp"
#include "infra/timer/Timer.hpp"
#include "hal/interfaces/Gpio.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/UartStmDma.hpp"
#include "services/util/GpioPinInverted.hpp"
#include "services/util/DebouncedButton.hpp"
#include "protobuf/echo/EchoInstantiation.hpp"
#include "generated/echo/Tester.pb.hpp"

/* Tester    Stim   Function
 * ------    ------ --------
 * CN8-9  -> CN8-9  E5V (NOTE: set JP3 on the Stim to E5V)
 * CN8-11 -- CN8-11 ground
 * CN9-20 -> CN8-5  nRst of Stim
 * CN9-22 <> CN9-22 open drain "#booted" signal (pulled up by Tester)
 * CN9-24 <- CN9-24 non-inverted GPIO
 * CN9-26 -> CN9-26 Tester GPIO control
 * CN9-28 <- CN9-28 inverted GPIO
 */


/* Connect debug session to GDB, bring up with:
 * JLinkGDBServer -select USB -device STM32F767ZI -endian little -if JTAG -speed auto -noir -LocalhostOnly -nologtofile
 */

extern "C" void Default_Handler()
{
    hal::InterruptTable::Instance().Invoke(hal::ActiveInterrupt());
}


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
    } conditions[5] =
    {
        {true, true},
        {false, true},
        {true, false},
        {true, false},
        {true, false}
    };
};


class EchoReport
    : public Report
{
public:
    EchoReport(tester_echo::ReponseProxy& response)
    : response(response)
    {}

    void State(Condition condition) override
    {
        response.RequestSend([this, condition]() {
            response.GpioTestResult(conditions[static_cast<int>(condition)].value, conditions[static_cast<int>(condition)].valueN);
        });
        
    }

private:
    tester_echo::ReponseProxy& response;

    static constexpr struct
    {
        bool value;
        bool valueN;
    } conditions[5] =
    {
        {true, true},
        {false, true},
        {true, false},
        {true, false},
        {true, false}
    };
};

class Tester
{
public:
    Tester(hal::GpioPin& resetPin, hal::GpioPin& bootedPin, hal::GpioPin& outPin, hal::GpioPin& inPin, hal::GpioPin& inNPin, Report& report)
    : ready(false)
    , reset(resetPin, false)
    , booted(bootedPin)
    , out(outPin, false)
    , in(inPin)
    , inN(inNPin)
    , report(report)
    {}

    void ResetStim();
    void Run();

private:
    void Timeout();
    void Triggered();

private:
    volatile bool ready;
    hal::OutputPin reset;
    hal::InputPin booted;
    hal::OutputPin out;
    hal::InputPin in;
    hal::InputPin inN;
    Report& report;
    infra::TimerSingleShot gpioTimeout;
    infra::TimerSingleShot resetTimer;
};


void Tester::ResetStim()
{
    ready = false;
    reset.Set(true);

    resetTimer.Start(std::chrono::milliseconds(3000), [this]()
    {
        assert(booted.Get() == true);
        booted.EnableInterrupt([this]() {
            report.State(Report::Condition::Ready);
            ready = true;
        }, hal::InterruptTrigger::risingEdge);
        reset.Set(false);
    });
}


void Tester::Run()
{
    if (!ready) return;

    if (in.Get() == true)
    {
        report.State(Report::Condition::FailedPrecondition);
        return;
    }
    
    const bool expected = true;
    const auto expectedEdge = expected ? hal::InterruptTrigger::risingEdge : hal::InterruptTrigger::fallingEdge;
    gpioTimeout.Start(std::chrono::seconds(1), [this]() { Timeout(); });
    in.EnableInterrupt([this]() { Triggered(); }, expectedEdge);
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
        if (in.Get() != inN.Get())
            report.State(Report::Condition::Success);
        else
            report.State(Report::Condition::Failed);
}


class Command
    : public tester_echo::Command
{
public:
    Command(services::Echo& echo, Tester& tester, hal::GpioPin& led)
        : tester_echo::Command(echo)
        , tester(tester)
        , led(led)
    {
    }

    void RunGpioTest(bool value) override
    {
        led.Set(true);
        tester.Run();
        MethodDone();
    }

private:
    Tester& tester;
    hal::OutputPin led;
};


int main()
{
    static main_::StmEventInfrastructure eventInfrastructure;
    static main_::NucleoF767ziUi ui;

    // I/O towards Stim
    static hal::GpioPinStm nResetStimPin(hal::Port::E, 6, hal::Drive::OpenDrain);
    static services::GpioPinInverted nResetStimPinInverted(nResetStimPin); // false: HiZ, true: Low
    
    static hal::GpioPinStm nBootedPin(hal::Port::E, 3, hal::Drive::OpenDrain, hal::Speed::Default, hal::WeakPull::Up);
    static services::GpioPinInverted nBootedPinInverted(nBootedPin);

    static hal::GpioPinStm outPin(hal::Port::F, 7);
    static hal::GpioPinStm inPin(hal::Port::F, 8);
    static hal::GpioPinStm inNPin(hal::Port::F, 9);

    // Echo infrastructure
    static hal::GpioPinStm hostUartTxPin(hal::Port::C, 10);
    static hal::GpioPinStm hostUartRxPin(hal::Port::C, 11);
    static hal::DmaStm dma;
    static hal::UartStmDma hostUart(dma, 4, hostUartTxPin, hostUartRxPin);
    static main_::EchoOnSerialCommunication<60> echo(hostUart);
    static tester_echo::ReponseProxy response(echo);
    
    // Tester infrastucture
    static LedReport ledReport(ui.ledRed, ui.ledGreen);
    static EchoReport echoReport(response);
    static Tester tester(nResetStimPinInverted, nBootedPinInverted, outPin, inPin, inNPin, echoReport);
    tester.ResetStim();
    static services::DebouncedButton button(ui.userButtonPin, []() { tester.Run(); });

    // Event infrastructure
    static Command command(echo, tester, ui.ledGreen);
    static services::DebugLed debugLed(ui.ledBlue);
    eventInfrastructure.Run();
    __builtin_unreachable();
}
