#include DEVICE_HEADER

extern "C"
{
    unsigned char __attribute__((used)) __atomic_exchange_1(volatile void* mem, unsigned char val, int model)
    {
        __disable_irq();
        __DSB();

        unsigned char result = *reinterpret_cast<volatile uint8_t*>(mem);
        *reinterpret_cast<volatile uint8_t*>(mem) = val;

        __DSB();
        __enable_irq();

        return result;
    }

    unsigned int __attribute__((used)) __atomic_exchange_4(volatile void* mem, unsigned int val, int model)
    {
        __disable_irq();
        __DSB();

        unsigned int result = *reinterpret_cast<volatile uint32_t*>(mem);
        *reinterpret_cast<volatile uint32_t*>(mem) = val;

        __DSB();
        __enable_irq();

        return result;
    }

    bool __attribute__((used)) __atomic_compare_exchange_4(volatile void* mem, void* expected, unsigned int desired, bool weak, int success, int failure)
    {
        __disable_irq();
        __DSB();

        if (*reinterpret_cast<volatile uint32_t*>(mem) == *reinterpret_cast<uint32_t*>(expected))
        {
            *reinterpret_cast<volatile uint32_t*>(mem) = desired;
            __DSB();
            __enable_irq();
            return true;
        }
        else
        {
            *reinterpret_cast<uint32_t*>(expected) = *reinterpret_cast<volatile uint32_t*>(mem);
            __enable_irq();
            return false;
        }
    }

    unsigned int __attribute__((used)) __atomic_fetch_add_4(volatile void* mem, unsigned int val, int model)
    {
        __disable_irq();
        __DSB();

        uint32_t previous = *reinterpret_cast<volatile uint32_t*>(mem);
        *reinterpret_cast<volatile uint32_t*>(mem) += val;

        __DSB();
        __enable_irq();

        return previous;
    }
}
