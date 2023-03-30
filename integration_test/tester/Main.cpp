#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "services/util/DebugLed.hpp"
#include "infra/timer/Timer.hpp"
#include "hal/interfaces/Gpio.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/UartStmDma.hpp"
#include "services/util/GpioPinInverted.hpp"
#include "services/util/DebouncedButton.hpp"

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

extern "C" void Default_Handler()
{
    hal::InterruptTable::Instance().Invoke(hal::ActiveInterrupt());
}

class Report
{
public:
    Report(hal::GpioPin& redPin, hal::GpioPin& greenPin)
    : red(redPin)
    , green(greenPin)
    {}

    enum class Condition
    {
        Ready,
        Success,
        Timeout,
        FailedPrecondition,
        Failed
    };

    void State(Condition condition)
    {
        red.Set(Conditions[static_cast<int>(condition)].red);
        green.Set(Conditions[static_cast<int>(condition)].green);
    }

private:
    hal::OutputPin red;
    hal::OutputPin green;

    struct
    {
        bool red;
        bool green;
    } Conditions[5] =
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

    resetTimer.Start(std::chrono::milliseconds(3), [this]()
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

    // Tester infrastucture
    static Report report(ui.ledRed, ui.ledGreen);

    static Tester tester(nResetStimPinInverted, nBootedPinInverted, outPin, inPin, inNPin, report);
    tester.ResetStim();
    static services::DebouncedButton button(ui.userButtonPin, []() { tester.Run(); }); // how to disable until booted?

    // Event loop
    static services::DebugLed debugLed(ui.ledBlue);
    eventInfrastructure.Run();
    __builtin_unreachable();
}
