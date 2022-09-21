#include "cmsis_device_hal.h"
#include "DefaultClock.h"

void __initialize_hardware_early(void)
{
    SystemInit();
}

// Avoid the SysTick handler from being initialised by HAL_Init
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{}

void __initialize_hardware(void)
{
    ConfigureDefaultClock();
    SystemCoreClockUpdate();
    HAL_Init();
}
