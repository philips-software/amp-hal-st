#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "services/util/DebugLed.hpp"
#include "infra/timer/Timer.hpp"
#include "hal/interfaces/Gpio.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/UartStmDma.hpp"
#include "services/util/GpioPinInverted.hpp"

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

    static hal::GpioPinStm nResetStim(hal::Port::E, 6, hal::Drive::OpenDrain);
    static services::GpioPinInverted resetStim(nResetStim); // false: HiZ, true: Low
    static hal::GpioPinStm nBooted(hal::Port::E, 3, hal::Drive::OpenDrain, hal::Speed::Default, hal::WeakPull::Up);
    static services::GpioPinInverted booted(nBooted);
    booted.Config(hal::PinConfigType::input);

    outPin.Set(false); // NOTE: is this redundant?
    resetStim.Set(true);
    infra::TimerSingleShot resetTimer(std::chrono::milliseconds(3), []()
        {
            if (booted.Get() == true) {
                ui.ledGreen.Set(false);
                ui.ledRed.Set(true);
            }
            else
            {
                booted.EnableInterrupt([]() {
                    ui.ledGreen.Set(true);
                    ui.ledRed.Set(true);
                    
                    ui.userButtonPin.EnableInterrupt([]() {                        
                        if (inPin.Get() == true)
                        {
                            ui.ledGreen.Set(false);
                            ui.ledRed.Set(true);
                        }
                        else
                        {
                            static infra::TimerSingleShot gpioTimeout(std::chrono::seconds(1), []() {
                                inPin.DisableInterrupt();
                                ui.ledGreen.Set(false);
                                ui.ledRed.Set(true);
                            });

                            const bool expect = true;
                            inPin.EnableInterrupt([]() {
                                gpioTimeout.Cancel();
                                const auto value = inPin.Get();
                                const auto nValue = inNPin.Get();
                                ui.ledGreen.Set(value == expect && nValue == !expect);
                                ui.ledRed.Set(value != expect || nValue != !expect);
                            }, hal::InterruptTrigger::risingEdge);

                            outPin.Set(expect);
                        }
                    }, hal::InterruptTrigger::fallingEdge);
                }, hal::InterruptTrigger::risingEdge);
                resetStim.Set(false);
            }
        });

    eventInfrastructure.Run();
    __builtin_unreachable();
}
