ENTRY(bl_entry)

MEMORY
{
    sram (wx) : ORIGIN = 0x81C00280, LENGTH = 64K - 0x280
}

SECTIONS
{
    /* Places the 4-byte header exactly at 0x81C0027C */
    .fixptr 0x81C0027C : {
        KEEP(*(.fixptr))
    }

    /* Places the machine instructions exactly at 0x81C00280 */
    .text : {
        KEEP(*(.bl_entry))
        *(.text*)
    } > sram

    .data   : { *(.data*)   } > sram
    .rodata : { *(.rodata*) } > sram
    .bss    : { *(.bss*)    } > sram
}
