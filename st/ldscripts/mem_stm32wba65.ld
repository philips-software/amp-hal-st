MEMORY
{
    RAM                     : ORIGIN = 0x20000000, LENGTH = 512K
    FLASH                   : ORIGIN = 0x08000000, LENGTH = 2M
    RAM_SHARED (xrw)        : ORIGIN = 0, LENGTH = 0
}
