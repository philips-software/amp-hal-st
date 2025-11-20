#include DEVICE_HEADER
#include "hal_st/cortex/Fault.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include <cstdlib>
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

    [[gnu::naked]] void Default_Handler_Forwarded()
    {
        asm volatile(
            "tst   lr, #4           \n"
            "ite   eq               \n"
            "mrseq r0, msp          \n"
            "mrsne r0, psp          \n"
            "mov r1, lr             \n"
            "b DefaultHandlerImpl   \n");
    }

    [[gnu::weak]] void DefaultHandlerImpl(const uint32_t* stack, uint32_t lr)
    {
        hal::fault::SetInterruptContext(stack, lr);
        hal::InterruptTable::Instance().Invoke(hal::ActiveInterrupt());
    }

    // Avoid the SysTick handler from being initialised by HAL_Init
    HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
    {
        return HAL_OK;
    }

    [[gnu::weak]] void abort()
    {
        __BKPT();
        HAL_NVIC_SystemReset();
        __builtin_unreachable();
    }

    void _init()
    {}

    void __assert_func(const char*, int, const char*, const char*)
    {
        std::abort();
    }

    void assert_failed(uint8_t* file, uint32_t line)
    {
        std::abort();
    }
}
