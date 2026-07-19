# Soft Reboot Payload & Compiler Configuration
**Date:** 13 July 2026

## Overview
During initial bare-metal payload testing on the RDA 8809, execution failed due to incorrect instruction set parsing by the MIPS core. The root cause was identified as missing strict Little Endian target flags (`-EL`) in the local GCC compiler toolchain. Once `-EL` was applied, the MIPS32 payload executed flawlessly.

This document outlines the baseline template for a successful global soft reboot payload injected into the target device.

## Payload Architecture & Constraints

To successfully execute code on the RDA 8809 without crashing the bootloader context, the payload must map precisely to the expected SRAM layout and satisfy the hardware validation checks.

### 1. Section Mapping & Entry Point
The execution entry function must be mapped explicitly to the `.bl_entry` section. The linker target address for this internal RAM execution space is explicitly defined as `0x81C00000`.

### 2. Vendor Fixed Pointer
A 32-bit tracking word must be preserved inside the `.fixptr` section. This pointer must target the exact absolute address `0x81C00280`, which tells the underlying hardware abstraction where the actual operational logic begins after vector table initialization.

### 3. Hardware Unlocking
Before any system control registers can be manipulated (such as forcing a reboot), the hardware debugging and security gate must be unlocked. This is achieved by writing the magic word `0xA50001` (`SYS_CTRL_PROTECT_UNLOCK`) to the `REG_DBG` register at `0xA1A00000`.

## Soft Reboot Payload Code (`main.c` Template)

```c
// ==========================================
// 1. Primitive Typdefs & Hardware Definitions
// ==========================================
typedef unsigned int uint32_t;

#ifndef REG32
#define REG32 volatile uint32_t
#endif

typedef volatile struct
{
    REG32 REG_DBG;        // 0x00000000 (Debug / Security Unlock Register)
    REG32 Sys_Rst_Set;    // 0x00000004 (Hardware Reset Line Set)
    REG32 Sys_Rst_Clr;    // 0x00000008 (Hardware Reset Line Clear)
} HWP_SYS_CTRL_T;

#define REG_SYS_CTRL_BASE        0xA1A00000
#define hwp_sysCtrl              ((HWP_SYS_CTRL_T*) REG_SYS_CTRL_BASE)

#define SYS_CTRL_PROTECT_UNLOCK  (0XA50001)
#define SYS_CTRL_SOFT_RST        (1 << 31)

// ==========================================
// 2. Vendor Fixed Pointer (.fixptr Section)
// ==========================================
uint32_t __attribute__((section(".fixptr"))) vendor_fixed_pointer = 0x81C00280;

// ==========================================
// 3. Execution Entry Point (.bl_entry Section)
// ==========================================
void __attribute__((section(".bl_entry"))) bl_entry(void)
{
    // Step 1: Force the hardware validation lock gates open
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;

    // Step 2: Drive the physical system soft reset register line high immediately
    hwp_sysCtrl->Sys_Rst_Set = SYS_CTRL_SOFT_RST;

    // Step 3: Trap the execution pipeline while the core power-cycles
    while(1) {
        __asm__ volatile ("" : : : "memory");
    }
}
```

## Compiler Requirements
Always compile RDA 8809 payloads using the `-EL` flag:
`mips-linux-gnu-gcc -EL -mips32 -O2 -G0 -nostdlib ...`
