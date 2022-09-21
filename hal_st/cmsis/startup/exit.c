#include <stdlib.h>

extern void __attribute__((noreturn)) HAL_NVIC_SystemReset(void);

void __attribute__((weak, noreturn)) _exit(int code __attribute__((unused)))
{
    __asm__ __volatile__("bkpt");
    HAL_NVIC_SystemReset();
}

void __attribute__((weak, noreturn)) abort(void)
{
    _exit(1);
}
