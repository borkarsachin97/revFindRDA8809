.section .text.boot
.set noreorder
.set noat
.globl _start

_start:
    /* Entry point matched to 0x81C00280 */
    /* 1. Clear CP0 Cause Register */
    mtc0    $0, $13      /* Cause = 0 */
    nop

    /* 2. Setup CP0 Status Register */
    li      $k1, 0x0040C001  /* Basic execution mode constraints */
    mtc0    $k1, $12     /* Status */

    /* 3. Setup Stack Pointer in SRAM */
    lui     $sp, 0x81C0  /* Assuming SRAM ends near here or we just use KSEG0 space */
    ori     $sp, $sp, 0x8000 /* Stack top at 0x81c08000 */

    /* 4. Clear .bss */
    la      $v0, _sbss
    la      $v1, _ebss
bss_clear_loop:
    beq     $v0, $v1, bss_clear_done
    nop
    sw      $0, 0($v0)
    addiu   $v0, 4
    j       bss_clear_loop
    nop

bss_clear_done:
    /* 5. Jump to main */
    la      $t9, main
    jalr    $t9
    nop

halt:
    j       halt
    nop
