#include "hal/interfaces/Gpio.hpp"
#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "services/util/DebugLed.hpp"
#include "services/util/GpioPinInverted.hpp"

extern "C" void Default_Handler()
{
    hal::InterruptTable::Instance().Invoke(hal::ActiveInterrupt());
}

class StimGpio
{
public:
    explicit StimGpio(hal::GpioPin& inPin, hal::GpioPin& outPin, hal::GpioPin& outNPin);

private:
    void Update();

    hal::InputPin in;
    hal::OutputPin out;
    services::GpioPinInverted outNPinInverted;
    hal::OutputPin outN;
};

StimGpio::StimGpio(hal::GpioPin& inPin, hal::GpioPin& outPin, hal::GpioPin& outNPin)
    : in(inPin)
    , out(outPin)
    , outNPinInverted(outNPin)
    , outN(outNPinInverted)
{
    Update();

    in.EnableInterrupt([this]()
        { Update(); },
        hal::InterruptTrigger::bothEdges);
}

void StimGpio::Update()
{
    const auto value = in.Get();
    out.Set(value);
    outN.Set(value);
}

int main()
{
    static hal::GpioPinStm nBootedPin(hal::Port::E, 3);
    static services::GpioPinInverted bootedPin(nBootedPin);
    static hal::OutputPin booted(bootedPin, false);

    static main_::StmEventInfrastructure eventInfrastructure;
    static main_::NucleoF767ziUi ui;
    static services::DebugLed debugLed(ui.ledBlue);

    static hal::GpioPinStm inPin(hal::Port::F, 7);
    static hal::GpioPinStm outPin(hal::Port::F, 8);
    static hal::GpioPinStm outNPin(hal::Port::F, 9);

    static StimGpio stimGpio(inPin, outPin, outNPin);

    booted.Set(true);
    eventInfrastructure.Run();
    __builtin_unreachable();
}
