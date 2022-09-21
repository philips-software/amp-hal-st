#include <cstdlib>
#include <sys/types.h>

namespace __gnu_cxx
{
    void __attribute__((noreturn)) __verbose_terminate_handler();

    void __verbose_terminate_handler()
    {
        abort();
    }
}

extern "C"
{
    void __attribute__((noreturn)) __cxa_pure_virtual();

    void __cxa_pure_virtual()
    {
        abort();
    }
}
