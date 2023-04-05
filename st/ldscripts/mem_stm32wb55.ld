MEMORY
{
    FLASH (rx)                 : ORIGIN = 0x08000000, LENGTH = 512K
    RAM (xrw)                 : ORIGIN = 0x20000008, LENGTH = 0x2FFF8
    RAM_SHARED (xrw)           : ORIGIN = 0x20030000, LENGTH = 10K
}
