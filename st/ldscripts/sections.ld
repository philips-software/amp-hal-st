__stack = ORIGIN(RAM) + LENGTH(RAM);
_estack = __stack;         /* STM specific definition */

__Main_Stack_Size = 1024 ;
PROVIDE(_Main_Stack_Size = __Main_Stack_Size);

__Main_Stack_Limit = __stack - __Main_Stack_Size ;
PROVIDE (_Main_Stack_Limit = __Main_Stack_Limit);

PROVIDE(_Heap_Begin = __end__);
PROVIDE(_Heap_Limit = __stack - __Main_Stack_Size);

EXTERN(__isr_vectors)

ENTRY(Reset_Handler)

SECTIONS
{
    .isr_vector : ALIGN(4)
    {
        FILL(0xFF)

        KEEP(*(.isr_vector))
        *(.after_vectors .after_vectors.*)
    } >FLASH

    .inits : ALIGN(4)
    {
        /*
         * These are the old initialisation sections, intended to contain
         * naked code, with the prologue/epilogue added by crti.o/crtn.o
         * when linking with startup files. The standalone startup code
         * currently does not run these, better use the init arrays below.
         */
        KEEP(*(.init))
        KEEP(*(.fini))
        _fini = .;

        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array_sysinit .preinit_array_sysinit.*))
        KEEP(*(.preinit_array_platform .preinit_array_platform.*))
        KEEP(*(.preinit_array .preinit_array.*))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(4);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } >FLASH

    .text : ALIGN(4)
    {
        link_code_location = .;
        *(.text .text.*)
        link_code_end = .;

        KEEP(*(.eh_frame*))

        *(.glue_7)    /* Linker-generated stubs for ARM code calling Thumb code */
        *(.glue_7t)   /* Linker-generated stubs for Thumb code calling ARM code */

        . = ALIGN(4);
        _etext = .;
    } >FLASH

    .rodata : ALIGN(4)
    {
        *(.rodata)
        *(.rodata*)
    } >FLASH

    /* Exception sections */
    .ARM.extab : ALIGN(4)
    {
       *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    .ARM.exidx : ALIGN(4)
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        . = ALIGN(4);
        __exidx_end = .;
    } > FLASH

    _sidata = LOADADDR(.data);

    .data : ALIGN(4)
    {
        FILL(0xFF)
        _sdata = .;
        __data_start__ = _sdata;
        *(.data .data.*)
        . = ALIGN(4);
        _edata = .;
        __data_end__ = _edata;

    } >RAM AT>FLASH
    
    .bss (NOLOAD) : ALIGN(4)
    {
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } >RAM

    .noinit (NOLOAD) : ALIGN(4)
    {
        _noinit = .;
        *(.noinit .noinit.*) 
         . = ALIGN(4) ;
        _end_noinit = .;
    } >RAM

    PROVIDE(__end__ = _end_noinit);

    MAPPING_TABLE (NOLOAD) : { *(MAPPING_TABLE) } >RAM_SHARED
    MB_MEM1 (NOLOAD)       : { *(MB_MEM1) } >RAM_SHARED
    MB_MEM2 (NOLOAD)       : { _sMB_MEM2 = . ; *(MB_MEM2) ; _eMB_MEM2 = . ; } >RAM_SHARED

    ._check_stack : ALIGN(4)
    {
        . = . + __Main_Stack_Size ;
    } >RAM

    /* Stabs debugging sections.  */
    .stab          0 : { *(.stab) }
    .stabstr       0 : { *(.stabstr) }
    .stab.excl     0 : { *(.stab.excl) }
    .stab.exclstr  0 : { *(.stab.exclstr) }
    .stab.index    0 : { *(.stab.index) }
    .stab.indexstr 0 : { *(.stab.indexstr) }
    .comment       0 : { *(.comment) }

    /* DWARF debug sections.
     * Symbols in the DWARF debugging sections are relative to the beginning
     * of the section so we begin them at 0.
     */
    /* DWARF 1 */
    .debug          0 : { *(.debug) }
    .line           0 : { *(.line) }
    /* GNU DWARF 1 extensions */
    .debug_srcinfo  0 : { *(.debug_srcinfo) }
    .debug_sfnames  0 : { *(.debug_sfnames) }
    /* DWARF 1.1 and DWARF 2 */
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    /* DWARF 2 */
    .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_line     0 : { *(.debug_line) }
    .debug_frame    0 : { *(.debug_frame) }
    .debug_str      0 : { *(.debug_str) }
    .debug_loc      0 : { *(.debug_loc) }
    .debug_macinfo  0 : { *(.debug_macinfo) }
    /* SGI/MIPS DWARF 2 extensions */
    .debug_weaknames 0 : { *(.debug_weaknames) }
    .debug_funcnames 0 : { *(.debug_funcnames) }
    .debug_typenames 0 : { *(.debug_typenames) }
    .debug_varnames  0 : { *(.debug_varnames) }
}
