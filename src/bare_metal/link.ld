ENTRY(_start)

MEMORY
{
    /* Enforce exactly the 64 KB memory limit constraint in KSEG0 SRAM */
    SRAM (rwx) : ORIGIN = 0x81C00000, LENGTH = 64K
}

SECTIONS
{
    /* Force entry vector exactly at 0x81C00280 */
    . = 0x81C00280;

    .text :
    {
        KEEP(*(.text.boot))
        *(.text)
        *(.text.*)
    } > SRAM

    .rodata :
    {
        *(.rodata)
        *(.rodata.*)
    } > SRAM

    .data :
    {
        *(.data)
        *(.data.*)
    } > SRAM

    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > SRAM

    /* Discard standard compiler artifacts not needed in bare-metal */
    /DISCARD/ :
    {
        *(.MIPS.abiflags)
        *(.reginfo)
    }
}
