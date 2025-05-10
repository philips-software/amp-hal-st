#include "generated/echo/LedsAndButton.pb.hpp"
#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "hal_st/stm32fxxx/DefaultClockNucleoF767ZI.hpp"
#include "services/util/DebugLed.hpp"
#include "services/util/EchoInstantiation.hpp"

namespace application
{
    class ButtonHandler
    {
    public:
        ButtonHandler(hal::GpioPin& button, services::Echo& echo)
            : button(button)
            , buttonProxy(echo)
        {
            button.EnableInterrupt([this]()
                {
                    ButtonPressed();
                },
                hal::InterruptTrigger::risingEdge);
        }

    private:
        void ButtonPressed()
        {
            if (!busy)
            {
                busy = true;
                buttonProxy.RequestSend([this]()
                    {
                        busy = false;
                        buttonProxy.Pressed();
                    });
            }
        }

    private:
        hal::InputPin button;
        leds_and_button::ButtonProxy buttonProxy;
        bool busy = false;
    };

}

unsigned int hse_value = 8'000'000;

int main()
{
    HAL_Init();

    // Configure your clock here
    ConfigureDefaultClockNucleo767ZI();

    static main_::StmEventInfrastructure eventInfrastructure;
    static main_::Nucleo144Ui ui;
    static services::DebugLed debugLed(ui.ledGreen);

    static hal::GpioPinStm stLinkUartTxPin{ hal::Port::D, 8 };
    static hal::GpioPinStm stLinkUartRxPin{ hal::Port::D, 9 };
    static hal::DmaStm::TransmitStream transmitStream{ dmaStm, hal::DmaChannelId{ 1, 3, 4 } };
    static hal::UartStmDma stLinkUart{ transmitStream, 3, stLinkUartTxPin, stLinkUartRxPin };
    static hal::BufferedSerialCommunicationOnUnbuffered::WithStorage<256> bufferedStLinkUart{ stLinkUart };

    static services::MethodSerializerFactory::ForServices<>::AndProxies<> serializerFactory;
    static main_::EchoOnSesame<256> echo{ bufferedStLinkUart, serializerFactory };

    eventInfrastructure.Run();
    __builtin_unreachable();
}
