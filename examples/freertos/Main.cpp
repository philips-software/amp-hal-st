#include "generated/stm32fxxx/PinoutTableDefault.hpp"
#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "osal/Osal.hpp"
#include "osal/freertos/LowPowerStrategyFreeRtos.hpp"
#include "osal/freertos_system_time/TimerServiceFreeRtos.hpp"
#include "services/util/DebugLed.hpp"
#include <chrono>
#include <thread>

extern "C" void Default_Handler()
{
    hal::InterruptTable::Instance().Invoke(hal::ActiveInterrupt());
}

extern "C" void abort()
{
    while (true);
}

#include <sys/types.h>
#include <errno.h>

extern char _Heap_Begin;
extern char _Heap_Limit;

extern "C" __attribute__((__used__)) caddr_t _sbrk(int incr)
{
    static char* current_heap_end = &_Heap_Begin;

    char* current_block_address = current_heap_end;

    incr = (incr + 3) & (~3); // align value to 4
    if (current_heap_end + incr > &_Heap_Limit)
    {
        errno = ENOMEM;
        return (caddr_t)-1;
    }

    current_heap_end += incr;

    return (caddr_t)current_block_address;
}

using namespace std::chrono_literals;

extern "C" void xPortPendSVHandler();
extern "C" void xPortSysTickHandler();
extern "C" void vPortSVCHandler();

extern "C" [[gnu::naked]] void SVC_Handler() { asm("b vPortSVCHandler"); }
extern "C" [[gnu::naked]] void PendSV_Handler() { asm("b xPortPendSVHandler"); }
extern "C" [[gnu::naked]] void SysTick_Handler() { asm("b xPortSysTickHandler"); };

int main()
{
    static hal::InterruptTable::WithStorage<128> interruptTable;
    static hal::GpioStm gpio{ hal::pinoutTableDefaultStm, hal::analogTableDefaultStm };
    static hal::TimerServiceFreeRtos timerService;
    static hal::LowPowerStrategyFreeRtos lowPowerStrategy;
    static infra::LowPowerEventDispatcher::WithSize<50> eventDispatcher(lowPowerStrategy);

    osal::Init();

    static main_::NucleoF767ziUi ui;
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
