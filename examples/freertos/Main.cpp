#include "generated/stm32fxxx/PinoutTableDefault.hpp"
#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "osal/Osal.hpp"
#include "osal/freertos/LowPowerStrategyFreeRtos.hpp"
#include "osal/freertos_system_time/TimerServiceFreeRtos.hpp"
#include "services/util/DebugLed.hpp"
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

extern "C" void xPortPendSVHandler();
extern "C" void xPortSysTickHandler();
extern "C" void vPortSVCHandler();

extern "C" [[gnu::naked]] void SVC_Handler()
{
    asm("b vPortSVCHandler");
}

extern "C" [[gnu::naked]] void PendSV_Handler()
{
    asm("b xPortPendSVHandler");
}

extern "C" [[gnu::naked]] void SysTick_Handler()
{
    asm("b xPortSysTickHandler");
};

unsigned int hse_value = 8000000;

int main()
{
    HAL_Init();

    // Configure your clock here
    // ConfigureDefaultClockNucleo767ZI();

    static hal::InterruptTable::WithStorage<128> interruptTable;
    static hal::GpioStm gpio{ hal::pinoutTableDefaultStm, hal::analogTableDefaultStm };
    static hal::TimerServiceFreeRtos timerService;
    static hal::LowPowerStrategyFreeRtos lowPowerStrategy;
    static infra::LowPowerEventDispatcher::WithSize<50> eventDispatcher(lowPowerStrategy);

    osal::Init();

    static main_::Nucleo144Ui ui;
    static hal::OutputPin pinGreen{ ui.ledGreen };

    static std::thread t1([]()
        {
            hal::OutputPin pin(ui.ledBlue);

            while (true)
            {
                std::this_thread::sleep_for(700ms);
                pin.Set(true);
                std::this_thread::sleep_for(200ms);
                pin.Set(false);
            } });

    static std::thread t2([]()
        {
            hal::OutputPin pin(ui.ledRed);

            infra::TimerRepeating toggle(500ms, [&]()
                {
                    pin.Set(!pin.GetOutputLatch());
                });

            eventDispatcher.Run(); });

    osal::Run();
    __builtin_unreachable();
}
