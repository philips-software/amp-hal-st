#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "services/util/DebugLed.hpp"
#include "hal/interfaces/Gpio.hpp"

extern "C" void Default_Handler()
{
    hal::InterruptTable::Instance().Invoke(hal::ActiveInterrupt());
}

int main()
{
    static main_::StmEventInfrastructure eventInfrastructure;
    static main_::NucleoF767ziUi ui;
    static services::DebugLed debugLed(ui.ledBlue);    

    static hal::GpioPinStm outPin(hal::Port::F, 7);
    static hal::GpioPinStm inPin(hal::Port::F, 8);
    static hal::GpioPinStm inNPin(hal::Port::F, 9);
    inPin.Config(hal::PinConfigType::input);
    inNPin.Config(hal::PinConfigType::input);
    
    ui.userButtonPin.EnableInterrupt([]() {
        const bool expect = true;
        outPin.Set(expect);
        const auto value = inPin.Get();
        const auto nValue = inNPin.Get();
        ui.ledGreen.Set(value == expect && nValue == !expect);
        ui.ledRed.Set(value != expect || nValue != !expect);
    }, hal::InterruptTrigger::fallingEdge);

    eventInfrastructure.Run();
    __builtin_unreachable();
}
