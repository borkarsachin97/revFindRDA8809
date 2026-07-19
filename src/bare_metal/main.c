#include <stdint.h>
#include "../hw_structs/sys_ctrl.h"
#include "../hw_structs/timer.h"
#include "../hw_structs/pwm.h"

// Simple delay using a spinloop
void delay(volatile uint32_t count) {
    while (count--) {
        __asm__ volatile ("nop");
    }
}

int main(void) {
    // 1. Kill the Watchdog Timer
    hw_timer->WDTimer_Ctrl = (1 << 4); // TIMER_STOP
    hw_timer->WDTimer_LoadVal = 0;

    // 2. Unlock System Registers
    hw_sys_ctrl->REG_DBG = 1U << 31; // SYS_CTRL_WRITE_UNLOCK

    // 3. Enable PWM Clock (SYSD_PWM is bit 29)
    hw_sys_ctrl->Clk_Sys_Enable = (1 << 29);

    // 4. Spinloop: Toggle LED infinitely using PWL1 (Pulse Width Light 1)
    while (1) {
        // Turn PWL1 fully ON (Force High, Output Enable)
        hw_pwm->PWL1_Config = (1 << 20) | (1 << 18); // PWM_PWL1_SET_OE | PWM_PWL1_FORCE_H
        delay(500000);

        // Turn PWL1 fully OFF (Force Low, Output Enable)
        hw_pwm->PWL1_Config = (1 << 20) | (1 << 17); // PWM_PWL1_SET_OE | PWM_PWL1_FORCE_L
        delay(500000);
    }

    return 0; // Unreachable
}
