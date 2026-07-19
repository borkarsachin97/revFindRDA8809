/*
 * =================================================================================
 * Annotated Assembly Blueprint: RDA 8809 Bootloader
 * =================================================================================
 *
 * Execution Flow & Hand-off:
 * 1. __xcpu_start (0x81C00280): Core entry vector configured by link.ld.
 * 2. __jump_1: Bypasses nop padding, configures exception vectors.
 * 3. bss_clear_loop: Clears uninitialized data variables to strictly bound states.
 * 4. jump to main: The 'jalr t9' instruction securely hands off hardware-native
 *    boot execution over to C (bare_metal/main.c).
 */

#include "../../helpfull-headers/regdef.h"
#include "../../helpfull-headers/cp0.h"

.section .text.boot
.set noreorder
.set noat
.globl __xcpu_start

__xcpu_start:
    /* Legacy NOP padding required for older chip revisions before jump */
    nop
    nop
    nop
    nop
    b __jump_1
    nop

__jump_1:
    /* -------------------------------------------------------------
     * MIPS Coprocessor 0 (CP0) Configuration
     * -------------------------------------------------------------
     * CP0 controls the core exception states and interrupt routing.
     * We MUST clear the Cause register so no spurious exceptions
     * fire immediately after enabling interrupts globally.
     */
    mtc0    zero, Cause

    /*
     * The Status register defines operating modes and interrupt masks.
     * Status_IntMASK (0xFF00) allows hardware interrupts.
     * Status_IEc (0x1) enables interrupts globally.
     * 0x400000 sets the BEV (Boot Exception Vectors) bit, forcing exceptions
     * to the uncached ROM space rather than RAM vectors.
     */
    li      k1, (Status_IntMASK | Status_IEc | 0x400000)
    mtc0    k1, Status

    /* -------------------------------------------------------------
     * Stack & SRAM Timing Setup
     * -------------------------------------------------------------
     * Establish the top of the stack pointer.
     * 0x81C10000 limits the pointer within the hard 64KB boundary of
     * internal KSEG0 (Uncached) SRAM.
     */
    lui     sp, 0x81C1

    /* -------------------------------------------------------------
     * Cache Invalidation Sequence & Memory Wipe
     * -------------------------------------------------------------
     * The .bss loop executes a classic runtime initialization.
     * We must zero out memory dynamically defined by the Linker Script
     * variables (_sbss to _ebss).
     */
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
    /* -------------------------------------------------------------
     * Hand-off to Application Logic
     * -------------------------------------------------------------
     * The system is now stabilized. We load the absolute pointer to
     * our C `main()` implementation and jump.
     */
    la      t9, main
    jalr    t9
    nop

halt:
    j       halt
    nop
