MEMORY
{
    FLASH                   : ORIGIN = 0x08000000, LENGTH = 2M
    RAM                     : ORIGIN = 0x20000000, LENGTH = 512K
    RAM_SHARED (xrw)        : ORIGIN = 0, LENGTH = 0
}
