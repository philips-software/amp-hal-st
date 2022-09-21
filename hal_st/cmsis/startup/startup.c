#include <stdint.h>
#include <sys/types.h>

extern unsigned int _sidata;        // Begin address for the initialisation values of the .data section
extern unsigned int __data_start__; // Begin address for the .data section
extern unsigned int __data_end__;   // End address for the .data section; defined in linker script
extern unsigned int __bss_start__;  // Begin address for the .bss section
extern unsigned int __bss_end__;    // End address for the .bss section
extern unsigned int __shared_mem_start__;  // Begin address for the shared memory section
extern unsigned int __shared_mem_end__;    // End address for the shared memory section

extern void __initialize_args(int*, char***);
extern int main(int argc, char* argv[]);
extern void __attribute__((noreturn)) _exit(int);
extern void __initialize_hardware_early(void);
extern void __initialize_hardware(void);

extern void(*__preinit_array_start[]) (void) __attribute__((weak));
extern void(*__preinit_array_end[]) (void) __attribute__((weak));
extern void(*__init_array_start[]) (void) __attribute__((weak));
extern void(*__init_array_end[]) (void) __attribute__((weak));
extern void(*__fini_array_start[]) (void) __attribute__((weak));
extern void(*__fini_array_end[]) (void) __attribute__((weak));

inline void __attribute__((always_inline)) __initialize_data(unsigned int* from, unsigned int* region_begin, unsigned int* region_end)
{
    for (unsigned int* p = region_begin; p != region_end; ++p, ++from)
        *p = *from;
}

inline void __attribute__((always_inline)) __initialize_zero(unsigned int* region_begin, unsigned int* region_end)
{
    for (unsigned int *p = region_begin;  p < region_end; ++p)
        *p = 0;
}

inline static void __attribute__((always_inline)) __run_init_array(void)
{
    for (int i = 0; i != __preinit_array_end - __preinit_array_start; ++i)
        __preinit_array_start[i]();

    for (int i = 0; i != __init_array_end - __init_array_start; ++i)
        __init_array_start[i]();
}

inline static void __attribute__((always_inline)) __run_fini_array(void)
{
    for (int i = __fini_array_end - __fini_array_start; i != 0; --i)
        __fini_array_start[i - 1]();
}

void __attribute__((section(".after_vectors"), noreturn)) _start(void)
{
    __initialize_hardware_early();
    __initialize_data(&_sidata, &__data_start__, &__data_end__);  // Copy the DATA segment from Flash to RAM
    __initialize_zero(&__bss_start__, &__bss_end__); // Zero fill the BSS section
#if defined(STM32WB)
    __initialize_zero(&__shared_mem_start__, &__shared_mem_end__); // Zero fill the SHARED_MEM sections
#endif

    __initialize_hardware();

    int argc;
    char** argv;
    __initialize_args(&argc, &argv);

    __run_init_array(); // Run C++ constructors
    int code = main(argc, argv);
    __run_fini_array(); // Run C++ destructors

    _exit(code);
}
