MEMORY
{
    FLASH (rx)              : ORIGIN = 0x08000000, LENGTH = 2048K
    RAM_SHARED (xrw)        : ORIGIN = 0, LENGTH = 0
    RAM (xrw)               : ORIGIN = 0x20000000, LENGTH = 640K
}
