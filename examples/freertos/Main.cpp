#include "generated/stm32fxxx/PinoutTableDefault.hpp"
#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "osal/Osal.hpp"
#include "osal/freertos_system_time/TimerServiceFreeRtos.hpp"
#include "services/util/DebugLed.hpp"
#include <chrono>
#include <thread>

extern "C" void Default_Handler()
{
    hal::InterruptTable::Instance().Invoke(hal::ActiveInterrupt());
}

main_::NucleoF767ziUi ui;
infra::Optional<hal::OutputPin> pinGreen;

using namespace std::chrono_literals;

int main()
{
    static hal::InterruptTable::WithStorage<128> interruptTable;
    static hal::GpioStm gpio{ hal::pinoutTableDefaultStm, hal::analogTableDefaultStm };
    static hal::TimerServiceFreeRtos timerService;
    static infra::EventDispatcherWithWeakPtr::WithSize<50> eventDispatcher;

    osal::Init();

    pinGreen.Emplace(ui.ledGreen);

    static std::thread t1([]()
        {
            hal::OutputPin pin(ui.ledBlue);

            while (true)
            {
                // std::this_thread::sleep_for(800s);
                pin.Set(true);
                // std::this_thread::sleep_for(200ms);
                pin.Set(false);
            }
        });

    static std::thread t2([]()
        {
            hal::OutputPin pin(ui.ledRed);

            infra::TimerRepeating toggle(500ms, [&]()
            {
                pin.Set(!pin.GetOutputLatch());
            });

            eventDispatcher.Run();
        });

    osal::Run();
    __builtin_unreachable();
}
