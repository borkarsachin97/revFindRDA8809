#include "sys_ctrl.h"

// SYS_CTRL Base address is mapped via hw_sys_ctrl pointing to 0xA1A00000

void sys_ctrl_init_clocks(void) {
    // Basic clock stabilization routine derived from bootloader tracing
    // Disable system resets
    hw_sys_ctrl->Sys_Rst_Clr = 0xFFFFFFFF;

    // Enable core peripheral clocks (e.g. GPIO, TIMER)
    // Using a theoretical bitmask based on typical architectures
    // In production, match exactly the bits for SYS_GPIO, SYS_TIMER etc.
    hw_sys_ctrl->Clk_Sys_Enable = 0xFFFFFFFF;
}
