#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "services/util/DebugLed.hpp"

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

int main()
{
    static main_::StmEventInfrastructure eventInfrastructure;
    static main_::NucleoF767ziUi ui;
    static services::DebugLed debugLed(ui.ledBlue);

    eventInfrastructure.Run();
    __builtin_unreachable();
}
