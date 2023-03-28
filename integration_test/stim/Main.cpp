#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "services/util/DebugLed.hpp"
#include "hal/interfaces/Gpio.hpp"
#include "services/util/GpioPinInverted.hpp"

extern "C" void Default_Handler()
{
    hal::InterruptTable::Instance().Invoke(hal::ActiveInterrupt());
}

int main()
{
    static hal::GpioPinStm nBooted(hal::Port::E, 3);
    static services::GpioPinInverted booted(nBooted);

    booted.Set(false);

    static main_::StmEventInfrastructure eventInfrastructure;
    static main_::NucleoF767ziUi ui;
    static services::DebugLed debugLed(ui.ledBlue);    

    static hal::GpioPinStm inPin(hal::Port::F, 7);
    static hal::GpioPinStm outPin(hal::Port::F, 8);
    static hal::GpioPinStm outNPin(hal::Port::F, 9);
    inPin.Config(hal::PinConfigType::input);
    outNPin.Set(true);
    
    inPin.EnableInterrupt([]() {
        const auto value = inPin.Get();
        outPin.Set(value);
        outNPin.Set(!value);
    }, hal::InterruptTrigger::bothEdges);

    booted.Set(true);
    
    eventInfrastructure.Run();
    __builtin_unreachable();
}
