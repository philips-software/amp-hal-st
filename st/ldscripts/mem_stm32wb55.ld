/*
 * Memory Spaces Definitions.
 *
 * Need modifying for a specific board. 
 *   FLASH.ORIGIN: starting address of flash
 *   FLASH.LENGTH: length of flash
 *   RAM.ORIGIN: starting address of RAM bank 0
 *   RAM.LENGTH: length of RAM bank 0
 *
 * The values below can be addressed in further linker scripts
 * using functions like 'ORIGIN(RAM)' or 'LENGTH(RAM)'.
 */

MEMORY
{
    FLASH (rx)                 : ORIGIN = 0x08000000, LENGTH = 512K
    RAM (xrw)                 : ORIGIN = 0x20000008, LENGTH = 0x2FFF8
    RAM_SHARED (xrw)           : ORIGIN = 0x20030000, LENGTH = 10K
}

_flash_start = ORIGIN(FLASH);
_flash_end = ORIGIN(FLASH) + LENGTH(FLASH);
