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
  RAM (w) : ORIGIN = 0x20020000, LENGTH = 368K
  DTCM_RAM : ORIGIN = 0x20000000, LENGTH = 128K
  ITCM_RAM : ORIGIN = 0x00000000, LENGTH = 16K
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 2048K
}
