#include "generated/echo/Testing.pb.hpp"
#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "hal_st/stm32fxxx/DefaultClockNucleoF767ZI.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/UartStmDma.hpp"
#include "infra/timer/Timer.hpp"
#include "integration_test/instantiations_st/EchoOnSesame.hpp"
#include "integration_test/logic/Gpio.hpp"
#include "integration_test/logic/Tester.hpp"
#include "services/util/DebugLed.hpp"

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

unsigned int hse_value = 8000000;

namespace main_
{
    struct GpioTester
    {
        GpioTester(services::Echo& echo);

        hal::GpioPinStm inPin{ hal::Port::F, 8 };
        hal::GpioPinStm outPin{ hal::Port::F, 7 };
        application::GpioTester gpioTester;
    };

    GpioTester::GpioTester(services::Echo& echo)
        : gpioTester{ echo, inPin, outPin }
    {}

    struct Tester
    {
        Tester(services::Echo& echo);

        hal::GpioPinStm nResetTester{ hal::Port::E, 6, hal::Drive::OpenDrain };
        application::Tester tester;
        application::Perpipheral<main_::GpioTester> gpioTester{ tester, testing::Peripheral::gpio };
    };

    Tester::Tester(services::Echo& echo)
        : tester(echo, nResetTester)
    {}

    struct EchoFromCloud
    {
        hal::GpioPinStm hostUartTxPin{ hal::Port::C, 10 };
        hal::GpioPinStm hostUartRxPin{ hal::Port::C, 11 };
        hal::DmaStm dma;
        hal::UartStmDma hostUart{ dma, 4, hostUartTxPin, hostUartRxPin };
        main_::EchoOnSerialCommunication<60> echo{ hostUart };
    };

    struct ForwardingEchoToTested
    {
        ForwardingEchoToTested(services::Echo& echo);

        main_::EchoToTested echoToTested;
        std::array<uint8_t, 1024> buffer;
        services::ServiceForwarder forwardTested;
        services::ServiceForwarder forwardGpioTested;
        services::ServiceForwarder forwardGpioObserver;
    };

    ForwardingEchoToTested::ForwardingEchoToTested(services::Echo& echo)
        : forwardTested(buffer, echo, testing::Tested::serviceId, echoToTested.echo)
        , forwardGpioTested(buffer, echo, testing::GpioTested::serviceId, echoToTested.echo)
        , forwardGpioObserver(buffer, echoToTested.echo, testing::GpioObserver::serviceId, echo)
    {}
}

int main()
{
    HAL_Init();
    ConfigureDefaultClockNucleo767ZI();

    static main_::StmEventInfrastructure eventInfrastructure;
    static main_::Nucleo144Ui ui;
    static services::DebugLed debugLed(ui.ledBlue);

    static main_::EchoFromCloud echo;
    static main_::Tester tester(echo.echo);
    static main_::ForwardingEchoToTested forwarder(echo.echo);

    eventInfrastructure.Run();
    __builtin_unreachable();
}
