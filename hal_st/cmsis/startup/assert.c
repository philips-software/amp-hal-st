#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

void __attribute__((weak, noreturn)) __assert_func(const char* file, int line, const char* func, const char* failedexpr)
{
    abort();
}

void __attribute__((weak, noreturn)) assert_failed(uint8_t* file, uint32_t line)
{
    abort();
}
