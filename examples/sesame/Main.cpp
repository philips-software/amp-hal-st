#include "generated/echo/LedsAndButton.pb.hpp"
#include "generated/echo/SesameSecurity.pb.hpp"
#include "infra/util/ByteRange.hpp"
#ifdef SECURE_SYMMETRIC
#include "generated/key_material/SymmetricKey.hpp"
#elif SECURE_DIFFIE_HELLMAN
#include "generated/key_material/DeviceCertificate.hpp"
#include "generated/key_material/DeviceCertificatePrivateKey.hpp"
#include "generated/key_material/RootCaCertificate.hpp"
#endif
#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "hal_st/stm32fxxx/DefaultClockNucleoF767ZI.hpp"
#include "hal_st/stm32fxxx/UartStmDma.hpp"
#include "hal_st/synchronous_stm32fxxx/SynchronousRandomDataGeneratorStm.hpp"
#include "infra/stream/ByteInputStream.hpp"
#include "infra/syntax/ProtoParser.hpp"
#include "services/util/DebugLed.hpp"
#include "services/util/EchoInstantiation.hpp"
#include "services/util/EchoInstantiationSecured.hpp"

namespace application
{
    class LedsHandler
        : private leds_and_button::Leds
    {
    public:
        LedsHandler(hal::GpioPin& led0, hal::GpioPin& led1, hal::GpioPin& led2, services::Echo& echo)
            : leds_and_button::Leds(echo)
            , led0(led0)
            , led1(led1)
            , led2(led2)
        {}

    private:
        virtual void On(uint32_t id) override
        {
            switch (id)
            {
                case 0:
                    led0.Set(true);
                    break;
                case 1:
                    led1.Set(true);
                    break;
                case 2:
                    led2.Set(true);
                    break;
            }

            MethodDone();
        }

        virtual void Off(uint32_t id) override
        {
            switch (id)
            {
                case 0:
                    led0.Set(false);
                    break;
                case 1:
                    led1.Set(false);
                    break;
                case 2:
                    led2.Set(false);
                    break;
            }

            MethodDone();
        }

    private:
        hal::OutputPin led0;
        hal::OutputPin led1;
        hal::OutputPin led2;
    };

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

    template<class T>
    T ParseProto(infra::ConstByteRange data)
    {
        infra::ByteInputStream stream{ data };
        infra::ProtoParser parser{ stream };
        return T{ parser };
    }
}

unsigned int hse_value = 8'000'000;

int main()
{
    HAL_Init();
    ConfigureDefaultClockNucleo767ZI();

    static main_::StmEventInfrastructure eventInfrastructure;
    static main_::Nucleo144Ui ui;
    static hal::DmaStm dma;
    static hal::SynchronousRandomDataGeneratorStm randomDataGenerator;

    static hal::GpioPinStm stLinkUartTxPin{ hal::Port::D, 8 };
    static hal::GpioPinStm stLinkUartRxPin{ hal::Port::D, 9 };
    static hal::DmaStm::TransmitStream transmitStream{ dma, hal::DmaChannelId{ 1, 3, 4 } };
    static hal::UartStmDma stLinkUart{ transmitStream, 3, stLinkUartTxPin, stLinkUartRxPin };
    static hal::BufferedSerialCommunicationOnUnbuffered::WithStorage<256> bufferedUart{ stLinkUart };

#ifdef SECURE_SYMMETRIC
    static services::MethodSerializerFactory::ForServices<leds_and_button::Leds, sesame_security::SymmetricKeyEstablishment>::AndProxies<leds_and_button::ButtonProxy, sesame_security::SymmetricKeyEstablishment> serializerFactory;
    auto keyMaterial{ application::ParseProto<sesame_security::SymmetricKeyFile>(key_material::SymmetricKey) };
    static main_::EchoOnSesameSecuredSymmetricKey::WithMessageSize<256> echo{ bufferedUart, serializerFactory, services::ConvertKeyMaterial(keyMaterial), randomDataGenerator };
#elif SECURE_DIFFIE_HELLMAN
    static services::MethodSerializerFactory::ForServices<leds_and_button::Leds, sesame_security::DiffieHellmanKeyEstablishment>::AndProxies<leds_and_button::ButtonProxy, sesame_security::DiffieHellmanKeyEstablishment> serializerFactory;
    auto rootCaCertificate{ application::ParseProto<sesame_security::Certificate>(key_material::RootCaCertificate) };
    auto deviceCertificate{ application::ParseProto<sesame_security::Certificate>(key_material::DeviceCertificate) };
    auto deviceCertificatePrivateKey{ application::ParseProto<sesame_security::CertificatePrivateKey>(key_material::DeviceCertificatePrivateKey) };
    static main_::EchoOnSesameSecuredDiffieHellman::WithMessageSize<256> echo{ bufferedUart, serializerFactory, infra::MakeRange(deviceCertificate.certificate), infra::MakeRange(deviceCertificatePrivateKey.privateKey), infra::MakeRange(rootCaCertificate.certificate), randomDataGenerator };
#else
    static services::MethodSerializerFactory::ForServices<leds_and_button::Leds>::AndProxies<leds_and_button::ButtonProxy> serializerFactory;
    static main_::EchoOnSesame::WithMessageSize<256> echo{ bufferedUart, serializerFactory };
#endif

    static application::ButtonHandler buttonHandler{ ui.buttonOne, echo.echo };
    static application::LedsHandler ledsHandler{ ui.ledBlue, ui.ledGreen, ui.ledRed, echo.echo };

    eventInfrastructure.Run();
    __builtin_unreachable();
}
