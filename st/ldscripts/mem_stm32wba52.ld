MEMORY
{
    FLASH (rx)                 : ORIGIN = 0x08000000, LENGTH = 1024K
    RAM (xrw)                 : ORIGIN = 0x20000000, LENGTH = 64K
    RAM_SHARED (xrw)           : ORIGIN = 0, LENGTH = 0
}
