#include "../../helpfull-headers/regdef.h"
#include "../../helpfull-headers/cp0.h"
#include "../../helpfull-headers/global_macros.h"
#include "../../helpfull-headers/sys_ctrl_asm.h"
#include "../../helpfull-headers/mem_bridge_asm.h"
#include "../../helpfull-headers/bb_sram_asm.h"

.section .text.boot
.set noreorder
.set noat
.globl __xcpu_start
.globl _start

/* Alias _start to __xcpu_start so the linker has a standard entry */
_start:
__xcpu_start:
    /* Historical reason: fixing the "four nop" issue */
    nop
    nop
    nop
    nop
    b __jump_1
    nop

__jump_1:
    /* ------------------------------------
       Save context for Watchdog in BB SRAM
       ------------------------------------ */
    la     k0, KSEG1(REG_BB_SRAM_BASE)
    .set noat
    sw  AT, 0x4(k0)
    .set at
    sw  a0, 0x10(k0)
    sw  a1, 0x14(k0)
    sw  t0, 0x20(k0)
    sw  t1, 0x24(k0)
    sw  a3, 0x1c(k0)
    sw  t3, 0x2c(k0)

    /* MIPS CP0 Initialization */
    /* 1. Clear CP0 Cause Register */
    mtc0    zero, Cause

    /* 2. Setup CP0 Status Register */
    li      t0, (Status_IntMASK | Status_IEc | 0x400000)
    mtc0    t0, Status

    /* 3. Setup Stack Pointer in SRAM */
    lui     sp, 0x81C1

    /* 4. Clear .bss */
    la      v0, _sbss
    la      v1, _ebss
bss_clear_loop:
    beq     v0, v1, bss_clear_done
    nop
    sw      zero, 0(v0)
    addiu   v0, 4
    j       bss_clear_loop
    nop

bss_clear_done:
    /* 5. Jump to main */
    la      t9, main
    jalr    t9
    nop

halt:
    j       halt
    nop
