#include "generated/echo/Testing.pb.hpp"
#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "hal_st/stm32fxxx/DefaultClockNucleoF767ZI.hpp"
#include "integration_test/instantiations_st/EchoOnSesame.hpp"
#include "services/util/DebugLed.hpp"
#include "services/util/GpioPinInverted.hpp"

unsigned int hse_value = 8000000;

class GpioTested
    : public testing::GpioTested
    , public testing::GpioObserverProxy
{
public:
    GpioTested(services::Echo& echo, hal::GpioPin& inPin, hal::GpioPin& outPin);

    // Implementation of GpioTested
    void SetGpio(bool state, uint32_t pin) override;

private:
    void InChanged();

private:
    hal::InputPin in;
    hal::OutputPin out;
    bool sending = false;
};

GpioTested::GpioTested(services::Echo& echo, hal::GpioPin& inPin, hal::GpioPin& outPin)
    : testing::GpioTested(echo)
    , testing::GpioObserverProxy(echo)
    , in(inPin)
    , out(outPin, false)
{
    in.EnableInterrupt([this]() { InChanged(); }, hal::InterruptTrigger::bothEdges);
}

void GpioTested::SetGpio(bool state, uint32_t pin)
{
    out.Set(state);
}

void GpioTested::InChanged()
{
    if (!sending)
        RequestSend([this]()
            {
                sending = false;
                TestedGpioChanged(in.Get(), 0);
            });

    sending = true;
}

int main()
{
    HAL_Init();
    ConfigureDefaultClockNucleo767ZI();

    static main_::StmEventInfrastructure eventInfrastructure;
    static main_::Nucleo144Ui ui;
    static services::DebugLed debugLed(ui.ledBlue);

    static hal::GpioPinStm inPin(hal::Port::F, 7);
    static hal::GpioPinStm outPin(hal::Port::F, 8);

    static main_::EchoBetweenTesterAndTested echo;

    GpioTested gpioTested(echo.echo, inPin, outPin);

    eventInfrastructure.Run();
    __builtin_unreachable();
}
