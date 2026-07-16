// ============================================================
// Minimal Reboot Payload – Unlock + Soft Reset
// ============================================================

typedef unsigned int uint32_t;

#ifndef REG32
#define REG32 volatile uint32_t
#endif

typedef volatile struct {
    REG32 REG_DBG;        // 0x00
    REG32 Sys_Rst_Set;    // 0x04
    REG32 Sys_Rst_Clr;    // 0x08
} HWP_SYS_CTRL_T;

#define REG_SYS_CTRL_BASE   0xA1A00000
#define hwp_sysCtrl         ((HWP_SYS_CTRL_T*) REG_SYS_CTRL_BASE)
#define SYS_CTRL_PROTECT_UNLOCK  (0xA50001)
#define SYS_CTRL_SOFT_RST        (1 << 31)

uint32_t __attribute__((section(".fixptr"))) vendor_fixed_pointer = 0x81C00280;

void __attribute__((section(".bl_entry"))) bl_entry(void) {
    // Unlock system debug register
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;

    // Assert soft reset – should reboot immediately
    hwp_sysCtrl->Sys_Rst_Set = SYS_CTRL_SOFT_RST;

    // Trap (won't be reached if reset works)
    while(1) {
        __asm__ volatile ("" : : : "memory");
    }
}
