#include DEVICE_HEADER
#include "hal_st/cortex/InterruptCortex.hpp"
#include <errno.h>
#include <sys/types.h>

extern "C"
{
    extern char _Heap_Begin;
    extern char _Heap_Limit;

    caddr_t _sbrk(int incr)
    {
        static char* current_heap_end = &_Heap_Begin;

        char* current_block_address = current_heap_end;

        incr = (incr + 3) & (~3);
        if (current_heap_end + incr > &_Heap_Limit)
        {
            errno = ENOMEM;
            return reinterpret_cast<caddr_t>(-1);
        }

        current_heap_end += incr;

        return static_cast<caddr_t>(current_block_address);
    }

    // Avoid the SysTick handler from being initialised by HAL_Init
    HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
    {
        return HAL_OK;
    }

    void Default_Handler_Forwarded()
    {
        hal::InterruptTable::Instance().Invoke(hal::ActiveInterrupt());
    }

    void abort()
    {
        __BKPT();
        HAL_NVIC_SystemReset();
        __builtin_unreachable();
    }

    void _init()
    {}

    void assert_failed(uint8_t* file, uint32_t line)
    {
        std::abort();
    }
}
